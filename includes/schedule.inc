<?	
	//including api file
	include("api/schedule.inc");
	include("api/events.inc");
	include("api/tasks.inc");

	$call_mode = get_form_values("call_mode");
	if($call_mode == "") $call_mode = get_url_values("call_mode");
	$sub_mode = get_form_values("sub_mode");
	if($sub_mode == "") $sub_mode = get_url_values("sub_mode");
	
	if($sub_mode == "schedule_detail"){
		if (file_exists("includes/schedule_detail.inc")) include("includes/schedule_detail.inc");
		$include_found = true;
	}
	
	if($sub_mode == ""){
		if($call_mode == "delete"){
			$obj = new api_schedule_master;
			$obj -> id = filter_value( get_url_values("schedule_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$obj -> active = 0;
				$obj -> updateObject();
				$error = "Requested schedule deleted successfully";
			}else{
				$error = "Requested schedule not available for editing";
			}
		}
		
		if($call_mode == "save"){
		
			$schedule_from = get_form_values("schedule_from");
			if($schedule_from == ""){
				$error = "Check the fields marked with *";
				$schedule_from_error = "*";
			}else{
				if( $cnfg_urgent_important_measured_by != "numbers" ){
					for($counter=1;$counter<5;$counter++){
						if( get_form_values("segment_$counter") != ""){
							if($include_segment != "") $include_segment .= ",";
							$include_segment .= $counter;
						}
					}
					
					if($include_segment == ""){
						$error = "Atleast 1 segment needs to be checked";
						$segment_error = "*";
					}
				}
			}
			
			if($error == ""){
				$obj = new api_schedule_master;
				
				$schedule_from_arr = getSplitArray( $schedule_from, " ");
				$schedule_from_arr2 = getSplitArray( $schedule_from_arr[0], "-");
				$date_value = $schedule_from_arr2[2] . "-" . $schedule_from_arr2[1] . "-" . $schedule_from_arr2[0] . " " . $schedule_from_arr[1] . ":00";
				$time_value = $schedule_from_arr[1] . ":00";
				$obj -> start_from = $date_value;
				
				$obj -> active = "1";
				$obj -> uid = getSession('g_uid');
				if( $cnfg_urgent_important_measured_by != "numbers" ) $obj -> include_segment = $include_segment;
				$schedule_id = $obj -> updateObject();
				
				//formatting the start date to date only
				$date_value = $schedule_from_arr2[2] . "-" . $schedule_from_arr2[1] . "-" . $schedule_from_arr2[0];
			}else{
				$call_mode = "new";
			}
		}
		
		if($call_mode == "reload_schedule"){
			$obj = new api_schedule_master;
			$obj -> id = filter_value( get_url_values("schedule_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$schedule_id = get_url_values("schedule_id");
				$row = fetch_mysql_data($result);
				$schedule_from_arr = getSplitArray( $row["start_from"], " ");
				$date_value = $schedule_from_arr[0];
				$time_value = $schedule_from_arr[1];
				if( $cnfg_urgent_important_measured_by != "numbers" ) $include_segment = $row["include_segment"];
				
				$obj = new api_schedule_details;
				$obj -> schedule_id = filter_value( $schedule_id );
				$result = $obj -> getDetail();
				if( get_total_rows($result) != 0){
					while($row = fetch_mysql_data($result)){	
						$obj -> id = filter_value( $row["id"] );
						$obj -> active = 0;
						$obj -> updateObject();
					}
				}				
			}else{
				$error = "Requested schedule not available for editing";
			}
		}
		
		if($error == ""){
			if($call_mode == "save" || $call_mode == "reload_schedule"){
				/*
					Process
					=======
					.. First all the break hours and holidays are inserted for the entire schedule duration
					.. Start date is validated to be greater than equal to current date.  Same with time.
					.. Task is browsed based on their priority and order by
					.. For each task the first date of the schedule period is taken as start point
					.. added time from user perferences is added to the total duration of tasks.
					.. Now a loop is done while the start date is <= to finish date of the schedule period
					.. For Each loop
						.. validation is done if start date is not one of the holidays
						.. validation is done if date is not a holiday as per sub category and if not set as per category
						.. If not holiday then start time, finish time and total duration for the day is worked out
						.. If subcategory time is set its used else if category time is set it is used instead of daily hours
						.. Start time is now validated so its not between any existing schedule 
						.. If not valid then the next start time is scheduled as the finish time + 1 min for the existing period found
						.. Once valid start time is found then duration is added to the start time and finish time is calculated
						.. Finish time is also then validated so its not between any existing schedule or end of the day
						.. If its between any schedule then finish time is calculated as 1 min less than the start time of the found period
						.. Exception is in the case of breaks and if break_time is set to 0 then those are ignored for completion of the particular tasks.
						.. Task is split only if split is allowed and each duration is more than the specified time.
						.. Once valid finish time is found then schedule is inserted and duration for the task is inserted
						.. For each day after hours is also used if available.
						.. Loop continuess till duration is 0 or is the end of the period. If end of period is reached then task assignment is cancelled and reported
						.. After entire duration is scheduled then date due for the tasks is matched and checked if all schedule is before that and if not assignment is all deleted
				*/
				
				//comparing the start date to todays date and if less then setting todays date as start date
				$todays_date = date('Y-m-d');
				$result = query_mysql("select '$date_value' < '$todays_date' as isLess, '$date_value' = '$todays_date' as isSame", $conn);
				if( get_total_rows($result) != 0){
					$row = fetch_mysql_data($result);
					if( $row["isLess"] == 1 || $row["isSame"] == 1 ){
						$date_value = $todays_date;
						$start_time_change_required = 1;
					} 
				}
				
				//getting the end date and forming the date condition.  Default added time pref is 0 and minutes
				$added_time = 0;
				$added_time_type = "minutes";
				$result = query_mysql("select added_time, added_time_in_percentage, ifnull(days_to_fit_task, 0) as days_to_fit_task from user_preferences where uid = " . getSession('g_uid'), $conn);
				if( get_total_rows($result) != 0){
					$row = fetch_mysql_data($result);
					$days_to_fit_task = $row["days_to_fit_task"];
					if( $row["added_time_in_percentage"] > 0 ){
						$added_time = $row["added_time_in_percentage"];
						$added_time_type = "percentage";
					}else{
						$added_time = $row["added_time"];
						$added_time_type = "minutes";
					}
				}
				if($days_to_fit_task == "" || $days_to_fit_task == 0) $days_to_fit_task = 30;				
				$result = query_mysql("select date_add('$date_value', interval $days_to_fit_task day) as end_date", $conn);
				$row = fetch_mysql_data($result);
				$end_date = $row["end_date"];
				
				//creating variable for after hours
				$sql = "select lcase(dayname) as dayname, time_from, time_to from after_hours where uid = " . getSession('g_uid');
				$result = query_mysql($sql, $conn);
				while($row = fetch_mysql_data($result)){
					$variable_name = "after_hour_from_" . $row["dayname"];
					$$variable_name = $row["time_from"];
					$variable_name = "after_hour_to_" . $row["dayname"];
					$$variable_name = $row["time_to"];
				}
				
				//creating variable for working hours
				$sql = "select weekday, time_from, time_to, is_holiday, ";
				$sql .= "( 60 * ( HOUR( TIMEDIFF( time_to, time_from ) ) ) ) + ( MINUTE( TIMEDIFF( time_to, time_from ) ) ) AS total_minutes "; 
				$sql .= "from working_hours where uid = " . getSession('g_uid');
				$result = query_mysql($sql, $conn);
				while($row = fetch_mysql_data($result)){
					if( $row["is_holiday"] == 1 ){
						$holiday_names .= "," . setUpperFull($row["weekday"]);
					}else{
						$variable_name = "working_hour_from_" . $row["weekday"];
						$$variable_name = $row["time_from"];
						$variable_name = "working_hour_to_" . $row["weekday"];
						$$variable_name = $row["time_to"];
						
						$total_minutes = $row["total_minutes"];
						//updating total minutes if after hours applicable
						$variable_name = "after_hour_from_" . $row["weekday"];
						$after_hour_from = $$variable_name;
						$variable_name = "after_hour_to_" . $row["weekday"];
						$after_hour_to = $$variable_name;
						if( ($after_hour_from != "") && ($after_hour_to != "") ){
							$sql = "select ( 60 * ( HOUR( TIMEDIFF( '$after_hour_to', '$after_hour_from' ) ) ) ) + ( MINUTE( TIMEDIFF( '$after_hour_to', '$after_hour_from' ) ) ) AS total_minutes "; 
							$result2 = query_mysql($sql, $conn);
							$row2 = fetch_mysql_data($result2);
							$total_minutes = $total_minutes + $row2["total_minutes"];
						}
						$variable_name = "total_minutes_" . $row["weekday"];
						$$variable_name = $total_minutes;
					}
				}
				
				//generating an array of weekdays
				$week_arr = getSplitArray($cnfg_weekdays, ",");
				
				//inserting the events for the date range
				$event_time_start = $time_value;
				if( $start_time_change_required == 1 ){
					$date_temp=date_create($date_value);
					$variable_name = "working_hour_from_" . $week_arr[date_format($date_temp,"w")];
					$event_time_start = $$variable_name;
				}
				$obj = new api_events;
				$obj -> created_by = getSession('g_uid');
				$obj -> active = 1;
				$custom_condition = " ((event_date > '$date_value') OR (event_date = '$date_value' and time_from >= '" . $event_time_start . "'))";
				$custom_condition .= " and ( event_date <= '" . $end_date . "' )";
				$obj -> custom_query = $custom_condition;		
				$result = $obj -> getDetail();
				if( get_total_rows($result) != 0){
					$obj = new api_schedule_details;
					while($row = fetch_mysql_data($result)){	
						$obj -> schedule_id = $schedule_id;
						$obj -> source_id = $row["event_id"];
						$obj -> source_type = "Event";
						$obj -> schedule_date = $row["event_date"];
						$obj -> time_from = $row["time_from"];
						$obj -> time_to = $row["time_to"];
						$obj -> updateObject();
					}
				}

				//creating variable for working hours per category
				$sql = "select cp.type_id, cp.weekday, cp.time_from, cp.time_to, cp.is_holiday, ";
				$sql .= "( 60 * ( HOUR( TIMEDIFF( cp.time_to, cp.time_from ) ) ) ) + ( MINUTE( TIMEDIFF( cp.time_to, cp.time_from ) ) ) AS total_minutes "; 
				$sql .= "from category_placement cp inner join types on ";
				$sql .= "cp.type_id = types.category_id where types.active = 1 and cp.active = 1 and types.type = 'category' and ";
				$sql .= "types.created_by = " . getSession('g_uid');
				$result = query_mysql($sql, $conn);
				while($row = fetch_mysql_data($result)){
					$variable_name = "category_processed_" . $row["type_id"] . "_" . $row["weekday"];
					$$variable_name = 1;
					if( $row["is_holiday"] == 1 ){
						$variable_name = "category_holiday_" . $row["type_id"] . "_" . $row["weekday"];
						$$variable_name = 1;
					}else{
						if( $row["time_from"] != "00:00:00" && $row["time_to"] != "00:00:00" ){
							$variable_name = "category_working_hour_" . $row["type_id"] . "_" . $row["weekday"] . "_from";
							$$variable_name = $row["time_from"];
							$variable_name = "category_working_hour_" . $row["type_id"] . "_" . $row["weekday"] . "_to";
							$$variable_name = $row["time_to"];
							$variable_name = "category_working_hour_" . $row["type_id"] . "_" . $row["weekday"] . "_duration";
							$$variable_name = $row["total_minutes"];
							$variable_name = "category_working_hour_" . $row["weekday"] . "_exist";
							$$variable_name == 1;
						}
					}
				}
				
				//creating variable for break hours/holidays and inserting into the schedule
				$sql = "select title as break_name, time_from, time_to, variance from break_hours where uid = " . getSession('g_uid');
				$result = query_mysql($sql, $conn);
				$process_holidays = 1;
				while($row = fetch_mysql_data($result)){
					$start_date = $date_value;
					$loop = strtotime($start_date) <= strtotime($end_date);
					while ($loop) {
						$process_break = 1;
						$date_temp=date_create($start_date);
						
						//checking if its one of the holidays
						if( holiday_names != "" ){
							$holidayname_array = getSplitArray($holiday_names, ",");
							for($counter=1;$counter<count($holidayname_array);$counter++){
								if( setUpperFull($week_arr[date_format($date_temp,"w")]) == $holidayname_array[$counter] ){
									$process_break = 0;
									if( $process_holidays == 1 ){
										$obj = new api_schedule_details;
										$obj -> schedule_id = $schedule_id;
										$obj -> schedule_date = $start_date;
										$obj -> source_id = 0;
										$obj -> source_type = "HOLIDAY";
										$obj -> updateObject();
										$holidays_list .= "," . $start_date;
									}
								}
							}
						}
						
						if( $process_break == 1 ){
							//checking if break time is after the finish time for the day
							$variable_name = "working_hour_to_" . $week_arr[date_format($date_temp,"w")];
							$working_hour_to = $$variable_name;
							if( $row["time_from"] > $working_hour_to ) $process_break = 0;
						}
						
						if( $process_break == 1 ){
							//checking if break time is between any events
							if ( validateStartTime($schedule_id, $start_date, $row["time_from"], 0, 1) != "1" || validateFinishTime($schedule_id, $start_date, $row["time_from"], $row["time_to"], 0, 1) != "1" ){
								$process_break = 0;
							}
						}
						
						if( $process_break == 1 ){
							$obj = new api_schedule_details;
							$obj -> schedule_id = $schedule_id;
							$obj -> schedule_date = $start_date;
							$obj -> source_id = 0;
							$obj -> source_type = setUpperFull($row["break_name"]);
							$obj -> time_from = $row["time_from"];
							$obj -> time_to = $row["time_to"];
							$obj -> updateObject();
							
							//populating a break name variable
							$break_name_list .= "," . setUpperFull($row["break_name"]);
						}
						
						//moving to next date
						$start_date = date ("Y-m-d", strtotime("+1 day", strtotime($start_date)));
						$loop = strtotime($start_date) <= strtotime($end_date);
					}
					$process_holidays = 0;
				}
				
				//forming the segment condition
				if( $cnfg_urgent_important_measured_by != "numbers" ){
					$segment_arr = getSplitArray( $include_segment . ",", ",");
					for($counter=0;$counter<count($segment_arr)-1;$counter++){
						if($segment_arr[$counter] == 1){
							if($segment_condition != "") $segment_condition .= " OR ";
							$segment_condition .= " (urgency = 1 and importance = 1)";
						}else if($segment_arr[$counter] == 2){
							if($segment_condition != "") $segment_condition .= " OR ";
							$segment_condition .= " (urgency = 0 and importance = 1)";
						}else if($segment_arr[$counter] == 3){
							if($segment_condition != "") $segment_condition .= " OR ";
							$segment_condition .= " (urgency = 1 and importance = 0)";
						}else if($segment_arr[$counter] == 4){
							if($segment_condition != "") $segment_condition .= " OR ";
							$segment_condition .= " (urgency = 0 and importance = 0)";
						}
					}
					$segment_condition = " and ($segment_condition)";
				}
				
				//getting all the tasks
				$sort_type = "desc";
				if( $cnfg_urgent_important_measured_by == "numbers" ) $sort_type = "asc";
				$sql = "select tasks.title, tasks.task_id, time_allocated, due_date, tasks.after_hours, time_split, time_split_min, ";
				$sql .= "date_format( due_date, '%Y-%m-%d' ) AS date_part, date_format( due_date, '%H:%i:00' ) AS time_part, break_time, ";
				$sql .= "tasks.category_id, tasks.sub_category_id, ";
				$sql .= "IF(tasks.productive = 'Billable', 1, IF(tasks.productive = 'Productive', 2, ";
				$sql .= "IF(tasks.productive = 'Non Productive', 3, IF(tasks.productive = 'Personal', 4, 5)))) as productivity_order ";
				$sql .= "from tasks left outer join types on tasks.category_id = types.category_id left outer join types t2 on ";
				$sql .= "tasks.sub_category_id = t2.category_id where tasks.created_by = " . getSession('g_uid') . " and ";
				$sql .= " tasks.active = 1 and tasks.status = 'current' $segment_condition and ";
				$sql .= "(tasks.on_hold = '0000-00-00' OR tasks.on_hold < curdate()) order by tasks.importance $sort_type, ";
				$sql .= "tasks.urgency $sort_type, (tasks.x_value/tasks.page_width) + (tasks.y_value/tasks.page_height) asc, ";
				$sql .= "types.priority asc, t2.priority asc, productivity_order asc";
				$result = query_mysql($sql, $conn);
				if( get_total_rows($result) != 0){
				
					while($row = fetch_mysql_data($result)){	
					
						//for each task running through the dates
						$start_date = $date_value;
						$unassigned_error = "";
						$loop = strtotime($start_date) <= strtotime($end_date);
						$task_duration = $row["time_allocated"];
						if( $task_duration > 0 && $added_time > 0 ){
							//adding the added time to duration
							if( $added_time_type == "minutes" ){
								$task_duration = $task_duration + $added_time;
							}else{
								$task_duration = $task_duration + floor(($added_time*$task_duration)/100);
							}
						}
						if($task_duration <= 0) $loop = false;
						while ($loop) {
							$process_this_date = 1;
							//checking if the current date is one of the holidays or completed date
							if( $holidays_list != "" ){
								$holidays_list_array = getSplitArray($holidays_list, ",");
								if( in_array($start_date, $holidays_list_array) ) $process_this_date = 0;
							}

							if( $process_this_date == 1 ){
								//checking if category preferences exist for holidays.
								if( $row["sub_category_id"] != 0 || $row["category_id"] != 0 ){
									$date_temp=date_create($start_date);
									$check_category_main = true;
									if( $row["sub_category_id"] != 0 ){
										$variable_name = "category_processed_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
										if( $$variable_name == 1 ){
											$check_category_main = false;
											$variable_name = "category_holiday_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
											if( $$variable_name == 1 ) $process_this_date = 0;
										}
									}
									
									if( $check_category_main ){
										$variable_name = "category_processed_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
										if( $$variable_name == 1 ){
											$check_category_main = false;
											$variable_name = "category_holiday_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
											if( $$variable_name == 1 ) $process_this_date = 0;
										}
									}
								}
							}

							if( $process_this_date == 1 ){
								$date_temp=date_create($start_date);
								$variable_name = "working_hour_from_" . $week_arr[date_format($date_temp,"w")];
								$start_time = $$variable_name;
								$variable_name = "working_hour_to_" . $week_arr[date_format($date_temp,"w")];
								$finish_time = $$variable_name;
								$variable_name = "total_minutes_" . $week_arr[date_format($date_temp,"w")];
								$total_minutes = $$variable_name;
								$loop_error = false;
								$category_exception = false;
								
								if( $row["sub_category_id"] != 0 || $row["category_id"] != 0 ){
									//if category preference exist changing the start and finish time
									$check_category_main = true;
									if( $row["sub_category_id"] != 0 ){
										$variable_name = "category_processed_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
										if( $$variable_name == 1 ){
											$variable_name = "category_working_hour_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_from";
											if( $$variable_name != "" ){
												$check_category_main = false;
												$start_time = $$variable_name;
												$variable_name = "category_working_hour_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_to";
												$finish_time = $$variable_name;
												$variable_name = "category_working_hour_" . $row["sub_category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_duration";
												$total_minutes = $$variable_name;
												$category_exception = true;
											}
										}
									}
									
									if( $check_category_main ){
										$variable_name = "category_processed_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")];
										if( $$variable_name == 1 ){
											$variable_name = "category_working_hour_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_from";
											if( $$variable_name != "" ){
												$check_category_main = false;
												$start_time = $$variable_name;
												$variable_name = "category_working_hour_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_to";
												$finish_time = $$variable_name;
												$variable_name = "category_working_hour_" . $row["category_id"] . "_" . $week_arr[date_format($date_temp,"w")] . "_duration";
												$total_minutes = $$variable_name;
												$category_exception = true;
											}
										}
									}
								}
								
								//if current date is the first day and time check required is evaluated to 1 then checking start time to be greater than current time
								if( $start_date == $date_value && $start_time_change_required == 1 ){
									$result_time = query_mysql("select '$start_time' < curtime() as isLess, curtime() as time_now", $conn);
									if( get_total_rows($result_time) != 0){
										$row_time = fetch_mysql_data($result_time);
										if( $row_time["isLess"] == 1 ) $start_time = $row_time["time_now"];
									}
								}
								
								//while start time is greater than finish time and task duration is greater than 0
								while( (strtotime($start_time) < strtotime($finish_time)) && ($task_duration > 0) && ($loop_error == false)){
									$loop_error = populateSchedule($row["task_id"], $row["time_split"], $row["time_split_min"], $row["break_time"]);
									
									if( $loop_error && ($category_exception == false) ){
										//checking if the day has after hours only if no category preferences exist
										$variable_name = "after_hour_from_" . $week_arr[date_format($date_temp,"w")];
										$after_hour_from = $$variable_name;
										$variable_name = "after_hour_to_" . $week_arr[date_format($date_temp,"w")];
										$after_hour_to = $$variable_name;
										if( ($after_hour_from != "") && ($after_hour_to != "") && ($task_duration > 0) && $row["after_hours"] != "0"){
											$start_time = $after_hour_from;
											$finish_time = $after_hour_to;
											$temp_var = populateSchedule($row["task_id"], $row["time_split"], $row["time_split_min"], $row["break_time"]);
										}
									}
								}
								
								if( $category_exception == false ){
									$variable_name = "category_working_hour_" . $week_arr[date_format($date_temp,"w")] . "_exist";
									if( $$variable_name != 1 ){
										//checking if any time left for the day else adding it to the holidays only if no category preferences exist
										$sql = "SELECT $total_minutes - sum( ( 60 * ( HOUR( TIMEDIFF( time_to, time_from ) ) ) ) + ( MINUTE( ";
										$sql .= "TIMEDIFF( time_to, time_from ) ) ) ) > 0 AS minutes_left FROM `schedule_details`WHERE schedule_date = '$start_date' ";
										$sql .= "AND active = 1 AND schedule_id = $schedule_id";
										$result2 = query_mysql($sql, $conn);
										if( get_total_rows($result2) != 0){
											$row2 = fetch_mysql_data($result2);
											if( $row2["minutes_left"] != 1 ){
												$holidays_list .= "," . $start_date;
											}
										}
									}
								}
							}
							
							//moving to next date and deciding the loop status
							$start_date = date ("Y-m-d", strtotime("+1 day", strtotime($start_date)));
							$loop = strtotime($start_date) <= strtotime($end_date);
							if($task_duration <= 0){
								$loop = false;
								//checking if due date exist for the tasks and created schedule is less than or equal to date due
								if( $row["due_date"] != "0000-00-00 00:00:00" ){
									$sql = "SELECT id from schedule_details where active = 1 and schedule_id = $schedule_id and source_type = 'Task' ";
									$sql .= "and source_id = '" . $row["task_id"] . "' and (schedule_date > '" . $row["date_part"] . "' OR ";
									$sql .= "(schedule_date = '" . $row["date_part"] . "' and time_to > '" . $row["time_part"] . "')) limit 1";
									
									$result2 = query_mysql($sql, $conn);
									if( get_total_rows($result2) != 0){
										$unassigned_error_master .= "<br><br><br><b>" . $row["title"] . " [Task#: " . $row["task_id"] . "] (Reason: Can not be fit into the schedule before specified due date)";
										//delete commented as per request
										//$sql = "delete from schedule_details where source_type = 'Task' and source_id = " . $row["task_id"] . " and schedule_id = " . $schedule_id;
										//query_mysql($sql, $conn);
									}
								}
							}
						}
						
						if($task_duration > 0){
							//unassigned task error
							$unassigned_error_master .= "<br><br><br><b>" . $row["title"] . " [Task#: " . $row["task_id"] . "] (Reason: End date too short to fit the task.)";
							$sql = "delete from schedule_details where source_type = 'Task' and source_id = " . $row["task_id"] . " and schedule_id = " . $schedule_id;
							query_mysql($sql, $conn);
						}
					}
				}
				
				$error = "Schedule successfully created";
				if( $unassigned_error_master != ""){
					$error = "Click on ! to view the schedule process status";
					$obj = new api_schedule_master;
					$obj -> id = $schedule_id;
					$obj -> status = "Following tasks could not be fit into the schedule<br>$unassigned_error_master";
					$obj -> updateObject();
				}else{
					$obj = new api_schedule_master;
					$obj -> id = $schedule_id;
					$obj -> status = "";
					$obj -> updateObject();
				}
				
			}
		}
		
		if($call_mode != "new"){
			$obj = new api_schedule_master;
			$obj -> created_by = getSession('g_uid');
			$obj -> active = 1;
			$obj -> order_by = "created desc";
			$result = $obj -> getDetail();
?>
	<table class="tblDisplay" width="800px">
		<?$colspan = 7;?>
		<tr class="header">
			<td class="action">@</td>
			<td class="action">@</td>
			<td class="action">@</td>
			<td>Created</td>
			<td>Schedule From</td>
			<?
			if( $cnfg_urgent_important_measured_by != "numbers" ){
				$colspan = 6;
			?>
			<td class="action">Segments Included</td>
			<?}?>
			<td class="action">
				<a href="index.php?mode=schedule&call_mode=new">
					<img src="images/add.gif" title="Add Schedule" border="0"></a>
			</td>
		</tr>
		<?
		if( get_total_rows($result) != 0){
			$class = "even";
			while($row = fetch_mysql_data($result)){
				if($class == "even") $class = "odd";
				else $class = "even";	
		?>
		<tr class="<?echo $class;?>">
			<td class="action">
				<a href="javascript:popupcenter('index.php?mode=schedule&sub_mode=schedule_detail&template=no&schedule_id=<?echo $row["id"];?>', 1100, 600)"><img src="images/view.gif" title="View Schedule" border="0"></a>
			</td>
			<td class="action">
				<?if($row["status"] != ""){?>
					<a href="javascript:popupcenter('index.php?mode=schedule&sub_mode=schedule_detail&template=no&schedule_id=<?echo $row["id"];?>&show_status=1', 800, 600)"><img src="images/exclaim.gif" title="View Schedule Status" border="0"></a>
				<?}?> 
			</td>
			<td class="action">
				<a href="index.php?mode=schedule&call_mode=reload_schedule&schedule_id=<?echo $row["id"];?>"><img src="images/reload_action.gif" title="Reload Schedule" border="0"></a>
			</td>
			<td><?echo $row["created_formatted"];?></td>
			<td><?echo $row["start_from_formatted"];?></td>
			<?if( $cnfg_urgent_important_measured_by != "numbers" ){?><td><?echo $row["include_segment"];?></td><?}?>
			<td>
				<a href="index.php?mode=schedule&schedule_id=<?echo $row["id"];?>&call_mode=delete" onclick="return confirm('Are you sure to delete this Schedule?');">
					<img src="images/trash.gif" title="Delete Schedule" border="0"></a>
			</td>
		</tr>
		<?
			}
		}else{
		?>
		<tr class="noResult">
			<td colspan="<?echo $colspan;?>">No Schedule found to display</td>
		</tr>
		<?}?>
	</table>
	<?if($error != ""){?>
	<script language="javascript">alert("<?echo $error;?>");</script>
	<?}?>
<?
		}else{
			//adding a new schedule
?>
<form method="post" action="index.php" name="editform">
	<input type="hidden" name="call_mode" id="call_mode" value="save">
	<input type="hidden" name="mode" value="schedule">
	
	<table class="tblEdit" width="700px" border="0">
		<tr>
			<td colspan="4" class="editHeader">Create Schedule<br><br></td>
		</tr>
		<?if($error == "1"){?>
		<tr>
			<td colspan="4" align="center"><b><font color="red"><?echo $error;?></font></b></td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_caption"><?echo $schedule_from_error;?>Schedule From:</td>
			<td class="frm_input" colspan="3">
				<?create_datetime_dropdown("schedule_from", $schedule_from)?>
			</td>
		</tr>
		<?if( $cnfg_urgent_important_measured_by != "numbers" ){?>
		<tr>
			<td class="frm_caption"><?echo $segment_error;?>Include Segment:</td>
			<td class="frm_input" colspan="3">
				<?for($counter=1;$counter<5;$counter++){?>
				<input type="checkbox" name="segment_<?echo $counter?>" value="1" checked>Segment <?echo $counter?>&nbsp;&nbsp;
				<?}?>
			</td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_input" colspan="4">
				<input type="submit" class="button" value="Create Schedule" style="width:200px;">
				<input type="button" class="button" value="Cancel" onclick="location.href='index.php?mode=schedule';">
			</td>
		</tr>
	</table>
</form>
<?			
		}
	}
?>