<?
	$obj = new api_schedule_master;
	$obj -> id = filter_value( get_url_values("schedule_id") );
	$obj -> created_by = getSession('g_uid');
	$result = $obj -> getDetail();
	if( get_total_rows($result) == 0){
?>
	<table class="tblDisplay" width="100%">
		<tr class="noResult">
			<td>Requested schedule not available for viewing</td>
		</tr>
	</table>
<?
	}else{
?>
<table class="tblDisplay" width="100%">
<?
	if( get_url_values("show_status") == 1 ){
		$row = fetch_mysql_data($result)
?>
	<tr class="odd">
		<td><?echo $row["status"]?></td>
	</tr>
<?	
	}else{
?>	
	<tr class="header">
		<td>Time From</td>
		<td>Time To</td>
		<td>Task/Event</td>
		<td>Details</td>
	</tr>
<?
	
	$obj = new api_schedule_details;
	$obj -> schedule_id = get_url_values("schedule_id");
	$result = $obj -> getDetail();
	$colspan = "4";
	$day_counter = 0;
	if( get_total_rows($result) != 0){
		$class = "even";
		while($row = fetch_mysql_data($result)){	
			if($class = "even") $class = "odd";
			else $class = "even";
?>
		<tr class="<?echo $class;?>" valign="top">
<?
			if( $row["source_type"] == "HOLIDAY" ){
?>
			<td colspan="<?echo $colspan;?>" style="text-align: center;font-weight: bold;color: green;">HOLIDAY (<?echo $row["schedule_date_formatted"];?>)</td>
<?
			}else{
				if( $row["schedule_date_formatted"] != $current_date ){
					$current_date = $row["schedule_date_formatted"];
					$current_dayname = $row["schedule_date_day_name"];
					$day_counter++;
?>
			<td colspan="<?echo $colspan;?>" style="font-weight: bold;color: red;">Day <?echo $day_counter . ": " . $current_date . " (" . $current_dayname . ")";?></td>
		</tr>
		<tr class="<?echo $class;?>" valign="top">	
<?
				}
?>
			<td><?echo $row["time_from"];?></td>
			<td><?echo $row["time_to"];?></td>
			<td nowrap <?if( $row["source_id"] == "0" ) echo " style='color: orange;font-weight: bold;'";?>><?echo $row["source_type"];?></td>
			<td>
<?
				if( $row["source_id"] != "0" ){
					if( $row["source_type"] == "Event" ){
						$obj = new api_events;
						$obj -> event_id = $row["source_id"];
					}else{
						$obj = new api_tasks;
						$obj -> task_id = $row["source_id"];
					}
					$result2 = $obj -> getDetail();
					$row2 = fetch_mysql_data($result2);
					echo $row["source_id"] . "-" . $row2["title"] . "<br>" . $row2["description"];
				}else{
					echo "N/A";
				}
		?>					
			</td>
<?
			}
?>
		</tr>
<?				
		}
	}else{
?>
	<tr class="noResult">
		<td colspan="<?echo $colspan;?>">No detail found to display</td>
	</tr>
<?		
	}}}	
?>
</table>