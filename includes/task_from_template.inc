<?
	$parent_id = get_form_values("parent_id");
	$sub_category = get_form_values("sub_category");
	
	if( get_form_values("call_mode") == "create" ){
		if( $parent_id != "" && $sub_category != "" ){
			$obj = new api_tasks;
			$obj -> category_id = $parent_id;
			$obj -> sub_category_id = $sub_category;
			$obj -> is_template = 1;
			$obj -> created_by = getSession('g_uid');
			$obj -> active = 1;
			$result = $obj -> getDetail();
			
			if( get_total_rows($result) != 0){
				$counter = 0;
				while( $row = fetch_mysql_data($result) ){
					$counter++;
					$obj2 = new api_tasks;
					$obj2 -> category_id = $row["category_id"];
					$obj2 -> sub_category_id = $row["sub_category_id"];
					$obj2 -> title = $row["title"];
					$obj2 -> description = $row["description"];
					$obj2 -> importance = $row["important"];
					$obj2 -> urgency = $row["urgent"];
					$obj2 -> time_allocated = $row["time_allocated"];
					$obj2 -> time_split = $row["time_split"];
					$obj2 -> time_split_min = $row["minimum"];
					$obj2 -> break_time = $row["break"];
					$obj2 -> filling = $row["filing"];
					$obj2 -> specific_time = $row["specific_time"];
					$obj2 -> repeat_start = $row["specific_time_start"];
					$obj2 -> repeat_finish = $row["specific_time_finish"];
					$obj2 -> productive = $row["productivity"];
					$obj2 -> destination = $row["destination"];
					$obj2 -> template_id = $row["task_id"];
					$obj2 -> status = "current";
					$obj2 -> updateObject();
				}
?>
<script language="javascript">//alert(" <?echo $counter?> new tasks created");</script>
<?
			}
		}else{
			$error = "1";
		}
	}
?>
<form method="post" action="index.php" name="editform">
	<input type="hidden" name="call_mode" id="call_mode" value="">
	<input type="hidden" name="mode" value="tasks">
	<input type="hidden" name="sub_mode" value="task_from_template">
	
	<table class="tblEdit" width="700px">
		<tr>
			<td colspan="2" class="editHeader">Task from Template<br><br></td>
		</tr>
		<?if($error == "1"){?>
		<tr>
			<td colspan="2"><font color="red">Category/Sub Category not selected</font></td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_caption">Category Name:</td>
			<td class="frm_input">
				<select name="parent_id" class="select-box" onchange="document.editform.submit();">
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
			</td>
		</tr>
		<tr>
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
		<tr>
			<td class="frm_input" colspan="4">
				<input type="submit" class="button" value="Create Task" onclick="$('call_mode').value='create';">
				<input type="button" class="button" value="Cancel" onclick="location.href='index.php?mode=tasks">
			</td>
		</tr>
	</table>
</form>