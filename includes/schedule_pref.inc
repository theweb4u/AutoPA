<?
	$process_preferences = get_form_values("process_preferences");
	//including users api file
	include("api/users.inc");
	
	if($process_preferences == 1){
		//saving preference
		$obj = new api_user_preferences;
		$obj -> uid = getSession("g_uid");
		$added_time = 0;
		$added_time_percentage = 0;
		
		if( get_form_values("added_time_type") == "minutes"){
			$added_time = get_form_values("added_time");
		}else{
			$added_time_percentage = get_form_values("added_time");
		}

		$obj -> added_time = filter_value( $added_time );
		$obj -> added_time_in_percentage = filter_value( $added_time_percentage );
		$obj -> days_to_fit_task = filter_value( get_form_values("days_to_fit_task") );
		$obj -> min_task_1 = filter_value( get_form_values("min_task_1") );
		$obj -> min_task_2 = filter_value( get_form_values("min_task_2") );
		$obj -> min_task_3 = filter_value( get_form_values("min_task_3") );
		$obj -> min_task_4 = filter_value( get_form_values("min_task_4") );
		$obj -> updateObject();
		
		//saving working hours
		$obj = new api_working_hours;
		$obj -> uid = getSession("g_uid");
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			while($row = fetch_mysql_data($result)){				
				$obj -> id = $row["id"];
				$control_name = $row["weekday"] . "_holiday";

				if( get_form_values("$control_name") == "" ){
					$obj -> is_holiday = 0;
				}else{
					$obj -> is_holiday = filter_value( get_form_values("$control_name") );
				}
				
				if( get_form_values("$control_name") == "1" ){
					$obj -> time_from = "0:00";
					$obj -> time_to = "0:00";
				}else{
					$control_name = $row["weekday"] . "_time_from";
					$obj -> time_from = filter_value( get_form_values("$control_name") );
					
					$control_name = $row["weekday"] . "_time_to";
					$obj -> time_to = filter_value( get_form_values("$control_name") );
				}
				
				$obj -> updateObject();
			}
		}
		
		//saving break hours
		$obj = new api_break_hours;
		$obj -> uid = getSession("g_uid");
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			while($row = fetch_mysql_data($result)){
				$obj -> id = $row["id"];
				$control_name = setLowerFull($row["title"]) . "_time_from";
				$obj -> time_from = filter_value( get_form_values("$control_name") );
				
				$control_name = setLowerFull($row["title"]) . "_time_to";
				$obj -> time_to = filter_value( get_form_values("$control_name") );
				
				$control_name = setLowerFull($row["title"]) . "_variance";
				$obj -> variance = filter_value( get_form_values("$control_name") );
				
				$obj -> updateObject();
			}
		}
		
		//saving after hours
		$obj = new api_after_hours;
		$obj -> uid = getSession("g_uid");
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			while($row = fetch_mysql_data($result)){
				$obj -> id = $row["id"];
				$control_name = "ah_time_from_" . $row["id"];
				$obj -> time_from = filter_value( get_form_values("$control_name") );
				
				$control_name = "ah_time_to_" . $row["id"];
				$obj -> time_to = filter_value( get_form_values("$control_name") );
				
				$control_name = "dayname_" . $row["id"];
				$obj -> dayname = filter_value( get_form_values("$control_name") );
				
				$obj -> updateObject();
			}
		}
		
		if( get_form_values("dayname_new") != ""){
			$obj -> time_from = filter_value( get_form_values("ah_time_from_new") );
			$obj -> time_to = filter_value( get_form_values("ah_time_to_new") );
			$obj -> dayname = filter_value( get_form_values("dayname_new") );
			
			$obj -> insertObject();
		}
		
		$success_msg = "Schedule preferences updated successfully";
	}
	
	//getting existing data
	$obj = new api_user_preferences;
	$obj -> uid = getSession("g_uid");
	$result = $obj -> getDetail();
	
	if( get_total_rows($result) != 0){
		$row = fetch_mysql_data($result);
		$added_time = $row["added_time"];
		$added_time_in_percentage = $row["added_time_in_percentage"];
		$days_to_fit_task = $row["days_to_fit_task"];
		$min_task_1 = $row["min_task_1"];
		$min_task_2 = $row["min_task_2"];
		$min_task_3 = $row["min_task_3"];
		$min_task_4 = $row["min_task_4"];
		if($added_time_in_percentage > 0){
			$added_time = $added_time_in_percentage;
			$added_time_type = "percentage";
		}else{
			$added_time_type = "minutes";
		}
	}
	
	$obj = new api_working_hours;
	$obj -> uid = getSession("g_uid");
	$result = $obj -> getDetail();
	
	if( get_total_rows($result) != 0){
		while($row = fetch_mysql_data($result)){
			$control_name = $row["weekday"] . "_time_from";
			$$control_name = $row["time_from"];
			
			$control_name = $row["weekday"] . "_time_to";
			$$control_name = $row["time_to"];
			
			$control_name = $row["weekday"] . "_holiday";
			$$control_name = $row["is_holiday"];
		}
	}
	
	$obj = new api_break_hours;
	$obj -> uid = getSession("g_uid");
	$result = $obj -> getDetail();
	
	if( get_total_rows($result) != 0){
		while($row = fetch_mysql_data($result)){
			$control_name = setLowerFull($row["title"]) . "_time_from";
			$$control_name = $row["time_from"];
			
			$control_name = setLowerFull($row["title"]) . "_time_to";
			$$control_name = $row["time_to"];
			
			$control_name = setLowerFull($row["title"]) . "_variance";
			$$control_name = $row["variance"];
		}
	}
