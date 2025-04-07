<?
	$call_mode = get_form_values("call_mode");
	if($call_mode == "") $call_mode = get_url_values("call_mode");
	$task_id = get_form_values("task_id");
	if($task_id == "") $task_id = get_url_values("task_id");
	
	if($call_mode == "from_view"){
		$important = get_url_values("important");
		$urgent = get_url_values("urgent");
	}
	
	if($call_mode == "save" || $call_mode == "category"){
		$parent_id = get_form_values("parent_id");
		$sub_category = get_form_values("sub_category");
		$title = get_form_values("title");
		$description = get_form_values("description");
		$important = get_form_values("important");
		$urgent = get_form_values("urgent");
		$hour = get_form_values("hour");
		$minute = get_form_values("minute");
		$time_split = get_form_values("time_split");
		$minimum = get_form_values("minimum");
		$break = get_form_values("break");
		$filing = get_form_values("filing");
		$due_date = get_form_values("due_date");
		$specific_time = get_form_values("specific_time");
		$specific_time_start = get_form_values("specific_time_start");
		$specific_time_finish = get_form_values("specific_time_finish");
		$productivity = get_form_values("productivity");
		$destination = get_form_values("destination");
		$is_template = get_form_values("is_template");
		$more_option = get_form_values("more_option");
		$after_hours = get_form_values("after_hours");
	}
	
	if($call_mode == "save"){
		//validation - title, category and time required is compulsory
		if($title == ""){
			$error = "1";
			$task_error = "<font color='red'>*</font>";
		}
		if($parent_id == ""){
			$error = "1";
			$parent_id_error = "<font color='red'>*</font>";
		}
		if($hour == "" && $minute == ""){
			$hour_error = "<font color='red'>*</font>";
			$minute_error = "<font color='red'>*</font>";
			$error = "1";
		}else{
			if($hour == "") $hour = 0;
			if($minute == "") $minute = 0;
			if( !checkNumber($hour) ){
				$hour_error = "<font color='red'>*</font>";
				$error = "1";
			}
			if( !checkNumber($minute) ){
				$minute_error = "<font color='red'>*</font>";
				$error = "1";
			}
		}
			
		if($error == ""){
			$obj = new api_tasks;
			if($task_id != ""){
				//update task
				$obj -> task_id = $task_id;
			}
			$obj -> category_id = $parent_id;
			$obj -> sub_category_id = $sub_category;
			$obj -> title = $title;
			$obj -> description = $description;
			$obj -> importance = ($important == ""? 0:$important);
			$obj -> urgency = ($urgent == ""? 0:$urgent);
			$result = query_mysql("select ('$hour' * 60) + '$minute' as time_allocated", $conn);
			$row = fetch_mysql_data($result);
			$obj -> time_allocated = $row["time_allocated"];
			$obj -> time_split = $time_split;
			$obj -> time_split_min = $minimum;
			$obj -> break_time = $break;
			$obj -> filling = $filing;
			if( $due_date != "" ){
				$date_arr = getSplitArray( get_form_values("due_date"), " ");
				$date_arr2 = getSplitArray( $date_arr[0], "-");
				$date_value = $date_arr2[2] . "-" . $date_arr2[1] . "-" . $date_arr2[0] . " " . $date_arr[1] . ":00";
				$obj -> due_date = $date_value;
			}
			$obj -> specific_time = $specific_time;
			$obj -> repeat_start = $specific_time_start;
			$obj -> repeat_finish = $specific_time_finish;
			$obj -> productive = $productivity;
			$obj -> destination = $destination;
			$obj -> is_template = $is_template;
			$obj -> after_hours = $after_hours;
			$effected_task_id = $obj -> updateObject();
			
			if( $cnfg_urgent_important_measured_by == "numbers" ){
				//updating the x and y coordinates
				$obj -> task_id = $effected_task_id;
				$obj -> updatePosition();
			}
			
			if($task_id != ""){
?>
			<script language="javascript">
				<?if($template == "no"){?>
				window.opener.location.reload();
				window.close();
				<?}else{?>
				//alert("Task saved successfully");
				location.href="index.php?mode=tasks";
				<?}?>
			</script>
<?			
			}else{
?>
			<script language="javascript">
				<?if($template == "no"){?>
				window.opener.location.reload();
				window.close();
				<?}else{?>
				//alert("Task saved successfully");
				<?}?>
			</script>
<?			
			}
			$parent_id = "";
			$sub_category = "";
			$title = "";
			$description = "";
			$important = "";
			$urgent = "";
			$hour = "";
			$minute = "";
			$time_split = "";
			$minimum = "";
			$break = "";
			$filing = "";
			$due_date = "";
			$specific_time = "0";
			$specific_time_start = "";
			$specific_time_finish = "";
			$productivity = "";
			$destination = "";
			$is_template = "";
			$more_option = "";
			$after_hours = "";
			$task_id = "";
		}
	}
	
	if($call_mode == "edit" && $task_id != ""){
		//getting existing data
		$obj = new api_tasks;
		$obj -> task_id = $task_id;
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			$row = fetch_mysql_data($result);
			$parent_id = $row["category_id"];
			$sub_category = $row["sub_category_id"];
			$title = $row["title"];
			$description = $row["description"];
			$important = $row["importance"];
			$urgent = $row["urgency"];
			$hour = floor( $row["time_allocated"] / 60 );
			$minute = $row["time_allocated"] % 60;
			$time_split = $row["time_split"];
			$minimum = $row["time_split_min"];
			$break = $row["break_time"];
			$filing = $row["filling"];
			$due_date = $row["due_date_formatted2"];
			$specific_time = $row["specific_time"];
			$specific_time_start = $row["repeat_start"];
			$specific_time_finish = $row["repeat_finish"];
			$productivity = $row["productive"];
			$destination = $row["destination"];
			$is_template = $row["is_template"];
			$after_hours = $row["after_hours"];
		}
	}
