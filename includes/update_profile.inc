<?
	$process_registration = get_form_values("process_registration");
	if($process_registration == 1){
		$firstname = get_form_values("firstname");
		$lastname = get_form_values("lastname");
		$email = get_form_values("email");
		$password = get_form_values("password");
		$password2 = get_form_values("password2");
		$username = get_form_values("username");
		$street = get_form_values("street");
		$suburb = get_form_values("suburb");
		$state = get_form_values("state");
		$country = get_form_values("country");
		$landline = get_form_values("landline");
		$mobile = get_form_values("mobile");
		
		//validation
		if( isBlank($firstname) ) {
			$firstname_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($lastname) ) {
			$lastname_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($email) ) {
			$email_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}else{
			if( checkEmail( $email ) == False ){
				$email_error = "<a href='#' title='Not a valid email'>*</a>";
				$error = "1";
			}
		}
		
		if( isBlank($street) ) {
			$street_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($suburb) ) {
			$suburb_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($state) ) {
			$state_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($country) ) {
			$country_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($landline) ) {
			$landline_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		if( isBlank($mobile) ) {
			$mobile_error = "<a href='#' title='Value Expected'>*</a>";
			$error = "1";
		}
		
		if($error != "1"){
			//validating for duplicate username/email
			$sql = "select email from users where email = '" . filter_value($email) . "' and uid != " . getSession("g_uid");
			$result = query_mysql($sql, $conn);
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				if($email = $row["email"]){
					$email_error = "<a href='#' title='Duplicate Email'>*</a>";
					$error = "1";
				}
			}
		}
		
		if($error != "1"){
			//including users api file
			include("api/users.inc");
	
			$obj = new api_users;
			$obj -> uid = getSession("g_uid");
			$obj -> firstname = filter_value( get_form_values("firstname") );
			$obj -> lastname = filter_value( get_form_values("lastname") );
			$obj -> email = filter_value( get_form_values("email") );
			$obj -> street = filter_value( get_form_values("street") );
			$obj -> suburb = filter_value( get_form_values("suburb") );
			$obj -> state = filter_value( get_form_values("state") );
			$obj -> country = filter_value( get_form_values("country") );
			$obj -> landline = filter_value( get_form_values("landline") );
			$obj -> mobile = filter_value( get_form_values("mobile") );
			$obj -> google_user_pwd = filter_value( get_form_values("google_user_pwd") );
			$obj -> google_user_id = filter_value( get_form_values("google_user_id") );
			$obj -> modified = date('Y-m-d H:i:s');
			$obj -> updateObject();
			
			$success_msg = "Your profile has been updated successfully";
		}else{
			$msg = "Validation Failed...Please check the field marked with *";
		}
	}else{
		//getting existing data
		include("api/users.inc");
		$obj = new api_users;
		$obj -> uid = getSession("g_uid");
		$result = $obj -> getDetail();
		
		if( get_total_rows($result) != 0){
			$row = fetch_mysql_data($result);
			$firstname = $row["firstname"];
			$lastname = $row["lastname"];
			$email = $row["email"];
			$street = $row["street"];
			$suburb = $row["suburb"];
			$state = $row["state"];
			$country = $row["country"];
			$landline = $row["landline"];
			$mobile = $row["mobile"];
			$google_user_id = $row["google_user_id"];
			$google_user_pwd = $row["google_user_pwd"];
		}
	}