?>
	<form method="post" action="index.php">
		<input type="hidden" name="process_preferences" value="1">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="schedule_pref">
		
		<table class="tblEdit" width="700px">
			<?if($success_msg != ""){?>
			<tr>
				<td class="success" colspan="4"><?echo $success_msg;?></td>
			</tr>
			<?}else{?>
			<tr>
				<td colspan="4" class="editHeader">Schedule Preferences<br><br></td>
			</tr>
			<?}?>
			<tr>
				<td class="frm_caption">Days to try and fit tasks:</td>
				<td class="frm_input"><input type="text" class="textboxR" name="days_to_fit_task" size="4" value="<?echo $days_to_fit_task ;?>"> Days</td>
				<td class="frm_caption">Added Time:</td>
				<td class="frm_input">
					<input type="text" class="textboxR" name="added_time" size="4" value="<?echo $added_time;?>"> 
					<input type="radio" name="added_time_type" value="minutes" <?if($added_time_type == "minutes") echo "checked"?>> Minutes
					<input type="radio" name="added_time_type" value="percentage" <?if($added_time_type == "percentage") echo "checked"?>> %
				</td>
			</tr>
			<tr>
				<td class="frm_caption" colspan="4">Minimum Tasks per Segment:</td>
			</tr>
			<tr>
				<td colspan="4">
					<table width="100%">
						<tr>
							<td class="frm_caption">Segment 1<br>(Urgent & Important)</td>
							<td class="frm_caption">Segment 2<br>(Not Urgent but Important)</td>
							<td class="frm_caption">Segment 3<br>(Urgent but Not Important)</td>
							<td class="frm_caption">Segment 4<br>(Not Urgent and Not Important)</td>
						</tr>
						<tr>
							<td>
							<select name="min_task_1" id="min_task_1" class="select-box">
								<?for($counter=0;$counter<=$cnfg_max_tasks_per_segment;$counter++){?>
								<option value="<?echo $counter;?>" <?if($counter == $min_task_1) echo "selected";?>><?echo $counter;?></option>
								<?}?>
							</select>
							</td>
							<td>
							<select name="min_task_2" id="min_task_2" class="select-box">
								<?for($counter=0;$counter<=$cnfg_max_tasks_per_segment;$counter++){?>
								<option value="<?echo $counter;?>" <?if($counter == $min_task_2) echo "selected";?>><?echo $counter;?></option>
								<?}?>
							</select>
							</td>
							<td>
							<select name="min_task_3" id="min_task_3" class="select-box">
								<?for($counter=0;$counter<=$cnfg_max_tasks_per_segment;$counter++){?>
								<option value="<?echo $counter;?>" <?if($counter == $min_task_3) echo "selected";?>><?echo $counter;?></option>
								<?}?>
							</select>
							</td>
							<td>
							<select name="min_task_4" id="min_task_4" class="select-box">
								<?for($counter=0;$counter<=$cnfg_max_tasks_per_segment;$counter++){?>
								<option value="<?echo $counter;?>" <?if($counter == $min_task_4) echo "selected";?>><?echo $counter;?></option>
								<?}?>
							</select>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td colspan="4">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="4">
					<script language="javascript">
						function updateHours( weekday ){
							if( $(weekday + '_holiday').checked ){
								$(weekday + '_time_from').value = "";
								$(weekday + '_time_to').value = "";
								$(weekday + '_time_from').disabled = true;
								$(weekday + '_time_to').disabled = true;
							}else{
								$(weekday + '_time_from').disabled = false;
								$(weekday + '_time_to').disabled = false;
							}
						}
					</script>
					<table width="100%">
						<tr>
							<td colspan="4"><b>Set Working Hours</b><br></td>
						</tr>
						<tr>
							<td>Weekday</td>
							<td>Hour From</td>
							<td>Hour To</td>
							<td>Holiday</td>
						</tr>
						<?
							$week_arr = getSplitArray($cnfg_weekdays, ",");
							for($counter=0;$counter<count($week_arr);$counter++){
						?>
						<tr>
							<td><?echo setUpperFull( $week_arr[$counter] );?></td>
							<td>
								<?
									$control_name = $week_arr[$counter] . "_time_from";
									$control_value = $$control_name;
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
							<td>
								<?
									$control_name = $week_arr[$counter] . "_time_to";
									$control_value = $$control_name;
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
							<td>
								<?
									$control_name = $week_arr[$counter] . "_holiday";
									$control_value = $$control_name;
								?>
								<input type="checkbox" name="<?echo $control_name;?>" id="<?echo $control_name;?>" value="1" <?if($control_value == 1) echo "checked";?> onclick="updateHours( '<?echo $week_arr[$counter];?>' )">
								<?if($control_value == 1){?><script language="javascript">updateHours( '<?echo $week_arr[$counter];?>' );</script><?}?>
							</td>
						</tr>
						<?
							}
						?>
					</table>
				</td>
			</tr>
			<tr>
				<td colspan="4">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="4">
					<table width="100%">
						<tr>
							<td colspan="4"><b>Set Break Times</b><br></td>
						</tr>
						<tr>
							<td>Break Type</td>
							<td>Time From</td>
							<td>Time To</td>
							<td style="display:none;">Variance</td>
						</tr>
						<?
							$obj = new api_break_hours;
							$obj -> uid = getSession("g_uid");
							$result = $obj -> getDetail();
							
							while($row = fetch_mysql_data($result)){
								$control_name_from = setLowerFull($row["title"]) . "_time_from";
								$control_name_to = setLowerFull($row["title"]) . "_time_to";
								$control_name_variance = setLowerFull($row["title"]) . "_variance";
						?>
						<tr>
							<td><?echo setUpperFull( $row["title"] );?></td>
							<td>
								<?
									$control_value = $$control_name_from;
									create_time_dropdown($control_name_from, $control_value)
								?>
							</td>
							<td>
								<?
									$control_value = $$control_name_to;
									create_time_dropdown($control_name_to, $control_value)
								?>
							</td>
							<td style="display:none;">
								<?
									$control_value = $$control_name_variance;
								?>
								<input type="text" class="textboxR" name="<?echo $control_name_variance;?>" size="5" value="<?echo $control_value;?>">
							</td>
						</tr>
						<?
							}
						?>
					</table>
				</td>
			</tr>
			<tr>
				<td colspan="4">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="4">
					<table width="100%">
						<tr>
							<td colspan="3"><b>Set After Hours</b><br></td>
						</tr>
						<tr>
							<td>Weekday</td>
							<td>Time From</td>
							<td>Time To</td>
						</tr>
						<?
							$obj = new api_after_hours;
							$obj -> uid = getSession("g_uid");
							$result = $obj -> getDetail();
							
							if( get_total_rows($result) != 0){
								while($row = fetch_mysql_data($result)){
						?>
						<tr>
							<td>
								<select name="dayname_<?echo $row["id"];?>">
									<?for($counter=0;$counter<count($week_arr);$counter++){?>
									<option value="<?echo setUpperFull( $week_arr[$counter] );?>" <?if(setUpperFull( $week_arr[$counter] ) == $row["dayname"] ) echo "selected";?>><?echo setUpperFull( $week_arr[$counter] );?></option>
									<?}?>
								</select>
							</td>
							<td>
								<?
									$control_name = "ah_time_from_" . $row["id"];
									$control_value = $row["time_from"];
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
							<td>
								<?
									$control_name = "ah_time_to_" . $row["id"];
									$control_value = $row["time_to"];
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
						</tr>
						<?
								}
							}
						?>
						<tr>
							<td>
								<select name="dayname_new">
									<option value="">Select</option>
									<?for($counter=0;$counter<count($week_arr);$counter++){?>
									<option value="<?echo setUpperFull( $week_arr[$counter] );?>"><?echo setUpperFull( $week_arr[$counter] );?></option>
									<?}?>
								</select>
							</td>
							<td>
								<?create_time_dropdown("ah_time_from_new", "")?>
							</td>
							<td>
								<?create_time_dropdown("ah_time_to_new", "")?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td class="frm_input" colspan="4">
					<input type="submit" class="button" value="Update">
				</td>
			</tr>
		</table>
	</form>