<?
	$call_mode = get_form_values("call_mode");
	if($call_mode == "") $call_mode = get_url_values("call_mode");
	if($call_mode == "") $call_mode = "display";
	
	$category_id = get_form_values("category_id");
	if($category_id == "") $category_id = get_url_values("category_id");
	
	$parent_id = get_form_values("parent_id");
	if($parent_id == "") $parent_id = get_url_values("parent_id");
	
	//including api file
	include("api/types.inc");
	
	if($call_mode == "save_category"){
		//saving preference
		$obj = new api_types;
		$title = get_form_values("title");
		$description = get_form_values("description");
		$colour = get_form_values("colour");
		$priority = get_form_values("priority");
		
		if( isBlank($title) ){
			$error = "Please specify a category name";
		}
		
		if($error == ""){
			if($category_id != ""){
				$obj -> title = filter_value($title);
				$obj -> category_id = filter_value($category_id);
				$obj -> description = filter_value($description);
				$obj -> colour = filter_value($colour);
				$obj -> parent_id = filter_value($parent_id);
				$obj -> priority = filter_value($priority);
				$obj -> modified_by = getSession('g_uid');
				$obj -> modified = date('Y-m-d H:i:s');
				$obj -> updateObject();
				$update_category_id = $category_id;
			}else{
				$obj -> title = filter_value($title);
				$obj -> description = filter_value($description);
				$obj -> colour = filter_value($colour);
				$obj -> parent_id = filter_value($parent_id);
				$obj -> priority = filter_value($priority);
				$obj -> created_by = getSession('g_uid');
				$obj -> created = date('Y-m-d H:i:s');
				$obj -> type = "category";
				$update_category_id = $obj -> updateObject();
			}
			$success_msg = "Category added successfully";
			$call_mode = "display";
			
			//saving working hours
			$obj = new api_category_placement;
			$obj -> type_id = $update_category_id;
			$result = $obj -> getDetail();
			
			if( get_total_rows($result) != 0){
				while($row = fetch_mysql_data($result)){				
					$obj -> placement_id = $row["placement_id"];
					$control_name = $row["weekday"] . "_holiday";
					$obj -> is_holiday = filter_value( get_form_values("$control_name") );
					
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
			}else{
				if( get_form_values("copyfrom") == ""){
					$week_arr = getSplitArray($cnfg_weekdays, ",");
					for($counter=0;$counter<count($week_arr);$counter++){
						$obj -> weekday = $week_arr["$counter"];
						$control_name = $week_arr["$counter"] . "_holiday";
						$obj -> is_holiday = filter_value( get_form_values("$control_name") );
					
						if( get_form_values("$control_name") == "1" ){
							$obj -> time_from = "0:00";
							$obj -> time_to = "0:00";
						}else{
							$control_name = $week_arr["$counter"] . "_time_from";
							$obj -> time_from = filter_value( get_form_values("$control_name") );
							
							$control_name = $week_arr["$counter"] . "_time_to";
							$obj -> time_to = filter_value( get_form_values("$control_name") );
						}
						$obj -> updateObject();
					}
				}else{
					$obj = new api_category_placement;
					$obj -> type_id = filter_value($parent_id);
					$result = $obj -> getDetail();
					if( get_total_rows($result) != 0){
						$obj2 = new api_category_placement;
						while($row = fetch_mysql_data($result)){		
							$obj2 -> weekday = $row["weekday"];
							$obj2 -> type_id = $update_category_id;
							$obj2 -> time_from = $row["time_from"];
							$obj2 -> time_to = $row["time_to"];
							$obj2 -> is_holiday = $row["is_holiday"];
							$obj2 -> updateObject();
						}
					}
				}
			}
		}else{
			$call_mode = "edit";
		}
	}
	
	if($call_mode == "edit" && $error == ""){
		if($category_id != ""){
			$obj = new api_types;
			$obj -> category_id = filter_value($category_id);
			$obj -> created_by = getSession('g_uid');
			$obj -> type = "category";
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				$title = $row["title"];
				$description = $row["description"];
				$colour = $row["colour"];
				$priority = $row["priority"];
				$parent_id = $row["parent_id"];
				
				$obj = new api_category_placement;
				$obj -> type_id = filter_value($category_id);
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
			}else{
				$error = "Requested category not available for editing";
				$call_mode = "display";
			}
		}
	}
	
	if($call_mode == "delete"){
		$obj = new api_types;
		$obj -> category_id = filter_value($category_id);
		$obj -> created_by = getSession('g_uid');
		$obj -> type = "category";
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			$obj -> active = 0;
			$obj -> updateObject();
			$error = "Requested category deleted successfully";
			
			$obj = new api_types;
			$obj -> parent_id = filter_value($category_id);
			$obj -> created_by = getSession('g_uid');
			$obj -> type = "category";
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				while($row = fetch_mysql_data($result)){
					$obj = new api_types;
					$obj -> category_id = $row["category_id"];
					$obj -> active = 0;
					$obj -> updateObject();
					$error = "Requested category deleted successfully";
				}
			}
		}else{
			$error = "Requested category not available for editing";
		}
		$call_mode = "display";
	}
	
	if($call_mode == "display"){
		$obj = new api_types;
		$obj -> created_by = getSession('g_uid');
		$obj -> order_by = "priority, title";
		if($parent_id != "") $obj -> parent_id = $parent_id;
		else $obj -> parent_id = 0;
		$obj -> type = "category";
		$result = $obj -> getDetail();
	}
	
	if($call_mode == "edit"){
?>
	<form method="post" action="index.php">
		<input type="hidden" name="call_mode" value="save_category">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="category">
		<input type="hidden" name="category_id" value="<?echo $category_id;?>">
		
		<table class="tblEdit" width="700px">
			<tr>
				<td colspan="2" class="editHeader">Category Update<br><br></td>
			</tr>
			<?if($error != ""){?>
			<tr>
				<td class="error" colspan="2"><?echo $error;?></td>
			</tr>
			<?}?>
			<tr>
				<td class="frm_caption">Category Name:</td>
				<td class="frm_input"><input type="text" class="textbox" name="title" size="55" value="<?echo $title ;?>"></td>
			</tr>
			<tr>
				<td class="frm_caption">Parent ID:</td>
				<td class="frm_input">
					<select name="parent_id" class="select-box">
						<option value="0" <?if($parent_id == 0) echo "selected";?>>Top</option>
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
				<td class="frm_caption">Colour:</td>
				<td class="frm_input">
					<script type="text/javascript" src="js/jscolor/jscolor.js"></script>
					<input type="text" class="color" name="colour" size="10" value="<?echo $colour ;?>">
				</td>
			</tr>
			<tr>
				<td class="frm_caption">Priority:</td>
				<td class="frm_input"><input type="text" class="textboxR" name="priority" size="5" value="<?echo $priority;?>"></td>
			</tr>
			<tr>
				<td class="frm_caption">Description:</td>
				<td class="frm_input">
					<textarea name="description" rows="3" cols="55" class="textarea"><?echo $description;?></textarea>
				</td>
			</tr>
			<tr>
				<td colspan="3">
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
					<table width="100%" border="0">
						<tr>
							<td colspan="6"><b>Set Working Hours</b><br></td>
						</tr>
						<?if( $category_id == "" && $parent_id != "" & $parent_id != "0"){?>
						<tr>
							<td colspan="6">
								<input type="checkbox" value="1" name="copyfrom"> Copy from parent
							</td>
						</tr>
						<?}?>
						<tr>
							<td>Weekday</td>
							<td colspan="2">Hour From</td>
							<td colspan="2">Hour To</td>
							<td>Holiday</td>
						</tr>
						<?
							$week_arr = getSplitArray($cnfg_weekdays, ",");
							for($counter=0;$counter<count($week_arr);$counter++){
						?>
						<tr>
							<td><?echo setUpperFull( $week_arr[$counter] );?></td>
							<td nowrap>
								<?
									$control_name = $week_arr[$counter] . "_time_from";
									$control_value = $$control_name;
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
							<td>
								<a href="#" onclick="copyAll('<?echo $control_name;?>');"><img src="images/down.gif" border="0" title="Copy to other rows"></a>
							</td>
							<td nowrap>
								<?
									$control_name = $week_arr[$counter] . "_time_to";
									$control_value = $$control_name;
									create_time_dropdown($control_name, $control_value)
								?>
							</td>
							<td>
								<a href="#" onclick="copyAll('<?echo $control_name;?>');"><img src="images/down.gif" border="0" title="Copy to other rows"></a>
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
					<script language="javascript">
						function copyAll(controlName){
							var weekdays = "<?echo $cnfg_weekdays?>";
							var week_arr = weekdays.split(",");
							var tempControlName;
							for(var counter=0;counter<week_arr.length;counter++){
								tempControlName = week_arr[counter] + "_time_";
								
								if( controlName.indexOf("time_to") >= 0 ) tempControlName += "to";
								else tempControlName += "from";
								
								if( $(tempControlName).value == "") $(tempControlName).value = $(controlName).value
							}
						}
					</script>
				</td>
			</tr>
			<tr>
				<td class="frm_input" colspan="4">
					<input type="submit" class="button" value="Update">
					<input type="button" class="button" value="Cancel" onclick="location.href='index.php?mode=preferences&sub_mode=category&parent_id=<?echo $parent_id;?>';">
				</td>
			</tr>
		</table>
	</form>
<?
	}
	
	if($call_mode == "display"){
?>
	<table class="tblDisplay" width="700px">
		<?
		if($parent_id != 0 && $parent_id != ""){
			$obj = new api_types;
			$obj -> category_id = filter_value($parent_id);
			$obj -> created_by = getSession('g_uid');
			$obj -> type = "category";
			$result2 = $obj -> getDetail();
			if( get_total_rows($result2) != 0){
				$row = fetch_mysql_data($result2);
		?>
		<tr class="header">
			<td colspan="5">
				<small>
					<a href="index.php?mode=preferences&sub_mode=category&call_mode=display">Home</a> >> <?echo $row["title"];?>
				</small>
			</td>
		</tr>
		<?
			}
		}
		?>
		<tr class="header">
			<td class="action">@</td>
			<td>Category Name</td>
			<td class="action">Priority</td>
			<td class="action">Colour</td>
			<td class="action">
				<a href="index.php?mode=preferences&sub_mode=category&call_mode=edit&parent_id=<?echo $parent_id;?>">
					<img src="images/add.gif" title="Add Category" border="0"></a>
			</td>
		</tr>
		<?
		if( get_total_rows($result) != 0){
			$class = "even";
			while($row = fetch_mysql_data($result)){
				if($class = "even") $class = "odd";
				else $class = "even";
		?>
		<tr class="<?echo $class;?>">
			<td>
				<a href="index.php?mode=preferences&sub_mode=category&category_id=<?echo $row["category_id"];?>&call_mode=edit">
					<img src="images/edit.gif" title="Edit Category" border="0"></a>
			</td>
			<td>
				<?echo $row["title"];?>
				<?if( $row["parent_id"] == 0 ) echo "<a href='index.php?mode=preferences&sub_mode=category&parent_id=" . $row["category_id"] . "'><img src='images/arrowup.gif' border='0' title='Browse this category'></a>";?>
			</td>
			<td><?echo $row["priority"];?></td>
			<td style="background-color: <?echo $row['colour'];?>;">&nbsp;</td>
			<td>
				<a href="index.php?mode=preferences&sub_mode=category&category_id=<?echo $row["category_id"];?>&call_mode=delete" onclick="return confirm('Are you sure to delete this category\nSub category would be deleted as well?');">
					<img src="images/trash.gif" title="Delete Category" border="0"></a>
			</td>
		</tr>
		<?
			}
		}else{
		?>
		<tr class="noResult">
			<td colspan="5">No category to display</td>
		</tr>
		<?
		}
		?>
	</table>
	
	<?if($error != ""){?>
	<script language="javascript">alert("<?echo $error;?>");</script>
	<?}?>
<?
	}
?>