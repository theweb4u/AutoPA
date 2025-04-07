<?
	function query_mysql( $sql, $conn ){
		return mysqli_query($conn, $sql);
	}
	
	function get_total_rows( $result ){
		return mysqli_num_rows($result);
	}
	
	function fetch_mysql_data( $result ){
		return mysqli_fetch_array($result);
	}
	
	function get_random_string(){
		$valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#-+=?{}[]|";
		$length = 55;
		// start with an empty random string
		$random_string = "";
		
		// count the number of chars in the valid chars string so we know how many choices we have
		$num_valid_chars = strlen($valid_chars);
		
		// repeat the steps until we've created a string of the right length
		for ($i = 0; $i < $length; $i++){
			// pick a random number from 1 up to the number of valid chars
			$random_pick = mt_rand(1, $num_valid_chars);
			
			// take the random character out of the string of valid chars
			// subtract 1 from $random_pick because strings are indexed starting at 0, and we started picking at 1
			$random_char = $valid_chars[$random_pick-1];
			
			// add the randomly-chosen char onto the end of our string so far
			$random_string .= $random_char;
		}
		
		// return our finished random string
		return $random_string;
	}

	function get_form_values($control_name){
		//returns the value in the supplied form control
		//global $$control_name;
		//return $$control_name;
		return $_POST[$control_name];
	}

	function get_url_values($control_name){
		//returns the value in the supplied query variable
		//global $$control_name;
		//return $$control_name;
		return $_GET[$control_name];
	}

	function filter_value($input){
		//filters the value before supplier in a db query
		return $input;
	}

	function setUpper($input){
		return ucwords( $input );
	}
	
	function setUpperFull($input){
		return strtoupper( $input );
	}
	
	function setLowerFull($input){
		return strtolower( $input );
	}
	
	function create_time_dropdown($control_name, $control_value){
		global $time_picker_variable_declared;
		if($time_picker_variable_declared != "1"){
?>
	<script language="javascript">var previous_time_picker = "";</script>
<?
			$time_picker_variable_declared = "1";
		}
?>
	<input name="<?echo $control_name;?>" id="<?echo $control_name;?>" class="timepicker" value="<?echo $control_value;?>" size="10" onclick="if(previous_time_picker != '' && previous_time_picker != '<?echo $control_name;?>') $(previous_time_picker).up().style.position='';previous_time_picker='<?echo $control_name;?>';">
<?	
	}
	
	function create_datetime_dropdown($control_name, $control_value){
		global $datetime_picker_variable_declared;
		if($datetime_picker_variable_declared != "1"){
?>
	<script language="javascript">var previous_datetime_picker = "";</script>
<?
			$datetime_picker_variable_declared = "1";
		}
?>
	<input name="<?echo $control_name;?>" id="<?echo $control_name;?>" class="datetimepicker" value="<?echo $control_value;?>" size="20" onclick="if(previous_datetime_picker != '' && previous_datetime_picker != '<?echo $control_name;?>') $(previous_datetime_picker).up().style.position='';previous_datetime_picker='<?echo $control_name;?>';">
<?	
	}
	
	function create_date_dropdown($control_name, $control_value){
		global $date_picker_variable_declared;
		if($date_picker_variable_declared != "1"){
?>
	<script language="javascript">var previous_date_picker = "";</script>
<?
			$date_picker_variable_declared = "1";
		}
?>
	<input name="<?echo $control_name;?>" id="<?echo $control_name;?>" class="datepicker" value="<?echo $control_value;?>" size="20" onclick="if(previous_date_picker != '' && previous_date_picker != '<?echo $control_name;?>') $(previous_date_picker).up().style.position='';previous_date_picker='<?echo $control_name;?>';">
<?	
	}
	
	function getSplitArray($input, $sep){
		return explode( $sep, $input );
	}
	
	function validateStartTime($schedule_id, $current_date, $start_time, $task_id, $break_time){
		global $conn;
		$sql = "SELECT addtime( time_to, '00:01:00' ) AS new_time FROM `schedule_details`WHERE schedule_date = '$current_date' ";
		$sql .= "AND schedule_id = $schedule_id and active = 1 and ('$start_time' between time_from and time_to) order by time_from limit 1";
		$result2 = query_mysql($sql, $conn);
		
		if( get_total_rows($result2) != 0){
			$row = fetch_mysql_data($result2);
			return $row["new_time"];
		}else{
			return 1;
		}
	}
	
	function validateFinishTime($schedule_id, $current_date, $start_time, $finish_time, $task_id, $break_time){
		global $conn;
		global $break_name_list;
		global $g_break_exception_id;
		global $g_break_exception_delete;
		global $g_break_exception_time;
		
		$sql = "SELECT subtime( time_from, '00:01:00' ) AS new_time, '$finish_time' >= time_to as finish_time_more, source_type, id, ";
		$sql .= "addtime( time_to, '00:01:00' ) as new_task_start_time, time_to, addtime( '$finish_time', '00:01:00' ) as new_break_start_time, ";
		$sql .= "'$finish_time' = time_to as finish_time_same FROM `schedule_details`WHERE schedule_date = '$current_date' ";
		$sql .= "AND schedule_id = $schedule_id and active = 1 and (time_from between '$start_time' and '$finish_time') order by time_from limit 1";
		$result2 = query_mysql($sql, $conn);
		
		if( get_total_rows($result2) != 0){
			$row = fetch_mysql_data($result2);
			/*
			if source type is break and break time is set to 0
			set a break variable with break id
			check if finish time is more than or equal to end of break
				set a variable with delete break
				if yes and more then validate finish time again with start time as end of break + 1
					if not ok or time was equal (set a variable with break finish time)
				return 1 
				if no then return 1 ( set a variable with new break start time)
			*/
			$break_exception = false;
			$break_arr = getSplitArray($break_name_list, ",");
			if( in_array($row["source_type"], $break_arr) && $break_time == 0){
				$break_exception = true;
				$g_break_exception_id = $row["id"];
				if( $row["finish_time_more"] == 1 ){
					$g_break_exception_delete = 1;
					if( $row["finish_time_same"] == "1" || (validateFinishTime($schedule_id, $current_date, $row["new_task_start_time"], $finish_time, $task_id, 1)) != 1 ) $g_break_exception_time = $row["time_to"];
				}else{
					$g_break_exception_time = $row["new_break_start_time"];
				}
			}
			
			if( $break_exception ) return 1;
			else return $row["new_time"];
		}else{
			return 1;
		}
	}
	
	function populateSchedule($task_id, $split_ok, $split_min, $break_time){
		global $schedule_id;
		global $start_date;
		global $start_time;
		global $finish_time;
		global $task_duration;
		global $conn;
		global $g_break_exception_id;
		global $g_break_exception_delete;
		global $g_break_exception_time;
		
		//initiating the variables
		$g_break_exception_id = 0;
		$g_break_exception_delete = 0;
		$g_break_exception_time = "";
		
		//validating the start time
		$startTimeOK = validateStartTime($schedule_id, $start_date, $start_time, $task_id, $break_time);
		while( $startTimeOK != 1 ){
			$start_time = $startTimeOK;
			$startTimeOK = validateStartTime($schedule_id, $start_date, $start_time, $task_id, $break_time);
		}
		
		if( $startTimeOK == 1  && (strtotime($start_time) < strtotime($finish_time))){
			//adding task duration
			$sql = "SELECT addtime( '$start_time', concat( if( floor( $task_duration /60 ) <10, concat( '0', ";
			$sql .= "floor( $task_duration /60 ) ) , floor( $task_duration /60 ) ) , ':', if( $task_duration - ( 60 * ( ";
			$sql .= "floor( $task_duration /60 ) ) ) <10, concat( '0', $task_duration - ( 60 * ( floor( $task_duration /60 ) ) ) ) , ";
			$sql .= "$task_duration - ( 60 * ( floor( $task_duration /60 ) ) ) ) , ':00' ) ) AS finish_time, ";
			$sql .= "addtime( '$start_time', concat( if( floor( $task_duration /60 ) <10, concat( '0', ";
			$sql .= "floor( $task_duration /60 ) ) , floor( $task_duration /60 ) ) , ':', if( $task_duration - ( 60 * ( ";
			$sql .= "floor( $task_duration /60 ) ) ) <10, concat( '0', $task_duration - ( 60 * ( floor( $task_duration /60 ) ) ) ) , ";
			$sql .= "$task_duration - ( 60 * ( floor( $task_duration /60 ) ) ) ) , ':00' ) ) > '$finish_time' as isGreater";
			$result2 = query_mysql($sql, $conn);
			$row2 = fetch_mysql_data($result2);
			//if greater than end of day setting the finish time to end of day
			if( $row2["isGreater"] == 1 ){
				$task_finish_time = $finish_time;
			}else{
				$task_finish_time = $row2["finish_time"];
			}
			
			//validating the finish time
			$finishTimeOK = validateFinishTime($schedule_id, $start_date, $start_time, $task_finish_time, $task_id, $break_time);
			while( $finishTimeOK != 1 ){
				$task_finish_time = $finishTimeOK;
				$finishTimeOK = validateFinishTime($schedule_id, $start_date, $start_time, $task_finish_time, $task_id, $break_time);
			}
			
			if( $finishTimeOK == 1 ){
				//if break is compromised updating the task finish time
				if( $g_break_exception_id != 0 && $g_break_exception_delete != 0 && $g_break_exception_time != "" ) $task_finish_time = $g_break_exception_time;
				
				$sql = "select addtime( '$task_finish_time', '00:01:00' ) AS new_start_time, $task_duration - ";
				$sql .= "(( 60 * ( HOUR( TIMEDIFF( '$task_finish_time', '$start_time' ) ) ) ) + ( MINUTE( TIMEDIFF( '$task_finish_time', '$start_time' ) ) ) ) AS duration_left, "; 
				$sql .= "(( 60 * ( HOUR( TIMEDIFF( '$task_finish_time', '$start_time' ) ) ) ) + ( MINUTE( TIMEDIFF( '$task_finish_time', '$start_time' ) ) ) ) >= $split_min as min_split_ok";
				$result2 = query_mysql($sql, $conn);
				$row2 = fetch_mysql_data($result2);
				
				//scheduling the tasks based on split allowed and min split time.
				$split_condition_ok = true;
				if( $split_ok == 0 && $row2["duration_left"] != 0 ) $split_condition_ok = false;
				if( $split_ok == 1 && $row2["duration_left"] != 0 && $row2["min_split_ok"] != 1 ) $split_condition_ok = false;
				if( $split_condition_ok ){
					//updating the task duration left
					$task_duration = $row2["duration_left"];
					
					//storing the schedule
					$obj = new api_schedule_details;
					$obj -> schedule_id = $schedule_id;
					$obj -> schedule_date = $start_date;
					$obj -> source_id = $task_id;
					$obj -> source_type = "Task";
					$obj -> time_from = $start_time;
					$obj -> time_to = $task_finish_time;
					$obj -> updateObject();
					
					//if break is compromised updating the break schedule
					if( $g_break_exception_id != 0 ){
						$obj = new api_schedule_details;
						$obj -> id = $g_break_exception_id;
						if( $g_break_exception_delete == 1 ) $obj -> active = 0;
						$obj -> time_from = $g_break_exception_time;
						$obj -> updateObject();
					}
				}
				
				//setting the new start time
				$start_time = $row2["new_start_time"];
				return false;
			}else{
				return true;
			}
		}else{
			return true;
		}
	}
	
	function isBlank($input){
		//check if the passed parameter is blank
		if($input == "") return true;
		else return false;
	}

	function checkNumber($input){
		//check if the passed parameter is numeric
		return is_numeric($input);
	}
	
	function checkEmail($input){
		//check if the passed parameter is valid email address
		if (!eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$", $input)){ 
		return false; 
		}else{
		return true;} 
	}

	function checkPhone($input){
		//check if the passed parameter is valid phone
		return true;
	}

	function checkValidDate($input){
		//check if the passed parameter is valid date
		global $conn;
		$date_sql = "select ifnull(datediff(curdate(), '$input'), 'invalid') as status";
		$dt_result=query_mysql($date_sql,$conn);
		$dt_row=fetch_mysql_data($dt_result);
		if($dt_row["status"] == "invalid") return false;
		else return true;
	}

	function showUserMessage($msg){
		echo "<script language='javascript'>";
		echo "alert('$msg')";
		echo "</script>";
	}
	function setSession($variable_name, $variable_value){
		//sets the value to the session variable
		//global $$variable_name;
		//$$variable_name = $variable_value;
		$_SESSION[$variable_name] = $variable_value;
	}

	function getSession($variable_name){
		//gets the value from session variable
		//global $$variable_name;
		//return $$variable_name;
		return $_SESSION[$variable_name];
	}

	function show_pagenation($total_records, $colspan, $curr_page, $url){
		global $cnfg_rpp;
		global $cnfg_pps;
		
		if($total_records > $cnfg_rpp){
		 if($curr_page == "") $curr_page = 1;
		 
		 //total pages
		 $total_pages = ceil($total_records / $cnfg_rpp);

		 //current section
		 $section = 1;
		 if($total_pages > $cnfg_pps) $section = ceil($curr_page / $cnfg_pps);

		 $limit_syntax = "limit " . ($cnfg_rpp * ($curr_page - 1)) . ", $cnfg_rpp";
?>
		 <tr class="odd">
		  <td colspan="<?echo $colspan;?>" class="paging">
<?
		 if($section != 1){
			$attach_url = str_replace('[page_param]', ((($section - 1)*$cnfg_pps)), $url);
?>
		  <a href="<? echo $attach_url;?>">&laquo;</a>&nbsp;
<?
		 }
		 for($page_counter=(($section - 1)*$cnfg_pps)+1; $page_counter <= ($section * $cnfg_pps); $page_counter++){
			if($page_counter <= $total_pages){
				if($page_counter != $curr_page){
					$attach_url = str_replace('[page_param]', $page_counter, $url);
?>
					<a href="<? echo $attach_url;?>">
<?
				}
				
				echo "<b>" . $page_counter . "</b>&nbsp;";

				if($page_counter != $curr_page){
				 echo "</a>";
				}
			}
		 }

		 if($total_pages >= $page_counter){
			$attach_url = str_replace('[page_param]', $page_counter, $url);
?>
		   <a href="<? echo $attach_url;?>">&raquo;</a>&nbsp;
<?
		 }
		 
		 if($total_pages > $curr_page){
			$next_page = $curr_page+1;
			$attach_url = str_replace('[page_param]', $next_page, $url);
?>
		    <a href="<? echo $attach_url;?>">next</a>
<?
		 }
?>
		 </td>
		</tr>
<?
		 return $limit_syntax;
		}
	}
	
	function show_drop_down($sql, $onchange, $name, $default, $show_none, $value_field, $caption_field){
		global $conn;
		$result_sdd = query_mysql($sql,$conn);	
		echo "<select name='$name' id='$name' onchange='$onchange' class='select-box'>";
		if($show_none != "") echo "<option value=''>$show_none</option>";
		if(get_total_rows($result_sdd) != 0){
		 while($row_sdd = fetch_mysql_data($result_sdd)){
			echo "<option value='" . $row_sdd[$value_field] . "'";
			if($row_sdd[$value_field] == $default) echo " selected";
			echo ">" . $row_sdd[$caption_field] . "</option>";
		 }
		}
		echo "</select>";
	}
	
	function populate_value($variable_name){
		//checks and returns value if specified variable exist as form or url variable
		$temp = get_form_values($variable_name);
		if($temp == "") $temp = get_url_values($variable_name);
		return $temp;
	}
	
	function upload_banner($expected_width, $expected_height){
		$file_type = $_FILES["advert"]['type'];
		$file_tmp = $_FILES["advert"]['tmp_name'];
		$file_size = $_FILES["advert"]['size'];
		$image_ok = true;
		
		//check if you have selected a file.
		if(is_uploaded_file($file_tmp)){
		
			//file's extension?
			$file_ext = getExtension($file_name);
			
			//keep image type
			if($file_size){
			
				if($file_type == "image/pjpeg" || $file_type == "image/jpeg"){
					$new_img = imagecreatefromjpeg($file_tmp);
				}elseif($file_type == "image/x-png" || $file_type == "image/png"){
					$new_img = imagecreatefrompng($file_tmp);
				}elseif($file_type == "image/gif"){
					$new_img = imagecreatefromgif($file_tmp);
				}
				
				//list the width and height and keep the height ratio.
				list($width, $height) = getimagesize($file_tmp);

				//if width and height = +-10 of the expectation its fine else cant upload
				if($width != $expected_width){
					if($width - $expected_width > 10) $image_ok = false;
					if($expected_width - $width > 10) $image_ok = false;
				}
				
				if($height != $expected_height){ 
					if($height - $expected_height > 10) $image_ok = false; 
					if($expected_height - $height > 10) $image_ok = false;
				}
				
				if($image_ok){
					return upload_file("advert", "advert_" . getSession("g_uid_advt"), "adverts_temp");
				}else{
					return false;
				}
			}
		}
	}
	
	function upload_file($control_name, $new_file_name, $file_path, $check_extension){
		$file_tmp = $_FILES[$control_name]['tmp_name'];
		$file_name = $_FILES[$control_name]['name'];
		if(is_uploaded_file($file_tmp)){
			$extension = getExtension($file_name);
			$upload = true;
			if($check_extension != ""){
				$extension_arr = split(",", $check_extension);
				$extension_match = false;
				for($counter=0;$counter<count($extension_arr);$counter++){
					if($extension_arr[$counter] == $extension){
						$extension_match = true;
					}
				}
			}
			
			if(!$extension_match){
				$upload = false;
				return "2";
			}
			
			if($upload){
				if(file_exists ( "$file_path/$new_file_name." . $extension)) unlink ( "$file_path/$new_file_name." . $extension);
				copy($file_tmp, "$file_path/$new_file_name." . $extension);
				return "$new_file_name.$extension";
			}
		}else{
			return "1";
		}
	}
	
	function generateUploadError($errorType){
		if($errorType == "1") return "No files found to upload";
		if($errorType == "2") return "Not valid extension";
	}
	
	function getExtension($file_name){
		$getExt = explode ('.', $file_name);
		return $getExt[count($getExt)-1];
	}
	
	function upload_file_resize($image_width, $control_name, $file_path, $new_file_name, $destroy){
		//the image -> variables
		$file_type = $_FILES[$control_name]['type'];
		$file_name = $_FILES[$control_name]['name'];
		$file_size = $_FILES[$control_name]['size'];
		$file_tmp = $_FILES[$control_name]['tmp_name'];

		//check if you have selected a file.
		if(is_uploaded_file($file_tmp)){
			//file's extension?
			$file_ext = getExtension($file_name);
			
			//keep image type
			if($file_size){
				if($file_type == "image/pjpeg" || $file_type == "image/jpeg"){
					$new_img = imagecreatefromjpeg($file_tmp);
				}elseif($file_type == "image/x-png" || $file_type == "image/png"){
					$new_img = imagecreatefrompng($file_tmp);
				}elseif($file_type == "image/gif"){
					$new_img = imagecreatefromgif($file_tmp);
				}
				
				//list the width and height and keep the height ratio.
				list($width, $height) = getimagesize($file_tmp);

				//calculate the image ratio
				$newwidth = $image_width;
				if($newwidth > $width) $newwidth = $width;
				$newheight = (100 / ($width / $newwidth)) * .01;
				$newheight = @round ($height * $newheight);
		
				//function for resize image.
				if (function_exists(imagecreatetruecolor)){
					$resized_img = imagecreatetruecolor($newwidth,$newheight);
				}else{
					die("Error: Please make sure you have GD library ver 2+");
				}
				
				//the resizing is going on here!
				ImageCopyResampled($resized_img, $new_img, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);

				//finally, save the image
				if(file_exists ( "$file_path/$new_file_name." . $file_ext)) unlink ( "$file_path/$new_file_name." . $file_ext);
				ImageJpeg ($resized_img,"$file_path/$new_file_name.$file_ext");
				ImageDestroy ($resized_img);
				if($destroy) ImageDestroy ($new_img);
				
				return "$new_file_name." . $file_ext;
			}
		}
	}
	
	function move_file($from, $to, $remove){
		if(file_exists ( $to )) unlink ( $to );
		if(file_exists ( $from )){ 
			copy($from, $to);
			if($remove) unlink($from);
		}
	}
	
	function SendEmail($to, $subject, $content, $from){
		//sending email
		$content = nl2br($content);
		$headers  = "From: $from\r\n"; 
		$headers .= "Content-type: text/html"; 
		mail($to, $subject, $content, "$headers");
	}
	
	function string2url($input){
		$input = str_replace("&", "-", $input);
		$input = str_replace("'", "_", $input);
		$input = str_replace(" ", "+", $input);
		$input = str_replace("/", "^", $input);
		return $input;
	}
	
	function url2string($input){
		$input = str_replace("-", "&", $input);
		$input = str_replace("_", "'", $input);
		$input = str_replace("+", " ", $input);
		$input = str_replace("^", "/", $input);
		return $input;
	}
	
	function getDuration($input){
		//get the duration from current time to supplied time input in format yyyy-mm-dd hh:mm:ss
		global $conn;
		$sql = "select Hour( timediff( now(), '$input' ) ) as total_hours";
		$result = query_mysql($sql,$conn);
		$row = fetch_mysql_data($result);
		$duration = $row["total_hours"];
		
		if($duration > 24){
			if($duration > (24*30)){
				return "more than 1 month";
			}else{
				if($duration > 48){
					return floor($duration / 24) . " days";
				}else{
					return "1 day";
				}
			}
		}else if($duration < 1){
			return "less than 1 hour";
		}else{
			if($duration >= 2){
				return $duration . " hours";
			}else{
				return "1 hour";
			}
		}
	}
	
	function MakeSessionId() {
		$day = date('d', time());
		$month = date('m', time());
		$year = date('Y', time());
		$hour = date('H', time());
		$min = date('i', time());
		$sec = date('s', time());

		return sprintf("%02d%04d%02d-%02d%02d%04d-%04d-%02d%04d", $sec, rand(0, 9999), $hour, $month, $min, rand(0, 9999), rand(0, 9999), $day, $year);
	}
	
	function isAdmin($redirect){
		if(getSession("g_member_type") != "admin"){
			if($redirect){
				header("Location: index.php?access_error=1");
				exit;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}
	
	function isStudent($redirect){
		if(getSession("g_member_type") != "student"){
			if($redirect){
				header("Location: index.php?access_error=1");
				exit;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}
	
	function isTutor($redirect){
		if(getSession("g_member_type") != "tutor"){
			if($redirect){
				header("Location: index.php?access_error=1");
				exit;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}
	
	function getFolderList($dir){
		//returns contents of $dir in a comma separated list
		$file_list = "";
		if (is_dir($dir)) {
			if ($dh = opendir($dir)) {
				while (($file = readdir($dh)) !== false) {
					if($file != "." && $file != ".." && $file != "index.php"){
						if($file_list == "") $file_list = $file;
						else $file_list .= "=next=" . $file;
					}
				}
				closedir($dh);
			}
		}
		return $file_list;
	}
	
	function getActivityType($type_id){
		return strtoupper(str_replace("_", " ", $type_id));
	}
	
	function showImage($videofile){
		/*
		$image = substr($videofile, 0, strlen($videofile) - 4);
		$time = "00:00:01";
		
		//header('Content-Type: image/png');  
		$myFile = "testFile.png";
		$fh = fopen($myFile, 'w') or die("can't open file");
		$stringData = exec("/usr/bin/ffmpeg -vcodec png -i \"" . $videofile . "\" -ss " . $time . " -vframes 1 -f image2 -");
		fwrite($fh, $stringData);
		fclose($fh);
		*/
		echo "<img src='images/video.jpg' title='video' alt='video' border='0'>";
	}
	
	function showCurvedContainer($content){
		echo $content;
	}
	
	function showCurvedContainer2($content){
?>
	<div class="t" style="width: 90%;height: 100%;">
		<div class="b" style="width: 100%;height: 100%;">
			<div class="l" style="width: 100%;height: 100%;">
				<div class="r" style="width: 100%;height: 100%;">
					<div class="bl" style="width: 100%;height: 100%;">
						<div class="br" style="width: 100%;height: 100%;">
							<div class="tl" style="width: 100%;height: 100%;">
								<div class="tr" style="width: 100%;height: 100%;">
									
										<?echo $content;?>
									
								</div>
							</div>
						</div>
					</div> 
				</div>
			</div>
		</div> 
	</div>
<?	
	}
	
	function logUserPOD($pod_id){
		global $conn;
		setSession("g_activity_id", "");
		$sql = "select pod_id from pod_user_log where pod_id = '" .  filter_value($pod_id) . "' and uid = " . getSession("g_uid");
		$result = query_mysql($sql,$conn);
		
		if(get_total_rows($result) == 0){
			$sql = "insert pod_user_log(uid, pod_id, date_started) values('" . getSession("g_uid") . "', '" .  filter_value($pod_id) . "', now())";
			query_mysql($sql,$conn);
		}
	}
	
	function updateActivityLog(){
		global $conn;
		$sql = "select id from activity_user_log where uid = " . getSession("g_uid") . " and activity_id = " . getSession("g_activity_id");
		$result = query_mysql($sql,$conn);
			
		if(get_total_rows($result) == 0){
			$sql = "insert activity_user_log(activity_id, uid, date_started) values(" . getSession("g_activity_id") . ", ";
			$sql .= getSession("g_uid") . ", now())";
			query_mysql($sql,$conn);
		}
	}
	
	function finishActivityLog(){
		global $conn;
		$sql = "select id from activity_user_log where uid = " . getSession("g_uid") . " and activity_id = " . getSession("g_activity_id");
		$result = query_mysql($sql,$conn);
			
		if(get_total_rows($result) != 0){
			$row = fetch_mysql_data($result);
			$sql = "update activity_user_log set date_completed = now() where id = " . $row["id"];
			query_mysql($sql,$conn);
			setSession("g_finish_type", "activity");
			
			//checking if all activity in the current pod is finished.
			$sql = "select pod_activities.id from pod_activities left outer join activity_user_log on pod_activities.id = ";
			$sql .= "activity_user_log.activity_id and uid = " . getSession("g_uid") . " and activity_user_log.date_completed != ";
			$sql .= "'0000-00-00 00:00:00' where pod_activities.active = 1 and pod_activities.pod_id = " . getSession("g_pod_id");
			$sql .= " and activity_user_log.id is null limit 1";
			$result = query_mysql($sql,$conn);
			
			if(get_total_rows($result) == 0){
				$sql = "update pod_user_log set date_completed = now() where uid = " . getSession("g_uid") . " and pod_id = ";
				$sql .= getSession("g_pod_id");
				query_mysql($sql,$conn);
				setSession("g_finish_type", "pod");
				
				//checking if all pod in the current module is finished.
				$sql = "select module_pods.id from module_pods left outer join pod_user_log on module_pods.pod_id = ";
				$sql .= "pod_user_log.pod_id and pod_user_log.uid = " . getSession("g_uid") . " and pod_user_log.date_completed != ";
				$sql .= "'0000-00-00 00:00:00' where module_pods.active = 1 and module_pods.module_id = " . getSession("g_module_id");
				$sql .= " and pod_user_log.id is null limit 1";
				$result = query_mysql($sql,$conn);
				
				if(get_total_rows($result) == 0){
					$sql = "update module_user_log set date_completed = now() where uid = " . getSession("g_uid") . " and module_id = ";
					$sql .= getSession("g_module_id");
					query_mysql($sql,$conn);
					setSession("g_finish_type", "module");
					
					//checking if all modules in the training is finished
					$sql = "select training_modules.id from training_modules left outer join module_user_log on training_modules.module_id = ";
					$sql .= "module_user_log.module_id and module_user_log.uid = " . getSession("g_uid") . " and module_user_log.date_completed != ";
					$sql .= "'0000-00-00 00:00:00' where training_modules.active = 1 and training_modules.training_id = " . getSession("g_training_id");
					$sql .= " and module_user_log.id is null limit 1";
					$result = query_mysql($sql,$conn);
					
					if(get_total_rows($result) == 0){
						$sql = "update training_user_log set date_completed = now() where uid = " . getSession("g_uid") . " and training_id = ";
						$sql .= getSession("g_training_id");
						query_mysql($sql,$conn);
						setSession("g_finish_type", "training");
					}
				}
			}
			
			setSession("g_activity_id", "");
			setSession("g_activity_type", "");
			setSession("g_activity_finished", "");
		}
	}
	
	function showModuleSelection(){
		global $conn;
		$module_shown = false;
		$shown_modules = "0";
?>
			<table width="100%"  border="0" cellpadding="2" cellspacing="1" class="tblDisplay">
				<?if(getSession("g_training_id") == ""){?>
				<tr>
				  <td class="section_head">
					<div align="center">&lt;Please select a training&gt;</div> 
				  </td>
				</tr>
				<?
					//showing training selection
					$sql = "select training.training_name, training.id, training_user_log.date_completed as finished_on, ";
					$sql .= "ifnull(training_user_log.id, 0) as log_id from training inner join training_users on training.id = training_users.training_id ";
					$sql .= "and training_users.active = 1 and training_users.uid = " . getSession("g_uid") . " left outer join training_user_log on ";
					$sql .= "training_users.uid = training_user_log.uid and training_users.training_id = training_user_log.training_id where ";
					$sql .= "training.active = 1 order by training.training_name";
					
					$result = query_mysql($sql,$conn);
					if(get_total_rows($result) != 0){
						$module_shown = true;
						while($row = fetch_mysql_data($result)){
				?>
						<tr class="odd">
							<td>
							<a href="#" onclick="ajaxGETRequest('ajax_handler_student.php?mode=linkTraining&training_id=<?echo $row["id"];?>', 1, reloadPage);"><?echo $row["training_name"];?></a>
							<?if($row["log_id"] != 0 && $row["finished_on"] != "0000-00-00 00:00:00") echo "&nbsp; <img src='images/tick.gif' title='Finished'>";?>
							</td>
						</tr>
				<?
						}
					}else{
				?>
					<tr class="noResult">
						<td>No trainings linked to your user</td>
					</tr>
				<?
					}
				?>
				<?}else{?>
				<tr>
				  <td colspan="3" class="section_head">
					<div align="center">&lt;Please select a module&gt;</div> 
				  </td>
				</tr>
				
				<?
					//showing modules from allowed training
					$sql = "select training_modules.training_id, modules.id, modules.module_name,  module_user_log.date_completed as finished_on, ";
					$sql .= "ifnull(module_user_log.id, 0) as log_id from training_modules inner join modules on training_modules.module_id = modules.id ";
					$sql .= "and modules.active = 1 left outer join module_user_log on ";
					$sql .= "modules.id = module_user_log.module_id and module_user_log.uid = ";
					$sql .= getSession("g_uid") . " where training_modules.active = 1 and training_modules.training_id = " . getSession("g_training_id");
					
					$result = query_mysql($sql,$conn);
					if(get_total_rows($result) != 0){
						while($row = fetch_mysql_data($result)){
							if($training_id != $row["training_id"]){
								if($training_id != ""){
									for($counter=$td_counter;$counter<3;$counter++){
										echo "<td>&nbsp;</td>";
									}
									echo "</tr>";
								}
								
								$training_id = $row["training_id"];
								$sql = "select training_name from training where id = $training_id";
								$result_title = query_mysql($sql,$conn);
								if(get_total_rows($result_title) != 0){
									$row_title = fetch_mysql_data($result_title);
									echo "<tr class='odd'><td colspan='3'><b>Training Name: " . $row_title["training_name"] . "</b>";
				?>					
									<a href='#' onclick="ajaxGETRequest('ajax_handler_student.php?mode=unlinkTraining', 1, reloadPage);">
				<?
									echo "<img src='images/switch.gif' title='Switch Training' border='0' alt='Switch'>";
									echo "</a>";
									echo "</td></tr>";
								}
								$td_counter = 0;
							}
							$shown_modules .= "," . $row["id"];
							if($td_counter == 0) echo "<tr class='odd'>";
							
							
				?>
					<td><a href="#" onclick="ajaxGETRequest('ajax_handler_student.php?mode=linkModule&module_id=<?echo $row["id"];?>', 1, refreshContents);Modalbox.hide();"><?echo $row["module_name"];?></a>
				<?
							if($row["log_id"] != 0 && $row["finished_on"] != "0000-00-00 00:00:00") echo "<img src='images/tick.gif' title='Finished'>";
							echo "</td>";
							$td_counter++;
							
							if($td_counter == 3){
								echo "</tr>";
								$td_counter = 0;
							}
						}
						
						if($training_id != ""){
							for($counter=$td_counter;$counter<3;$counter++){
								echo "<td>&nbsp;</td>";
							}
							echo "</tr>";
						}
					}else{
				?>
					<tr class="noResult">
						<td colspan="3">No modules linked to your user.</td>
					</tr>
				<?
					}
				}
				?>
			</table>
<?	
	}
	
	function log_last_access($access_id, $access_type){
		global $conn;
		$sql = "update users set $access_type = '" . $access_id . "' where uid = '" . getSession("g_uid") . "'";
		query_mysql($sql,$conn);
	}
	
	function getSegment($urgent, $important){
		if($urgent == 1 && $important == 1) return "Segment 1";
		if($urgent == 1 && $important == 0) return "Segment 2";
		if($urgent == 0 && $important == 1) return "Segment 3";
		if($urgent == 0 && $important == 0) return "Segment 4";
	}
	
	function get_content($url){
		$ch = curl_init();

		curl_setopt ($ch, CURLOPT_URL, $url);
		curl_setopt ($ch, CURLOPT_HEADER, 0);

		ob_start();

		curl_exec ($ch);
		curl_close ($ch);
		$string = ob_get_contents();

		ob_end_clean();

		return $string;     
	}
	
	function put_task_on_hold($task_id, $hold_date){
		$obj = new api_tasks;
		$obj -> task_id = $task_id;
		$obj -> created_by = getSession('g_uid');
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			if( $hold_date != "" ){
				$date_arr = getSplitArray( $hold_date, "-");
				$date_value = $date_arr[2] . "-" . $date_arr[1] . "-" . $date_arr[0];
				$obj -> on_hold = $date_value;
			}
			$obj -> updateObject();
		}
	}
	
	function show_hold_option(){
		global $mode;
		global $sub_mode;
?>
	<form method="post" action="index.php?call_mode=hold" name="editform">
	<span id="onhold_params" style="display:none;">
		<input type="hidden" name="task_id" id="task_id">
		<input type="hidden" name="mode" id="mode" value="<?echo $mode;?>">
		<input type="hidden" name="sub_mode" id="mode" value="<?echo $sub_mode;?>">
		<?if( get_url_values("segment") != "" ){?>
			<input type="hidden" name="segment" id="segment" value="<?echo get_url_values("segment");?>">
		<?}?>
		<?if( get_url_values("template") != "" ){?>
			<input type="hidden" name="template" id="template" value="<?echo get_url_values("template");?>">
		<?}?>
		<table class="tblEdit" cellspacing="0" width="300px" border="0">
			<tr>
				<td class="frm_caption">Hold Untill:</td>
				<td class="frm_input" colspan="3">
					<?create_date_dropdown("hold_date", '')?>
				</td>
			</tr>
			<tr>
				<td class="frm_input" colspan="4">
					<input type="submit" class="button" value="Put on hold">
					<input type="button" class="button" value="Cancel" onclick="$('onhold_params').hide();">
				</td>
			</tr>
		</table>
	</span>
	</form>
<?
	}
?>
