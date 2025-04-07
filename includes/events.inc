<?
	//including api file
	include("api/types.inc");
	include("api/events.inc");
	
	$include_found = false;
	
	if($sub_mode == "event_edit"){
		if (file_exists("includes/events_edit.inc")) include("includes/events_edit.inc");
		$include_found = true;
	}
	
	if($sub_mode == "events_google"){
		if (file_exists("includes/events_google.inc")) include("includes/events_google.inc");
		$include_found = true;
	}
	
	if($sub_mode == "") $include_found = true;
	
	if(!$include_found){
		echo "<p align='center' class='error'>Unexpected Request!!</p>";
	}
	
	if($sub_mode == ""){
		$call_mode = get_url_values("call_mode");
		if($call_mode == "delete"){
			$obj = new api_events;
			$obj -> event_id = filter_value( get_url_values("event_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$obj -> active = 0;
				$obj -> updateObject();
			}else{
				$error = "Requested event not available for editing";
			}
		}
		
		if($call_mode == "delete_bulk_events"){
			$obj = new api_events;
			$obj -> custom_query = " event_date < curdate() or (event_date = curdate() and time_to < curtime()) ";
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$obj2 = new api_events;
				while($row = fetch_mysql_data($result)){
					$obj2 -> event_id = $row["event_id"];
					$obj2 -> active = 0;
					$obj2 -> updateObject();
				}	
			}else{
				$error = "Requested event not available for editing";
			}
		}
		
		if($call_mode == "update_status"){
			$obj = new api_events;
			$obj -> event_id = filter_value( get_url_values("event_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				if( $row["status"] == "current" ) $obj -> status = "complete";
				else $obj -> status = "current";
				$obj -> updateObject();
			}else{
				$error = "Requested event not available for editing";
			}
		}
		
		$obj = new api_events;
		$obj -> created_by = getSession('g_uid');
		$obj -> active = 1;
		$obj -> order_by = "created desc, title";
		$result = $obj -> getDetail();
?>
	<table class="tblDisplay" width="800px">
		<tr class="header">
			<td class="action">@</td>
			<td class="action">Created</td>
			<td>Event</td>
			<td class="action">Date Due</td>
			<td class="action">Time From</td>
			<td class="action">Time To</td>
			<td class="action">@</td>
			<td class="action">
				<a href="index.php?mode=events&sub_mode=event_edit">
					<img src="images/add.gif" title="Add Events" border="0"></a>
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
			<td>
				<a href="index.php?mode=events&sub_mode=event_edit&event_id=<?echo $row["event_id"];?>&call_mode=edit">
					<img src="images/edit.gif" title="Edit Events" border="0"></a>
			</td>
			<td><?echo $row["created_formatted"];?></td>
			<td><?echo $row["title"];?></td>
			<td><?echo $row["event_date_formatted"];?></td>
			<td nowrap><?echo $row["time_from_formatted"];?></td>
			<td nowrap><?echo $row["time_to_formatted"];?></td>
			<td class="action">
				<a href="index.php?mode=events&event_id=<?echo $row["event_id"];?>&call_mode=update_status">
					<?if( $row["status"] == "current" ){?>
					<img src="images/active.gif" title="Status: Current / Click to complete" border="0">
					<?}else{?>
					<img src="images/inactive.gif" title="Status: Complete / Click to make current" border="0">
					<?}?>
				</a>
			</td>
			<td>
				<a href="index.php?mode=events&event_id=<?echo $row["event_id"];?>&call_mode=delete" onclick="return confirm('Are you sure to delete this Event?');">
					<img src="images/trash.gif" title="Delete Events" border="0"></a>
			</td>
		</tr>
		<?
			}
		}else{
		?>
		<tr class="noResult">
			<td colspan="8">No Events found to display</td>
		</tr>
		<?}?>
	</table>
	<?if($error != ""){?>
	<script language="javascript">alert("<?echo $error;?>");</script>
	<?}?>
<?
	}
?>