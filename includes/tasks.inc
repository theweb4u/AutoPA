<?
	//including api file
	include("api/types.inc");
	include("api/tasks.inc");
	
	$include_found = false;
	
	if($sub_mode == "task_edit"){
		if (file_exists("includes/task_edit.inc")) include("includes/task_edit.inc");
		$include_found = true;
	}
	
	if($sub_mode == "task_from_template"){
		if (file_exists("includes/task_from_template.inc")) include("includes/task_from_template.inc");
		$include_found = true;
	}
	
	if($sub_mode == "") $include_found = true;
	
	if(!$include_found){
		echo "<p align='center' class='error'>Unexpected Request!!</p>";
	}
	
	if($sub_mode == ""){
		$call_mode = get_url_values("call_mode");
		if($call_mode == "delete"){
			$obj = new api_tasks;
			$obj -> task_id = filter_value( get_url_values("task_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$obj -> active = 0;
				$obj -> updateObject();
			}else{
				$error = "Requested task not available for editing";
			}
		}
		
		if($call_mode == "hold"){
			$obj = new api_tasks;
			$obj -> task_id = filter_value( get_form_values("task_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				if( get_form_values("hold_date") != "" ){
					$date_arr = getSplitArray( get_form_values("hold_date"), "-");
					$date_value = $date_arr[2] . "-" . $date_arr[1] . "-" . $date_arr[0];
					$obj -> on_hold = $date_value;
				}
				$obj -> updateObject();
			}else{
				$error = "Requested task not available for editing";
			}
		}
		
		if($call_mode == "unhold"){
			$obj = new api_tasks;
			$obj -> task_id = filter_value( get_url_values("task_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$obj -> on_hold = 'null';
				$obj -> updateObject();
			}else{
				$error = "Requested task not available for editing";
			}
		}
		
		if($call_mode == "update_status"){
			$obj = new api_tasks;
			$obj -> task_id = filter_value( get_url_values("task_id") );
			$obj -> created_by = getSession('g_uid');
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				if( $row["status"] == "current" ) $obj -> status = "complete";
				else $obj -> status = "current";
				$obj -> updateObject();
			}else{
				$error = "Requested task not available for editing";
			}
			
			$ajaxcall = get_url_values("ajaxcall");
			if( $ajaxcall == "1" ){
				echo $error;
				exit;
			}
		}
		
		$filter = get_form_values("filter");
		$obj = new api_tasks;
		$obj -> created_by = getSession('g_uid');
		$obj -> active = 1;
		
		if( $filter == "1" ){
			$order_by = get_form_values("order_by");
			$txttitle = get_form_values("txttitle");
			$task_type = get_form_values("task_type");
			$sort_as = get_form_values("sort_as");
			if( $txttitle != "" ) $obj -> title = $txttitle;
			if( $task_type != "" ) $obj -> task_type = $task_type;
		}else{
			$order_by = "created";
			$sort_as = "desc";
		}
		
		$obj -> order_by = "$order_by $sort_as, title";
		$result = $obj -> getDetail();
?>
	<table class="tblDisplay" width="800px">
		<tr>
			<td colspan="10" style="text-align: left;">
			<a href="#" onclick="$('span_filter').toggle();">Click to view Filters:</a><br>
			<span id="span_filter" style="display:none;">
				<form name="frmFilter" method="post" action="index.php?mode=tasks">
					<input type="hidden" name="filter" value="1">
					<table width="100%">
						<tr>
							<td><b>Title: </b></td>
							<td><input type="text" name="txttitle" size="65" value="<?echo $txttitle;?>"></td>
						</tr>
						<tr>
							<td><b>Other: </b></td>
							<td>
								<select name="task_type" id="task_type">
									<option value="">Select</option>
									<option value="completed" <?if($task_type == "completed") echo "selected";?>>Show Completed</option>
									<option value="not_completed" <?if($task_type == "not_completed") echo "selected";?>>Show Active Tasks</option>
									<option value="on_hold" <?if($task_type == "on_hold") echo "selected";?>>Show On Hold</option>
								</select>
							</td>
						</tr>
						<tr>
							<td><b>Order by: </b></td>
							<td>
							<select name="order_by">
								<option value="created" <?if( $order_by == "created" ) echo "selected";?>>Created</option>
								<option value="title" <?if( $order_by == "title" ) echo "selected";?>>Title</option>
								<option value="category_name" <?if( $order_by == "category_name" ) echo "selected";?>>Category</option>
								<option value="sub_category_name" <?if( $order_by == "sub_category_name" ) echo "selected";?>>Sub Category</option>
								<option value="due_date" <?if( $order_by == "due_date" ) echo "selected";?>>Date Due</option>
							</select>
							</td>
						</tr>
						<tr>
							<td><b>Sort As: </b></td>
							<td>
							<select name="sort_as">
								<option value="desc" <?if( $sort_as == "desc" ) echo "selected";?>>Desc</option>
								<option value="asc" <?if( $sort_as == "asc" ) echo "selected";?>>Asc</option>
							</select>
							</td>
						</tr>
						<tr>
							<td colspan="2"><input type="submit" value="Apply Filters"></td>
						</tr>
					</table>
				</form>
			</span>
			</td>
		</tr>
		<tr class="header">
			<td class="action">@</td>
			<td class="action">Task#</td>
			<td class="action">Created</td>
			<td>Title</td>
			<td class="action">Category</td>
			<td class="action">Sub Category</td>
			<td class="action">Date Due</td>
			<td class="action">Segment</td>
			<td class="action">@</td>
			<td class="action">
				<a href="index.php?mode=tasks&sub_mode=task_edit">
					<img src="images/add.gif" title="Add Tasks" border="0"></a>
			</td>
		</tr>
		<?
		if( get_total_rows($result) != 0){
			$class = "even";
			$task_list = array();
			$counter = 1;
			$task_list[0] = array (
				"task_id"=>"Task ID",
				"title"=>"Title",
				"description"=>"Description",
				"category_name"=>"Category",
				"sub_category_name"=>"Sub Category",
				"time_allocated"=>"Time Allocated",
				"important"=>"Important",
				"urgency"=>"Urgent",
				"time_split"=>"Time Split",
				"time_split_min"=>"Time Split Minimum",
				"break_time"=>"Break Time",
				"after_hours"=>"After Hours",
				"filling"=>"Filling",
				"due_date"=>"Due Date",
				"specific_start"=>"Specific Start",
				"repeat_start"=>"Repeat Start",
				"repeat_finish"=>"Repeat Finish",
				"productive"=>"Productive",
				"destination"=>"Destination",
				"allocate_1"=>"Allocated To - I",
				"allocate_2"=>"Allocated To - II",
				"is_template"=>"Is Template",
				"template_id"=>"Template ID",
				"created"=>"Created On",
				"created_by"=>"Created By",
				"modified"=>"Modified On",
				"modified_by"=>"Modified By",
				"status"=>"Status"
			 );
			while($row = fetch_mysql_data($result)){
				if($class == "even") $class = "odd";
				else $class = "even";	
				$task_list[$counter] = array (
										"task_id"=>$row['task_id'],
										"title"=>$row['title'],
										"description"=>$row['description'],
										"category_name"=>$row['category_name'],
										"sub_category_name"=>$row['sub_category_name'],
										"time_allocated"=>$row['time_allocated'],
										"important"=>$row['importance'],
										"urgency"=>$row['urgency'],
										"time_split"=>$row['time_split'],
										"time_split_min"=>$row['time_split_min'],
										"break_time"=>$row['break_time'],
										"after_hours"=>$row['after_hours'],
										"filling"=>$row['filling'],
										"due_date"=>$row['due_date_formatted'],
										"specific_start"=>$row['specific_start'],
										"repeat_start"=>$row['repeat_start'],
										"repeat_finish"=>$row['repeat_finish'],
										"productive"=>$row['productive'],
										"destination"=>$row['destination_name'],
										"allocate_1"=>$row['allocate_1_by_name'],
										"allocate_2"=>$row['allocate_2_by_name'],
										"is_template"=>$row['is_template'],
										"template_id"=>$row['template_id'],
										"created"=>$row['created_formatted'],
										"created_by"=>$row['created_by_name'],
										"modified"=>$row['modified_formatted'],
										"modified_by"=>$row['modified_by_name'],
										"status"=>$row['status']
									 );
				$counter++;
		?>
		<tr class="<?echo $class;?>">
			<td>
				<a href="index.php?mode=tasks&sub_mode=task_edit&task_id=<?echo $row["task_id"];?>&call_mode=edit">
					<img src="images/edit.gif" title="Edit Tasks" border="0"></a>
			</td>
			<td><?echo $row["task_id"];?></td>
			<td><?echo $row["created_formatted"];?></td>
			<td nowrap>
				<?
				echo $row["title"];
				if( $row["on_hold_formatted"] != "" ){
					echo " [<a href='#' onclick='javascript:UnHoldTask(" . $row["task_id"] . ", \"tasks\", '')' title='Task on hold till " . $row["on_hold_formatted"] . ".  Click to release'>On hold</a>]";
				}
				echo " [<a href='#' onclick='javascript:holdTask(" . $row["task_id"] . ")' title='Click to put task on hold'><span id='task_hide_span_" . $row["task_id"] . "'>H</span></a>]";
				?>
			</td>
			<td><?echo $row["category_name"];?></td>
			<td nowrap><?echo $row["sub_category_name"];?></td>
			<td nowrap><?echo $row["due_date_formatted"];?></td>
			<td nowrap><?echo getSegment( $row["urgency"], $row["importance"] );?></td>
			<td class="action">
				<a href="index.php?mode=tasks&task_id=<?echo $row["task_id"];?>&call_mode=update_status">
					<?if( $row["status"] == "current" ){?>
					<img src="images/active.gif" title="Status: Current / Click to complete" border="0">
					<?}else{?>
					<img src="images/inactive.gif" title="Status: Complete / Click to make current" border="0">
					<?}?>
				</a>
			</td>
			<td>
				<a href="index.php?mode=tasks&task_id=<?echo $row["task_id"];?>&call_mode=delete" onclick="return confirm('Are you sure to delete this Task?');">
					<img src="images/trash.gif" title="Delete Tasks" border="0"></a>
			</td>
		</tr>
		<?
			}
			if( file_exists('csv/' . getSession("g_uid") . '_tasks_list.csv') ){
				$fp = fopen('csv/' . getSession("g_uid") . '_tasks_list.csv', 'w');
	
				foreach ($task_list as $fields) {
					 fputcsv($fp, $fields);
				}
				
				fclose($fp);
				echo "<script language='javascript'>$('csv_option').show();</script>";	
			}
		}else{
		?>
		<tr class="noResult">
			<td colspan="10">No Tasks found to display</td>
		</tr>
		<?}?>
	</table>
	<?show_hold_option();?>
	<?if($error != ""){?>
	<script language="javascript">//alert("<?echo $error;?>");</script>
	<?}?>
<?
	}
?>