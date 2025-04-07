<?
	//including connection file
	include("db/conn.php");

	//including session variables
	include("sessions.inc");

	//including function file
	include("globals/functions.inc");
	
	//including config file
	include("globals/config.inc");
	
	//including api file
	include("api/tasks.inc");
	
	if( getSession("g_uid") == ""){
		echo "Error: Not Authorized";
	}else{
		$obj = new api_tasks;
		$obj -> task_id = filter_value( get_url_values("task_id") );
		$obj -> created_by = getSession('g_uid');
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			$obj -> task_id = filter_value( get_url_values("task_id") );
			if( get_url_values("sub_mode") != "" ){
				$sql = "select importance, urgency, x_value/page_width as x_factor, y_value/page_height as y_factor from tasks where task_id = ";
				$sql .= filter_value( get_url_values("task_id") );
				$result = query_mysql($sql, $conn);
				if( get_total_rows($result) != 0){
					$row = fetch_mysql_data($result);
					$x_factor = $row["x_factor"];
					$y_factor = $row["y_factor"];
					$urgency = $row["urgency"];
					$importance = $row["importance"];
					
					if( get_url_values("sub_mode") == "urgent" ){
						$obj -> y_value = filter_value( get_url_values("pos") );
						$obj -> page_height = filter_value( get_url_values("page_height") );
					}else{
						$obj -> x_value = filter_value( get_url_values("pos") );
						$obj -> page_width = filter_value( get_url_values("page_height") );
					}
					$obj -> updateObject();
					
					//updating the urgency and importance
					$obj -> updateUrgencyImportance();
					
					if( get_url_values("multiple") == "true" ){
						//if multiple is checked
						$sql = "select (x_value/page_width) - $x_factor as x_factor, y_value/page_height - $y_factor as y_factor ";
						$sql .= "from tasks where task_id = " . filter_value( get_url_values("task_id") );
						$result = query_mysql($sql, $conn);
						$row = fetch_mysql_data($result);
						$x_factor_diff = $row["x_factor"];
						$y_factor_diff = $row["y_factor"];
						
						//getting all the tasks after this task
						$sql = "select task_id, x_value + ceil(page_width * $x_factor_diff) as x_value, y_value + ceil(page_height * $y_factor_diff) as y_value ";
						$sql .= "from tasks where task_id != " . filter_value( get_url_values("task_id") ) . " and active = 1 and ";
						if( get_url_values("sub_mode") == "urgent" ){
							$sql .= "( (urgency = $urgency and (y_value/page_height) > $y_factor) OR urgency > $urgency)";
						}else{
							$sql .= "( (importance = $importance and (x_value/page_width) > $x_factor) OR importance > $importance)";
						}
						$result = query_mysql($sql, $conn);
						if( get_total_rows($result) != 0){
							while($row = fetch_mysql_data($result)){
								$obj -> task_id = $row["task_id"];
								if( get_url_values("sub_mode") == "urgent" ){
									$obj -> y_value = $row["y_value"];
								}else{
									$obj -> x_value = $row["x_value"];
								}
								$obj -> updateObject();
								
								//updating the urgency and importance
								$obj -> updateUrgencyImportance();
							}
						}
					}
				}
			}else{
				$obj -> x_value = filter_value( get_url_values("x_value") );
				$obj -> y_value = filter_value( get_url_values("y_value") );
				$obj -> page_width = filter_value( get_url_values("page_width") );
				$obj -> page_height = filter_value( get_url_values("page_height") );
				$obj -> updateObject();
				//updating the urgency and importance
				if( $cnfg_urgent_important_measured_by == "numbers" ) $obj -> updateUrgencyImportance();
			}
		}
	}
?>