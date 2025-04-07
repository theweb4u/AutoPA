<?
	$call_mode = get_form_values("call_mode");
	if($call_mode == "") $call_mode = get_url_values("call_mode");
	if($call_mode == "") $call_mode = "display";
	
	$category_id = get_form_values("category_id");
	if($category_id == "") $category_id = get_url_values("category_id");
	
	//including api file
	include("api/types.inc");
	
	if($call_mode == "save_location"){
		//saving preference
		$obj = new api_types;
		$title = get_form_values("title");
		
		
		if( isBlank($title) ){
			$error = "Please specify a location name";
		}
		
		if($error == ""){
			if($category_id != ""){
				$obj -> title = filter_value($title);
				$obj -> category_id = filter_value($category_id);
				$obj -> modified_by = getSession('g_uid');
				$obj -> modified = date('Y-m-d H:i:s');
				$obj -> updateObject();
			}else{
				$obj -> title = filter_value($title);
				$obj -> created_by = getSession('g_uid');
				$obj -> created = date('Y-m-d H:i:s');
				$obj -> type = "location";
				$obj -> updateObject();
			}
			$success_msg = "Location added successfully";
			$call_mode = "display";
		}else{
			$call_mode = "edit";
		}
	}
	
	if($call_mode == "edit" && $error == ""){
		if($category_id != ""){
			$obj = new api_types;
			$obj -> category_id = filter_value($category_id);
			$obj -> created_by = getSession('g_uid');
			$obj -> type = "location";
			$result = $obj -> getDetail();
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				$title = $row["title"];
			}else{
				$error = "Requested location not available for editing";
				$call_mode = "display";
			}
		}
	}
	
	if($call_mode == "delete"){
		$obj = new api_types;
		$obj -> category_id = filter_value($category_id);
		$obj -> created_by = getSession('g_uid');
		$obj -> type = "location";
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			$obj -> active = 0;
			$obj -> updateObject();
			$error = "Requested location deleted successfully";
		}else{
			$error = "Requested location not available for editing";
		}
		$call_mode = "display";
	}
	
	if($call_mode == "display"){
		$obj = new api_types;
		$obj -> created_by = getSession('g_uid');
		$obj -> order_by = "title";
		$obj -> type = "location";
		$result = $obj -> getDetail();
	}
	
	if($call_mode == "edit"){
?>
	<form method="post" action="index.php">
		<input type="hidden" name="call_mode" value="save_location">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="location">
		<input type="hidden" name="category_id" value="<?echo $category_id;?>">
		
		<table class="tblEdit" width="700px">
			<tr>
				<td colspan="2" class="editHeader">Location Update<br><br></td>
			</tr>
			<?if($error != ""){?>
			<tr>
				<td class="error" colspan="2"><?echo $error;?></td>
			</tr>
			<?}?>
			<tr>
				<td class="frm_caption">Location Name:</td>
				<td class="frm_input"><input type="text" class="textbox" name="title" size="55" value="<?echo $title ;?>"></td>
			</tr>
			<tr>
				<td class="frm_input" colspan="4">
					<input type="submit" class="button" value="Update">
					<input type="button" class="button" value="Cancel" onclick="location.href='index.php?mode=preferences&sub_mode=location';">
				</td>
			</tr>
		</table>
	</form>
<?
	}
	
	if($call_mode == "display"){
?>
	<table class="tblDisplay" width="700px">
		<tr class="header">
			<td class="action">@</td>
			<td>Location Name</td>
			<td class="action">
				<a href="index.php?mode=preferences&sub_mode=location&call_mode=edit">
					<img src="images/add.gif" title="Add Location" border="0"></a>
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
				<a href="index.php?mode=preferences&sub_mode=location&category_id=<?echo $row["category_id"];?>&call_mode=edit">
					<img src="images/edit.gif" title="Edit Location" border="0"></a>
			</td>
			<td><?echo $row["title"];?></td>
			<td>
				<a href="index.php?mode=preferences&sub_mode=location&category_id=<?echo $row["category_id"];?>&call_mode=delete" onclick="return confirm('Are you sure to delete this location?');">
					<img src="images/trash.gif" title="Delete Location" border="0"></a>
			</td>
		</tr>
		<?
			}
		}else{
		?>
		<tr class="noResult">
			<td colspan="3">No locations to display</td>
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