?>
<form method="post" action="index.php" name="editform">
	<input type="hidden" name="call_mode" id="call_mode" value="">
	<input type="hidden" name="more_option" id="more_option" value="<?echo $more_option;?>">
	<input type="hidden" name="task_id" value="<?echo $task_id;?>">
	<input type="hidden" name="mode" value="tasks">
	<input type="hidden" name="sub_mode" value="task_edit">
	<input type="hidden" name="template" value="<?echo $template;?>">
	
	<table class="tblEdit" width="700px" border="0">
		<tr>
			<td colspan="4" class="editHeader">Task Mantainence<br><br></td>
		</tr>
		<?if( $is_template == "1"){?>
		<tr>
			<td colspan="4" align="center"><b><font size="+1">You are editing a template</font></b></td>
		</tr>
		<?}?>
		<?if($error == "1"){?>
		<tr>
			<td colspan="4" align="center"><b><font color="red">Check the fields marked with *</font></b></td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_caption">Title:</td>
			<td class="frm_input" colspan="3">
				<input type="text" name="title" value="<?echo $title?>" size="100" class="textbox">
				<?echo $task_error;?>
				<input type="checkbox" name="is_template" id="is_template" value="1" <?if($is_template == "1") echo "checked";?>> Is Template
			</td>
		</tr>
		<tr>
			<td class="frm_caption">Description:</td>
			<td class="frm_input" colspan="3">
				<textarea cols="65" rows="3" name="description" class="textarea"><?echo $description;?></textarea>
			</td>
		</tr>
		<tr>
			<td class="frm_caption">Category Name:</td>
			<td class="frm_input">
				<select name="parent_id" class="select-box" onchange="$('call_mode').value='category';document.editform.submit();">
					<option value="">Select a category</option>
					<?
						$obj = new api_types;
						$obj -> created_by = getSession('g_uid');
						$obj -> order_by = "priority, title";
						$obj -> parent_id = 0;
						$obj -> type = "category";
						$result = $obj -> getDetail();
						while($row = fetch_mysql_data($result)){
							echo "<option value='" . $row["category_id"] . "' ";
							if( $parent_id == $row["category_id"] ) echo "selected";
							echo ">" . $row["title"] . "</option>";
						}
					?>
				</select>
				<?echo $parent_id_error;?>
			</td>
			<td class="frm_caption">Sub Category Name:</td>
			<td class="frm_input">
				<select name="sub_category" class="select-box">
					<?
						if($parent_id == "" || $parent_id == 0){
							echo "<option value=''>Please select a top category</option>";
						}else{
							echo "<option value=''>Select Sub Category</option>";
							$obj = new api_types;
							$obj -> created_by = getSession('g_uid');
							$obj -> order_by = "priority, title";
							$obj -> parent_id = $parent_id;
							$obj -> type = "category";
							$result = $obj -> getDetail();
							while($row = fetch_mysql_data($result)){
								echo "<option value='" . $row["category_id"] . "' ";
								if( $sub_category == $row["category_id"] ) echo "selected";
								echo ">" . $row["title"] . "</option>";
							}
						}
					?>
				</select>
			</td>
		</tr>
		<?if( $cnfg_urgent_important_measured_by == "numbers" ){?>
		<tr>
			<td class="frm_caption">Urgent:</td>
			<td class="frm_input">
				<select name="important" id="important">
					<option value="">Select</option>
					<?
					for( $counter=1;$counter<11;$counter++ ){
						echo "<option value='$counter'";
						if( $counter == $important ) echo " selected";
						echo ">$counter</option>";
					}
					?>
				</select>
			</td>
			<td class="frm_caption">Important:</td>
			<td class="frm_input">
				<select name="urgent" id="urgent">
					<option value="">Select</option>
					<?
					for( $counter=1;$counter<11;$counter++ ){
						echo "<option value='$counter'";
						if( $counter == $urgent ) echo " selected";
						echo ">$counter</option>";
					}
					?>
				</select>
			</td>
		</tr>
		<?}else{?>
		<tr>
			<td class="frm_caption">Important:</td>
			<td class="frm_input">
				<input class="checkbox" type="checkbox" name="important" id="important" value="1" <?if($important == "1") echo "checked"?>>
			</td>
			<td class="frm_caption">Urgent:</td>
			<td class="frm_input">
				<input class="checkbox" type="checkbox" name="urgent" id="urgent" value="1" <?if($urgent == "1") echo "checked"?>>
			</td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_caption">Time Required:</td>
			<td colspan="2">
				<input type="text" name="hour" value="<?echo $hour?>" size="10" class="textboxR"> hr.
				<?echo $hour_error;?>
				<input type="text" name="minute" value="<?echo $minute?>" size="10" class="textboxR"> min.
				<?echo $minute_error;?>
			</td>
			<td class="frm_caption" style="padding:8px;"><a href="#" onclick="moreOptions();">More Options 
			<span id="span_option"><img src="images/arrowup.gif" title="Show More Options" border="0"></span></a>
			</td>
		</tr>
		<tr style="display:none;" id="row1">
			<td class="frm_caption">Time Split:</td>
			<td class="frm_input">
				<select name="time_split" id="time_split" class="select-box" onchange="show_min();">
					<option value="1" <?if($time_split == "1") echo "selected"?>>Yes</option>
					<option value="0" <?if($time_split == "No") echo "selected"?>>No</option>
				</select>
				<span id="span_min">
				Minimum
				<input type="Text" name="minimum" id="minimum" size="5" value="<?echo $minimum;?>"> min.
				</span>
				<script language="javascript">
					function show_min(){
						if( $("time_split").value == "1" ) $("span_min").show();
						else $("span_min").hide();
					}
					show_min();
				</script>
			</td>
			<td class="frm_caption">Break:</td>
			<td class="frm_input">
				<select name="break" id="break" class="select-box">
					<option value="1" <?if($break == "1") echo "selected"?>>Yes</option>
					<option value="0" <?if($break == "0") echo "selected"?>>No</option>
				</select>
			</td>
		</tr>
		<tr style="display:none;" id="row2">
			<td class="frm_caption">Filing:</td>
			<td class="frm_input">
				<input type="text" name="filing" value="<?echo $filing?>" size="65" class="textbox">
			</td>
			<td class="frm_caption">After Hours:</td>
			<td class="frm_input">
				<select name="after_hours" id="after_hours" class="select-box">
					<option value="0" <?if($after_hours == "0") echo "selected"?>>No</option>
					<option value="1" <?if($after_hours == "1") echo "selected"?>>Yes</option>
				</select>
			</td>
		</tr>
		<tr style="display:none;" id="row3">
			<td class="frm_caption">Due Date:</td>
			<td class="frm_input" colspan="3">
				<?create_datetime_dropdown("due_date", $due_date)?>
			</td>
		</tr>
		<tr style="display:none;" id="row4">
			<td class="frm_caption">Specific Time:</td>
			<td class="frm_input" colspan="3">
				<select name="specific_time" id="specific_time" class="select-box" onchange="show_specific_time();">
					<option value="" <?if($specific_time == "") echo "selected"?>>N/A</option>
					<option value="repeat" <?if($specific_time == "repeat") echo "selected"?>>Repeat</option>
				</select>
			</td>
		</tr>
		<tr style="display:none;" id="row5">
			<td class="frm_caption">Start:</td>
			<td class="frm_input" colspan="3">
				<?create_time_dropdown("specific_time_start", $specific_time_start)?>
			</td>
		</tr>
		<tr style="display:none;" id="row6">
			<td class="frm_caption">Finish:</td>
			<td class="frm_input" colspan="3">
				<?create_time_dropdown("specific_time_finish", $specific_time_finish)?>
				<script language="javascript">
					function show_specific_time(){
						if( $("row4").style.display != "none" && $("specific_time").value == "repeat" ){
							$("row5").show();
							$("row6").show();
						}else{
							$("row5").hide();
							$("row6").hide();
						}
					}
					show_specific_time();
				</script>
			</td>
		</tr>
		<tr style="display:none;" id="row7">
			<td class="frm_caption">Productivity:</td>
			<td class="frm_input">
				<select name="productivity" class="select-box">
					<option value="Billable" <?if($productivity == "Billable") echo "selected"?>>Billable</option>
					<option value="Productive" <?if($productivity == "Productive") echo "selected"?>>Productive</option>
					<option value="Non Productive" <?if($productivity == "Non Productive") echo "selected"?>>Non Productive</option>
					<option value="Personal" <?if($productivity == "Personal") echo "selected"?>>Personal</option>
					<option value="" <?if($productivity == "") echo "selected"?>>N/A</option>
				</select>
			</td>
			<td class="frm_caption">Destination:</td>
			<td class="frm_input">
				<select name="destination" class="select-box">
					<option value="">N/A</option>
				<?
					$obj = new api_types;
					$obj -> created_by = getSession('g_uid');
					$obj -> order_by = "title";
					$obj -> type = "location";
					$result = $obj -> getDetail();
					
					if( get_total_rows($result) != 0){
						while($row = fetch_mysql_data($result)){
				?>
						<option value="<?echo $row["category_id"];?>" <?if($destination == $row["category_id"]) echo "selected"?>><?echo $row["title"];?></option>"
				<?
						}
					}
				?>
				</select>
			</td>
		</tr>
		<tr>
			<td class="frm_input" colspan="4">
				<input type="submit" class="button" value="Save Task" onclick="$('call_mode').value = 'save';">
				<input type="button" class="button" value="Cancel" onclick="<?if($template == "no"){?>window.close();<?}else{?>location.href='index.php?mode=tasks';<?}?>">
			</td>
		</tr>
	</table>
</form>

<script language="javascript">
	function moreOptions(){
		for(var counter=1;counter<8;counter++){
			if( counter != 4) $("row" + counter).toggle();
		}
		if($("row1").style.display == "none"){
			$("span_option").innerHTML = "<img src=\"images/arrowup.gif\" title=\"Show More Options\" border=\"0\">";
			$("more_option").value = "";
		}else{
			$("span_option").innerHTML = "<img src=\"images/arrowdown.gif\" title=\"Hide Options\" border=\"0\">";
			$("more_option").value = "1";
		}
		show_specific_time();
	}
	
	if( $("more_option").value == "1" ) moreOptions();
</script>
