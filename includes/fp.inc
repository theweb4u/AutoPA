<?
	$send_pwd = get_form_values("send_pwd");
	if($send_pwd == 1){
		$email = get_form_values("email");
		if( isBlank($email) ) {
			$msg = "No email address found";
		}else{
			if( checkEmail( $email ) == False ){
				$msg = "Please specify a valid email address";
			}
		}
		
		if($msg == ""){
			//validating for email
			$sql = "select uid from users where email = '" . filter_value($email) . "'";
			$result = query_mysql($sql, $conn);
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				$password = "Pass" . rand();
				
				//including users api file
				include("api/users.inc");
	
				$obj = new api_users;
				$obj -> uid = $row["uid"];
				$obj -> password = $password;
				$obj -> modified = date('Y-m-d H:i:s');
				$obj -> updateObject();
				
				//Sending Email
				$to = $email;
				$subject = "TimeManagement - Password Reset";
				$content = "Your password request was processed and your password is now reset to:\n\n$password";
				SendEmail($to, $subject, $content, $from);
			
				$success_msg = "Your password is reset and emailed to you";
			}else{
				$msg = "No details found for the email, Please try again";
			}
		}
	}
?>
<?if($msg != ""){?>
<script language="javascript">alert("<?echo $msg;?>");</script>
<?}?>
<?if($success_msg != ""){?>
<script language="javascript">
	//alert("<?echo $success_msg;?>");
	parent.Modalbox.hide();
</script>
<?}?>

	<form method="post" action="index.php">
		<input type="hidden" name="send_pwd" value="1">
		<input type="hidden" name="mode" value="fp">
		<input type="hidden" name="template" value="no">
		
		<table class="tblEdit" width="250px">
			<tr>
				<td colspan="2" class="editHeader">Please enter your email..<br><br></td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2"><input type="text" class="textbox" name="email"></td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2">
					<input type="submit" class="button" value="Request">
					<input type="button" class="button" onclick="parent.Modalbox.hide();" value="Cancel">
				</td>
			</tr>
		</table>
	</form>
