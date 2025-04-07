<?
	$change_password = get_form_values("change_password");
	
	if($change_password == 1){
		$old_password = get_form_values("old_password");
		$new_password = get_form_values("new_password");
		$confirm_password = get_form_values("confirm_password");
		
		if( isBlank($old_password) || isBlank($new_password) || isBlank($confirm_password) ){
			$msg = "Password/s value blank";
		}else if ($new_password != $confirm_password){
			$msg = "New passwords not the same";
		}else{
			//including users api file
			include("api/users.inc");
			
			$obj = new api_users;
			$obj -> uid = getSession('g_uid');
			$obj -> password = filter_value($old_password);
			$result = $obj -> getDetail();
			if( get_total_rows($result) == 0){
				$msg = "Old password mismatch";
			}
		}
		
		if($msg == ""){
			$obj = new api_users;
			$obj -> uid = getSession('g_uid');
			$obj -> modified = date('Y-m-d H:i:s');
			$obj -> password = $new_password;
			$obj -> updateObject();
			$msg = "Password Updated Successfully";
		}
?>
		<script language="javascript">alert("<?echo $msg;?>");</script>
<?		
	}
?>

	<form method="post" action="index.php">
		<input type="hidden" name="change_password" value="1">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="change_password">
		
		<table class="tblEdit" width="300px">
			<tr>
				<td colspan="2" class="editHeader">Change Password..<br><br></td>
			</tr>
			<tr>
				<td class="frm_caption">Old Password:</td>
				<td class="frm_input"><input type="password" class="textbox" name="old_password"></td>
			</tr>
			<tr>
				<td class="frm_caption">New Password:</td>
				<td class="frm_input"><input type="password" class="textbox" name="new_password"></td>
			</tr>
			<tr>
				<td class="frm_caption">Confirm Password:</td>
				<td class="frm_input"><input type="password" class="textbox" name="confirm_password"></td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2"><input type="submit" class="button" value="Update"></td>
			</tr>
		</table>
	</form>
