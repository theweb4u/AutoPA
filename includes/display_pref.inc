<?
	$process_preferences = get_form_values("process_preferences");
	if($process_preferences == 1){
		$icon_size = get_form_values("icon_size");
		$grid_size = get_form_values("grid_size");
		$default_zoom = get_form_values("default_zoom");
		
		//including users api file
		include("api/users.inc");
	
		$obj = new api_user_preferences;
		$obj -> uid = getSession("g_uid");
		$obj -> icon_size = filter_value( get_form_values("icon_size") );
		$obj -> grid_size = filter_value( get_form_values("grid_size") );
		$obj -> default_zoom = filter_value( get_form_values("default_zoom") );
		$obj -> updateObject();
			
		$success_msg = "Display preferences updated successfully";
	}else{
		//getting existing data
		include("api/users.inc");
		$obj = new api_user_preferences;
		$obj -> uid = getSession("g_uid");
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			$row = fetch_mysql_data($result);
			$icon_size = $row["icon_size"];
			$grid_size = $row["grid_size"];
			$default_zoom = $row["default_zoom"];
		}else{
			$obj -> insertObject();
		}
	}
?>

	<form method="post" action="index.php">
		<input type="hidden" name="process_preferences" value="1">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="display_pref">
		
		<table class="tblEdit" width="600px">
			<tr>
				<td colspan="2" class="editHeader">Display Preferences<br><br></td>
			</tr>
			<?if($success_msg != ""){?>
			<tr>
				<td class="success" colspan="2"><?echo $success_msg;?></td>
			</tr>
			<?}?>
			<tr>
				<td class="frm_caption">Size of Icons:</td>
				<td class="frm_input">
					<select name="icon_size" class="select-box">
						<?
							if($icon_size == "") $icon_size = $cnfg_icon_size;
							for($counter=50;$counter<=200;$counter=$counter+50){
								echo "<option value='$counter' ";
								if($icon_size == $counter) echo "selected";
								echo ">$counter</option>";
							}
						?>
					</select>
					pixels
				</td>
			</tr>
			<tr>
				<td class="frm_caption">Grid Size:</td>
				<td class="frm_input">
					<?if($grid_size == "") $grid_size = $cnfg_grid_size;?>
					<select name="grid_size" class="select-box">
						<option value="800 X 600" <?if($grid_size == "800 X 600") echo "selected";?>>800 X 600</option>
						<option value="1024 X 768" <?if($grid_size == "1024 X 768") echo "selected";?>>1024 X 768</option>
						<option value="1280 X 800" <?if($grid_size == "1280 X 800") echo "selected";?>>1280 X 800</option>
					</select>
				</td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2">
					<input type="submit" class="button" value="Update">
				</td>
			</tr>
		</table>
	</form>