?>

	<form method="post" action="index.php">
		<input type="hidden" name="process_registration" value="1">
		<input type="hidden" name="mode" value="preferences">
		<input type="hidden" name="sub_mode" value="update_profile">
		
		<table class="tblEdit" width="600px">
			<tr>
				<td colspan="2" class="editHeader">Update Profile<br><br></td>
			</tr>
			<?if($msg != ""){?>
			<tr>
				<td class="error" colspan="2"><?echo $msg;?></td>
			</tr>
			<?}?>
			<?if($success_msg != ""){?>
			<tr>
				<td class="success" colspan="2"><?echo $success_msg;?></td>
			</tr>
			<?}else{?>
			<tr>
				<td class="frm_caption">First Name:</td>
				<td class="frm_input"><input type="text" class="textbox" name="firstname" id="firstname" value="<?echo $firstname;?>"><?echo $firstname_error;?></td>
			</tr>
			<tr>
				<td class="frm_caption">Last Name:</td>
				<td class="frm_input"><input type="text" class="textbox" name="lastname" id="lastname" value="<?echo $lastname;?>"><?echo $lastname_error;?></td>
			</tr>
			<tr>
				<td class="frm_caption">Email:</td>
				<td class="frm_input"><input type="text" class="textbox" name="email" id="email" value="<?echo $email;?>"><?echo $email_error;?></td>
			</tr>
			<tr>
				<td class="frm_caption">Address:</td>
				<td class="frm_input">
					<input type="text" class="textbox" name="street" id="street" value="<?echo $street;?>">: Street<?echo $street_error;?><br>
					<input type="text" class="textbox" name="suburb" id="suburb" value="<?echo $suburb;?>">: Suburb<?echo $suburb_error;?><br>
					<input type="text" class="textbox" name="state" id="state" value="<?echo $state;?>">: State<?echo $state_error;?><br>
					<select name="country" id="country">
						<option value="">Please select</option>
						<?
							$country_list = "Afghanistan,Albania,Algeria,American Samoa,Andorra,Angola,Anguilla,Antarctica,Antigua and Barbuda,Argentina,Armenia,";
							$country_list .= "Aruba,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,Barbados,Belarus,Belgium,Belize,Benin,Bermuda,Bhutan,";
							$country_list .= "Bolivia,Bosnia and Herzegowina,Botswana,Bouvet Island,Brazil,British Indian Ocean Territory,Brunei Darussalam,Bulgaria,";
							$country_list .= "Burkina Faso,Burundi,Cambodia,Cameroon,Canada,Cape Verde,Cayman Islands,Central African Republic,Chad,Chile,China,";
							$country_list .= "Christmas Island,Cocos (Keeling) Islands,Colombia,Comoros,Congo,Congo, the Democratic Republic of the,Cook Islands,";
							$country_list .= "Costa Rica,Croatia (Hrvatska),Cuba,Cyprus,Czech Republic,Denmark,Djibouti,Dominica,Dominican Republic,East Timor,";
							$country_list .= "Ecuador,Egypt,El Salvador,Equatorial Guinea,Eritrea,Estonia,Ethiopia,Falkland Islands (Malvinas),Faroe Islands,Fiji,";
							$country_list .= "Finland,France,France, Metropolitan,French Guiana,French Polynesia,French Southern Territories,Gabon,Gambia,Georgia,";
							$country_list .= "Germany,Ghana,Gibraltar,Greece,Greenland,Grenada,Guadeloupe,Guam,Guatemala,Guinea,Guinea-Bissau,Guyana,Haiti,";
							$country_list .= "Heard and Mc Donald Islands,Holy See (Vatican City State),Honduras,Hong Kong,Hungary,Iceland,India,Indonesia,";
							$country_list .= "Iran (Islamic Republic of),Iraq,Ireland,Israel,Italy,Jamaica,Japan,Jordan,Kazakhstan,Kenya,Kiribati,Korea, ";
							$country_list .= "Republic of,Kuwait,Kyrgyzstan,Latvia,Lebanon,Lesotho,Liberia,Libyan Arab Jamahiriya,Liechtenstein,Lithuania,";
							$country_list .= "Luxembourg,Macau,Macedonia, The Former Yugoslav Republic of,Madagascar,Malawi,Malaysia,Maldives,Mali,Malta,";
							$country_list .= "Marshall Islands,Martinique,Mauritania,Mauritius,Mayotte,Mexico,Micronesia, Federated States of,Moldova,";
							$country_list .= "Republic of,Monaco,Mongolia,Montserrat,Morocco,Mozambique,Myanmar,Namibia,Nauru,Nepal,Netherlands,";
							$country_list .= "Netherlands Antilles,New Caledonia,New Zealand,Nicaragua,Niger,Nigeria,Niue,Norfolk Island,Northern Mariana Islands,";
							$country_list .= "Norway,Oman,Pakistan,Palau,Panama,Papua New Guinea,Paraguay,Peru,Philippines,Pitcairn,Poland,Portugal,Puerto Rico,";
							$country_list .= "Qatar,Reunion,Romania,Russian Federation,Rwanda,Saint Kitts and Nevis,Saint LUCIA,Saint Vincent and the Grenadines,";
							$country_list .= "Samoa,San Marino,Sao Tome and Principe,Saudi Arabia,Senegal,Seychelles,Sierra Leone,Singapore,";
							$country_list .= "Slovakia (Slovak Republic),Slovenia,Solomon Islands,Somalia,South Africa,South Georgia and the South Sandwich Islands,";
							$country_list .= "Spain,Sri Lanka,St. Helena,St. Pierre and Miquelon,Sudan,Suriname,Svalbard and Jan Mayen Islands,Swaziland,Sweden,";
							$country_list .= "Switzerland,Syrian Arab Republic,Taiwan, Province of China,Tajikistan,Tanzania, United Republic of,Thailand,Togo,";
							$country_list .= "Tokelau,Tonga,Trinidad and Tobago,Tunisia,Turkey,Turkmenistan,Turks and Caicos Islands,Tuvalu,Uganda,Ukraine,";
							$country_list .= "United Arab Emirates,United Kingdom,United States,United States Minor Outlying Islands,Uruguay,Uzbekistan,Vanuatu,";
							$country_list .= "Venezuela,Viet Nam,Virgin Islands (British),Virgin Islands (U.S.),Wallis and Futuna Islands,Western Sahara,Yemen,";
							$country_list .= "Yugoslavia,Zambia,Zimbabwe";
							$country_arr = getSplitArray($country_list, ',');
							for($counter=0;$counter<count($country_arr);$counter++){
						?>
					
						<option value="<?echo $country_arr[$counter];?>" <?if($country == $country_arr[$counter]) echo "selected";?>><?echo $country_arr[$counter];?></option>
					
						<?
							}
						?>
					</select>: Country<?echo $country_error;?>
				</td>
			</tr>
			<tr>
				<td class="frm_caption">Phone:</td>
				<td class="frm_input">
					<input type="text" class="textbox" name="landline" id="landline" value="<?echo $landline;?>">: Landline<?echo $landline_error;?><br>
					<input type="text" class="textbox" name="mobile" id="mobile" value="<?echo $mobile;?>">: Mobile<?echo $mobile_error;?>
				</td>
			</tr>
			<tr>
				<td class="frm_caption">Google User ID:</td>
				<td class="frm_input">
					<input type="text" class="textbox" name="google_user_id" id="google_user_id" value="<?echo $google_user_id;?>">(For Google Events)
				</td>
			</tr>
			<tr>
				<td class="frm_caption">Google Password:</td>
				<td class="frm_input">
					<input type="text" class="textbox" name="google_user_pwd" id="google_user_pwd" value="<?echo $google_user_pwd;?>">
				</td>
			</tr>
			<tr>
				<td class="frm_input" colspan="2">
					<input type="submit" class="button" value="Update">
					<input type="button" class="button" value="Cancel" onclick="location.href='index.php';">
				</td>
			</tr>
			<?}?>
		</table>
	</form>
