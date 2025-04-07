<?
	$validate_login = get_form_values("validate_login");
	$confirm_code = get_url_values("confirm_code");
	
	if($validate_login == 1 || $confirm_code != "" || $_COOKIE['autopa_keeploggedin'] != ""){
		//including users api file
		if (class_exists('api_users') == false) {
			include("api/users.inc");
		}
	
		if($validate_login == 1){
			$username = get_form_values("username");
			$password = get_form_values("password");
			
			//login process
			$obj = new api_users;
			$obj -> username = filter_value($username);
			$obj -> password = filter_value($password);
			$obj -> status = "active";
			$result = $obj -> getDetail();
		}else if($_COOKIE['autopa_keeploggedin'] != ""){
			$username = get_form_values("username");
			$password = get_form_values("password");
			
			//login process
			$obj = new api_users;
			$obj -> username = filter_value($_COOKIE['autopa_username']);
			$obj -> key_value = filter_value($_COOKIE['autopa_keeploggedin']);
			$obj -> status = "active";
			$result = $obj -> getDetail();
		}else{
			//registration complete process
			$obj = new api_users;
			$obj -> reg_code = filter_value($confirm_code);
			$obj -> status = "pending";
			$result = $obj -> getDetail();
		}
		
		if( get_total_rows( $result ) != 0){
			$row = fetch_mysql_data($result);
			setSession('g_uid', $row["uid"]);
			setSession('g_username', $row["username"]);
			setSession('g_email', $row["email"]);
			setSession('g_is_admin', $row["is_admin"]);
			
			if($validate_login == 1 || $_COOKIE['autopa_keeploggedin'] != ""){
				setSession('g_welcome_msg', "Welcome back to time management software");
			}else{ 
				setSession('g_welcome_msg', "Congratulations!! You have successfully completed the registration process.");
				
				//inserting default user preferences
				$obj = new api_user_preferences;
				$obj -> uid = $row["uid"];
				$obj -> icon_size = $cnfg_icon_size;
				$obj -> grid_size = $cnfg_grid_size;
				$obj -> default_zoom = $cnfg_default_zoom;
				$obj -> days_to_fit_task = $cnfg_days_to_fit_task;
				$obj -> urgency_importance_allocation = $cnfg_urgency_importance_allocation_type;
				$obj -> min_task = $cnfg_min_task;
				$obj -> insertObject();
				
				//inserting default working hours
				$week_arr = getSplitArray($cnfg_weekdays, ",");
				$obj = new api_working_hours;
				$obj -> uid = $row["uid"];
				for($counter=0;$counter<count($week_arr);$counter++){
					$obj -> weekday = $week_arr["$counter"];
					$obj -> time_from = $cnfg_time_from;
					$obj -> time_to = $cnfg_time_to;
					$obj -> insertObject();
				}
				
				//inserting default break hours
				$break_arr = getSplitArray($cnfg_breaktypes, ",");
				$obj = new api_break_hours;
				$obj -> uid = $row["uid"];
				$obj -> uid = $row["uid"];
				$obj -> variance = 0;
				for($counter=0;$counter<count($break_arr);$counter++){
					$obj -> title = $break_arr[$counter];
					$obj -> time_from = $cnfg_breakfast_from;
					$obj -> time_to = $cnfg_breakfast_to;
					$obj -> insertObject();
				}
			}
			setSession('g_logged_on', date('d-m-Y H:i'));
			
			if($confirm_code != ""){
				//completing the registration process
				$obj = new api_users;
				$obj -> uid = $row["uid"];
				$obj -> reg_confirmed = date('Y-m-d H:i:s');
				$obj -> status = "active";
				$obj -> updateObject();
			}
			
			//redirecting to member section
			if( get_form_values("keep_logged") == "1" ){
				$keep_logged_in_param = "&keep_logged=1";
			}
			echo "<script language='javascript'>location.href='index.php?mode=tasks".$keep_logged_in_param."';</script>";
			exit;
		}else{
			if($validate_login == 1) $msg = "Invalid login..Try again";
			else if($_COOKIE['autopa_keeploggedin'] != "" ) $msg = "Invalid Cookie variable.";
			else $msg = "Registration code have no matching records";
?>
		<script language="javascript">alert("<?echo $msg;?>");</script>
<?		
		}
	}
?>

	<form method="post" action="index.php">
		<input type="hidden" name="validate_login" value="1">
		<input type="hidden" name="mode" value="login">
		
		<table class="tblEdit" width="300px">
			<tr>
				<td colspan="2" class="editHeader">Validate Yourself..<br><br></td>
			</tr>
			<tr>
				<td class="frm_caption">Username:</td>
				<td class="frm_input"><input type="text" class="textbox" name="username"></td>
			</tr>
			<tr>
				<td class="frm_caption">Password:</td>
				<td class="frm_input"><input type="password" class="textbox" name="password"></td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2"><input type="submit" class="button" value="Login">
				<input type="checkbox" name="keep_logged" id="keep_logged" value="1">Keep me logged in.</td>
			</tr>
			<tr>
				<td>
					<span class="note"><a href="index.php?mode=registration">New User? Signup</a></span>
				</td>
				<td align="right">
					<span class="note"><a href="#" onclick="show_fp();">Forgot Password</a></span>
				</td>
			</tr>
		</table>
	</form>

<span style="display:none;" id="fp_span"><iframe width="255px" id="fp" frameborder="0"></iframe></span>
<script language="javascript">
	function show_fp(){
		$('fp').src = "index.php?mode=fp&template=no";
		showModal($('fp_span'), "Forgot Password", 260)
	}
</script>