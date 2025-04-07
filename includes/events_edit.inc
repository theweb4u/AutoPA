<?
	$call_mode = get_form_values("call_mode");
	if($call_mode == "") $call_mode = get_url_values("call_mode");
	$event_id = get_form_values("event_id");
	if($event_id == "") $event_id = get_url_values("event_id");
	
	if($call_mode == "save"){
		$title = get_form_values("title");
		$description = get_form_values("description");
		$event_date = get_form_values("event_date");
		$destination = get_form_values("destination");
		$time_from = get_form_values("time_from");
		$time_to = get_form_values("time_to");
	}
	
	if($call_mode == "save"){
		//validation - title, category and time required is compulsory
		if($title == ""){
			$error = "1";
			$task_error = "<font color='red'>*</font>";
		}
		
		if($error == ""){
			$obj = new api_events;
			if($event_id != ""){
				//update event
				$obj -> event_id = $event_id;
			}
			$obj -> title = $title;
			$obj -> description = $description;
			if( $event_date != "" ){
				$date_arr = getSplitArray( get_form_values("event_date"), " ");
				$date_arr2 = getSplitArray( $date_arr[0], "-");
				$date_value = $date_arr2[2] . "-" . $date_arr2[1] . "-" . $date_arr2[0] . " " . $date_arr[1] . ":00";
				$obj -> event_date = $date_value;
			}
			$obj -> destination = $destination;
			$obj -> time_from = $time_from;
			$obj -> time_to = $time_to;
			$obj -> updateObject();
?>
			<script language="javascript">//alert("Event saved successfully");</script>
<?			
			$title = "";
			$description = "";
			$event_date = "";
			$destination = "";
			$time_from = "";
			$time_to = "";
		}
	}
	
	if($call_mode == "edit" && $event_id != ""){
		//getting existing data
		$obj = new api_events;
		$obj -> event_id = $event_id;
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			$row = fetch_mysql_data($result);
			$title = $row["title"];
			$description = $row["description"];
			$event_date = $row["event_date_formatted2"];
			$destination = $row["destination"];
			$time_from = $row["time_from_formatted"];
			$time_to = $row["time_to_formatted"];
		}
	}
?>
<form method="post" action="index.php" name="editform">
	<input type="hidden" name="call_mode" id="call_mode" value="">
	<input type="hidden" name="event_id" value="<?echo $event_id;?>">
	<input type="hidden" name="mode" value="events">
	<input type="hidden" name="sub_mode" value="event_edit">
	
	<table class="tblEdit" width="700px" border="0">
		<tr>
			<td colspan="4" class="editHeader">Event Mantainence<br><br></td>
		</tr>
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
			</td>
		</tr>
		<tr>
			<td class="frm_caption">Description:</td>
			<td class="frm_input" colspan="3">
				<textarea cols="65" rows="3" name="description" class="textarea"><?echo $description;?></textarea>
			</td>
		</tr>
		<tr>
			<td class="frm_caption">Event Date:</td>
			<td class="frm_input">
				<?create_date_dropdown("event_date", $event_date)?>
			</td>
			<td class="frm_caption">Location:</td>
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
			<td>Time From:</td>
			<td>
				<?
					create_time_dropdown("time_from", $time_from)
				?>
			</td>
			<td>Time To:</td>
			<td>
				<?
					create_time_dropdown("time_to", $time_to)
				?>
			</td>
		</tr>
		<tr>
			<td class="frm_input" colspan="4">
				<input type="submit" class="button" value="Save Event" onclick="$('call_mode').value = 'save';">
				<input type="button" class="button" value="Cancel" onclick="location.href='index.php?mode=events';">
			</td>
		</tr>
	</table>
</form>
