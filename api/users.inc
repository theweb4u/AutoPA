<?
class api_allowed_user_groups{
	public $access_id, $uid, $group_id, $active;

	public function getDetail(){
		global $conn;
		$sql = "select allowed_user_groups.* from allowed_user_groups where active = 1";

		if(isset($this -> access_id)) $sql .= " and access_id = '" . $this -> access_id . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> group_id)) $sql .= " and group_id = '" . $this -> group_id . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> access_id != ''){
			$sql = "update allowed_user_groups set access_id = access_id ";
			if(isset($this -> uid)) $sql .= ", uid = '" . $this -> uid . "'";
			if(isset($this -> group_id)) $sql .= ", group_id = '" . $this -> group_id . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			$sql .= " where access_id = '" . $this -> access_id . "'";
			query_mysql($sql, $conn);
			return $this -> access_id;
		}else{
			$field_sql = "insert allowed_user_groups(access_id ";
			$value_sql = "values (NULL ";
			if(isset($this -> uid)){
				$field_sql .= ", uid ";
				$value_sql .= ", '" . $this -> uid . "'";
			}
			if(isset($this -> group_id)){
				$field_sql .= ", group_id ";
				$value_sql .= ", '" . $this -> group_id . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_login_details{
	public $id, $uid, $login_time, $logout_time, $ip;

	public function getDetail(){
		global $conn;
		$sql = "select login_details.* from login_details where active = 1";

		if(isset($this -> id)) $sql .= " and id = '" . $this -> id . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> login_time)) $sql .= " and login_time = '" . $this -> login_time . "'";
		if(isset($this -> logout_time)) $sql .= " and logout_time = '" . $this -> logout_time . "'";
		if(isset($this -> ip)) $sql .= " and ip = '" . $this -> ip . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update login_details set id = id ";
			if(isset($this -> uid)) $sql .= ", uid = '" . $this -> uid . "'";
			if(isset($this -> login_time)) $sql .= ", login_time = '" . $this -> login_time . "'";
			if(isset($this -> logout_time)) $sql .= ", logout_time = '" . $this -> logout_time . "'";
			if(isset($this -> ip)) $sql .= ", ip = '" . $this -> ip . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}else{
			$field_sql = "insert login_details(id ";
			$value_sql = "values (NULL ";
			if(isset($this -> uid)){
				$field_sql .= ", uid ";
				$value_sql .= ", '" . $this -> uid . "'";
			}
			if(isset($this -> login_time)){
				$field_sql .= ", login_time ";
				$value_sql .= ", '" . $this -> login_time . "'";
			}
			if(isset($this -> logout_time)){
				$field_sql .= ", logout_time ";
				$value_sql .= ", '" . $this -> logout_time . "'";
			}
			if(isset($this -> ip)){
				$field_sql .= ", ip ";
				$value_sql .= ", '" . $this -> ip . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_user_preferences{
	public $uid, $icon_size, $default_zoom, $grid_size, $added_time, $added_time_in_percentage, $days_to_fit_task, $urgency_importance_allocation, $min_task_1, $min_task_2, $min_task_3, $min_task_4;

	public function getDetail(){
		global $conn;
		$sql = "select user_preferences.* from user_preferences where uid = uid";

		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> icon_size)) $sql .= " and icon_size = '" . $this -> icon_size . "'";
		if(isset($this -> default_zoom)) $sql .= " and default_zoom = '" . $this -> default_zoom . "'";
		if(isset($this -> grid_size)) $sql .= " and grid_size = '" . $this -> grid_size . "'";
		if(isset($this -> added_time)) $sql .= " and added_time = '" . $this -> added_time . "'";
		if(isset($this -> added_time_in_percentage)) $sql .= " and added_time_in_percentage = '" . $this -> added_time_in_percentage . "'";
		if(isset($this -> days_to_fit_task)) $sql .= " and days_to_fit_task = '" . $this -> days_to_fit_task . "'";
		if(isset($this -> urgency_importance_allocation)) $sql .= " and urgency_importance_allocation = '" . $this -> urgency_importance_allocation . "'";
		if(isset($this -> min_task_1)) $sql .= " and min_task_1 = '" . $this -> min_task_1 . "'";
		if(isset($this -> min_task_2)) $sql .= " and min_task_2 = '" . $this -> min_task_2 . "'";
		if(isset($this -> min_task_3)) $sql .= " and min_task_3 = '" . $this -> min_task_3 . "'";
		if(isset($this -> min_task_4)) $sql .= " and min_task_4 = '" . $this -> min_task_4 . "'";
		
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> uid != ''){
			$sql = "update user_preferences set uid = uid ";
			if(isset($this -> icon_size)) $sql .= ", icon_size = '" . $this -> icon_size . "'";
			if(isset($this -> default_zoom)) $sql .= ", default_zoom = '" . $this -> default_zoom . "'";
			if(isset($this -> grid_size)) $sql .= ", grid_size = '" . $this -> grid_size . "'";
			if(isset($this -> added_time)) $sql .= ", added_time = '" . $this -> added_time . "'";
			if(isset($this -> added_time_in_percentage)) $sql .= ", added_time_in_percentage = '" . $this -> added_time_in_percentage . "'";
			if(isset($this -> days_to_fit_task)) $sql .= ", days_to_fit_task = '" . $this -> days_to_fit_task . "'";
			if(isset($this -> urgency_importance_allocation)) $sql .= ", urgency_importance_allocation = '" . $this -> urgency_importance_allocation . "'";
			if(isset($this -> min_task_1)) $sql .= ", min_task_1 = '" . $this -> min_task_1 . "'";
			if(isset($this -> min_task_2)) $sql .= ", min_task_2 = '" . $this -> min_task_2 . "'";
			if(isset($this -> min_task_3)) $sql .= ", min_task_3 = '" . $this -> min_task_3 . "'";
			if(isset($this -> min_task_4)) $sql .= ", min_task_4 = '" . $this -> min_task_4 . "'";
			$sql .= " where uid = '" . $this -> uid . "'";
			query_mysql($sql, $conn);
			return $this -> uid;
		}
	}
	
	public function insertObject(){
		global $conn;
		
		$field_sql = "insert user_preferences(uid ";
		$value_sql = "values ('" . $this -> uid . "'";
		if(isset($this -> icon_size)){
			$field_sql .= ", icon_size ";
			$value_sql .= ", '" . $this -> icon_size . "'";
		}
		if(isset($this -> default_zoom)){
			$field_sql .= ", default_zoom ";
			$value_sql .= ", '" . $this -> default_zoom . "'";
		}
		if(isset($this -> grid_size)){
			$field_sql .= ", grid_size ";
			$value_sql .= ", '" . $this -> grid_size . "'";
		}
		if(isset($this -> added_time)){
			$field_sql .= ", added_time ";
			$value_sql .= ", '" . $this -> added_time . "'";
		}
		if(isset($this -> added_time_in_percentage)){
			$field_sql .= ", added_time_in_percentage ";
			$value_sql .= ", '" . $this -> added_time_in_percentage . "'";
		}
		if(isset($this -> days_to_fit_task)){
			$field_sql .= ", days_to_fit_task ";
			$value_sql .= ", '" . $this -> days_to_fit_task . "'";
		}
		if(isset($this -> urgency_importance_allocation)){
			$field_sql .= ", urgency_importance_allocation ";
			$value_sql .= ", '" . $this -> urgency_importance_allocation . "'";
		}
		if(isset($this -> min_task_1)){
			$field_sql .= ", min_task_1 ";
			$value_sql .= ", '" . $this -> min_task_1 . "'";
		}
		if(isset($this -> min_task_2)){
			$field_sql .= ", min_task_2 ";
			$value_sql .= ", '" . $this -> min_task_3 . "'";
		}
		if(isset($this -> min_task_3)){
			$field_sql .= ", min_task_3 ";
			$value_sql .= ", '" . $this -> min_task_3 . "'";
		}
		if(isset($this -> min_task_4)){
			$field_sql .= ", min_task_4 ";
			$value_sql .= ", '" . $this -> min_task_4 . "'";
		}
		$sql .= " $field_sql) $value_sql)";
		query_mysql($sql, $conn);
		$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
		$row = fetch_mysql_data($result);
		return $row["id"];
	}
}

class api_user_tasks{
	public $relation_id, $uid, $task_id;

	public function getDetail(){
		global $conn;
		$sql = "select user_tasks.* from user_tasks where active = 1";

		if(isset($this -> relation_id)) $sql .= " and relation_id = '" . $this -> relation_id . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> task_id)) $sql .= " and task_id = '" . $this -> task_id . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> relation_id != ''){
			$sql = "update user_tasks set relation_id = relation_id ";
			if(isset($this -> uid)) $sql .= ", uid = '" . $this -> uid . "'";
			if(isset($this -> task_id)) $sql .= ", task_id = '" . $this -> task_id . "'";
			$sql .= " where relation_id = '" . $this -> relation_id . "'";
			query_mysql($sql, $conn);
			return $this -> relation_id;
		}else{
			$field_sql = "insert user_tasks(relation_id ";
			$value_sql = "values (NULL ";
			if(isset($this -> uid)){
			$field_sql .= ", uid ";
			$value_sql .= ", '" . $this -> uid . "'";
			}
			if(isset($this -> task_id)){
			$field_sql .= ", task_id ";
			$value_sql .= ", '" . $this -> task_id . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_users{
	public $uid, $company_id, $firstname, $lastname, $email, $password, $username, $street, $suburb, $state, $country, $landline, $mobile, $active, $created, $modified, $group_id, $reg_code, $reg_confirmed, $status, $last_url, $google_user_id, $google_user_pwd, $key_value, $is_admin;

	public function getDetail(){
		global $conn;
		$sql = "select users.*, date_format(created, '%d/%m/%Y') as created_formatted from users where active = 1";

		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> company_id)) $sql .= " and company_id = '" . $this -> company_id . "'";
		if(isset($this -> firstname)) $sql .= " and firstname = '" . $this -> firstname . "'";
		if(isset($this -> lastname)) $sql .= " and lastname = '" . $this -> lastname . "'";
		if(isset($this -> email)) $sql .= " and email = '" . $this -> email . "'";
		if(isset($this -> password)) $sql .= " and password = password('" . $this -> password . "')";
		if(isset($this -> username)) $sql .= " and username = '" . $this -> username . "'";
		if(isset($this -> street)) $sql .= " and street = '" . $this -> street . "'";
		if(isset($this -> suburb)) $sql .= " and suburb = '" . $this -> suburb . "'";
		if(isset($this -> state)) $sql .= " and state = '" . $this -> state . "'";
		if(isset($this -> country)) $sql .= " and country = '" . $this -> country . "'";
		if(isset($this -> landline)) $sql .= " and landline = '" . $this -> landline . "'";
		if(isset($this -> mobile)) $sql .= " and mobile = '" . $this -> mobile . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> modified)) $sql .= " and modified = '" . $this -> modified . "'";
		if(isset($this -> group_id)) $sql .= " and group_id = '" . $this -> group_id . "'";
		if(isset($this -> reg_code)) $sql .= " and reg_code = '" . $this -> reg_code . "'";
		if(isset($this -> reg_confirmed)) $sql .= " and reg_confirmed = '" . $this -> reg_confirmed . "'";
		if(isset($this -> status)) $sql .= " and status = '" . $this -> status . "'";
		if(isset($this -> last_url)) $sql .= " and last_url = '" . $this -> last_url . "'";
		if(isset($this -> key_value)) $sql .= " and key_value = '" . $this -> key_value . "'";
		if(isset($this -> is_admin)) $sql .= " and is_admin = '" . $this -> is_admin . "'";
		
		$result = query_mysql( $sql, $conn );
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> uid != ''){
			$sql = "update users set uid = uid ";
			if(isset($this -> company_id)) $sql .= ", company_id = '" . $this -> company_id . "'";
			if(isset($this -> firstname)) $sql .= ", firstname = '" . $this -> firstname . "'";
			if(isset($this -> lastname)) $sql .= ", lastname = '" . $this -> lastname . "'";
			if(isset($this -> email)) $sql .= ", email = '" . $this -> email . "'";
			if(isset($this -> password)) $sql .= ", password = password('" . $this -> password . "')";
			if(isset($this -> username)) $sql .= ", username = '" . $this -> username . "'";
			if(isset($this -> street)) $sql .= ", street = '" . $this -> street . "'";
			if(isset($this -> suburb)) $sql .= ", suburb = '" . $this -> suburb . "'";
			if(isset($this -> state)) $sql .= ", state = '" . $this -> state . "'";
			if(isset($this -> country)) $sql .= ", country = '" . $this -> country . "'";
			if(isset($this -> landline)) $sql .= ", landline = '" . $this -> landline . "'";
			if(isset($this -> mobile)) $sql .= ", mobile = '" . $this -> mobile . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> created)) $sql .= ", created = '" . $this -> created . "'";
			if(isset($this -> modified)) $sql .= ", modified = '" . $this -> modified . "'";
			if(isset($this -> group_id)) $sql .= ", group_id = '" . $this -> group_id . "'";
			if(isset($this -> reg_code)) $sql .= ", reg_code = '" . $this -> reg_code . "'";
			if(isset($this -> reg_confirmed)) $sql .= ", reg_confirmed = '" . $this -> reg_confirmed . "'";
			if(isset($this -> status)) $sql .= ", status = '" . $this -> status . "'";
			if(isset($this -> last_url)) $sql .= ", last_url = '" . $this -> last_url . "'";
			if(isset($this -> google_user_id)) $sql .= ", google_user_id = '" . $this -> google_user_id . "'";
			if(isset($this -> google_user_pwd)) $sql .= ", google_user_pwd = '" . $this -> google_user_pwd . "'";
			if(isset($this -> key_value)) $sql .= ", key_value = '" . $this -> key_value . "'";
			if(isset($this -> is_admin)) $sql .= " and is_admin = '" . $this -> is_admin . "'";
			
			$sql .= " where uid = '" . $this -> uid . "'";
			query_mysql($sql, $conn);
			return $this -> uid;
		}else{
			$field_sql = "insert users(uid ";
			$value_sql = "values (NULL ";
			if(isset($this -> company_id)){
				$field_sql .= ", company_id ";
				$value_sql .= ", '" . $this -> company_id . "'";
			}
			if(isset($this -> firstname)){
				$field_sql .= ", firstname ";
				$value_sql .= ", '" . $this -> firstname . "'";
			}
			if(isset($this -> lastname)){
				$field_sql .= ", lastname ";
				$value_sql .= ", '" . $this -> lastname . "'";
			}
			if(isset($this -> email)){
				$field_sql .= ", email ";
				$value_sql .= ", '" . $this -> email . "'";
			}
			if(isset($this -> password)){
				$field_sql .= ", password ";
				$value_sql .= ", password('" . $this -> password . "')";
			}
			if(isset($this -> username)){
				$field_sql .= ", username ";
				$value_sql .= ", '" . $this -> username . "'";
			}
			if(isset($this -> street)){
				$field_sql .= ", street ";
				$value_sql .= ", '" . $this -> street . "'";
			}
			if(isset($this -> suburb)){
				$field_sql .= ", suburb ";
				$value_sql .= ", '" . $this -> suburb . "'";
			}
			if(isset($this -> state)){
				$field_sql .= ", state ";
				$value_sql .= ", '" . $this -> state . "'";
			}
			if(isset($this -> country)){
				$field_sql .= ", country ";
				$value_sql .= ", '" . $this -> country . "'";
			}
			if(isset($this -> landline)){
				$field_sql .= ", landline ";
				$value_sql .= ", '" . $this -> landline . "'";
			}
			if(isset($this -> mobile)){
				$field_sql .= ", mobile ";
				$value_sql .= ", '" . $this -> mobile . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> created)){
				$field_sql .= ", created ";
				$value_sql .= ", '" . $this -> created . "'";
			}
			if(isset($this -> modified)){
				$field_sql .= ", modified ";
				$value_sql .= ", '" . $this -> modified . "'";
			}
			if(isset($this -> group_id)){
				$field_sql .= ", group_id ";
				$value_sql .= ", '" . $this -> group_id . "'";
			}
			if(isset($this -> reg_code)){
				$field_sql .= ", reg_code ";
				$value_sql .= ", '" . $this -> reg_code . "'";
			}
			if(isset($this -> reg_confirmed)){
				$field_sql .= ", reg_confirmed ";
				$value_sql .= ", '" . $this -> reg_confirmed . "'";
			}
			if(isset($this -> status)){
				$field_sql .= ", status ";
				$value_sql .= ", '" . $this -> status . "'";
			}
			if(isset($this -> last_url)){
				$field_sql .= ", last_url ";
				$value_sql .= ", '" . $this -> last_url . "'";
			}
			if(isset($this -> google_user_id)){
				$field_sql .= ", google_user_id ";
				$value_sql .= ", '" . $this -> google_user_id . "'";
			}
			if(isset($this -> google_user_pwd)){
				$field_sql .= ", google_user_pwd ";
				$value_sql .= ", '" . $this -> google_user_pwd . "'";
			}
			if(isset($this -> key_value)){
				$field_sql .= ", key_value ";
				$value_sql .= ", '" . $this -> key_value . "'";
			}
			if(isset($this -> is_admin)){
				$field_sql .= ", is_admin ";
				$value_sql .= ", '" . $this -> is_admin . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			
			return $row["id"];
		}
	}
}

class api_working_hours{
	public $uid, $weekday, $time_from, $time_to, $id, $is_holiday;

	public function getDetail(){
		global $conn;
		$sql = "select weekday, id, uid, date_format(time_from, '%H:%i') as time_from, date_format(time_to, '%H:%i') as time_to, is_holiday from working_hours where uid = uid";

		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> weekday)) $sql .= " and weekday = '" . $this -> weekday . "'";
		if(isset($this -> time_from)) $sql .= " and time_from = '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to = '" . $this -> time_to . "'";
		if(isset($this -> is_holiday)) $sql .= " and is_holiday = '" . $this -> is_holiday . "'";
		
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update working_hours set uid = uid ";
			if(isset($this -> weekday)) $sql .= ", weekday = '" . $this -> weekday . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			if(isset($this -> is_holiday)) $sql .= ", is_holiday = '" . $this -> is_holiday . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}
	}
	
	public function insertObject(){
		global $conn;
		$field_sql = "insert working_hours(uid ";
		$value_sql = "values ('" . $this -> uid . "'";
		
		if(isset($this -> weekday)){
			$field_sql .= ", weekday ";
			$value_sql .= ", '" . $this -> weekday . "'";
		}
		if(isset($this -> time_from)){
			$field_sql .= ", time_from ";
			$value_sql .= ", '" . $this -> time_from . "'";
		}
		if(isset($this -> time_to)){
			$field_sql .= ", time_to ";
			$value_sql .= ", '" . $this -> time_to . "'";
		}
		if(isset($this -> is_holiday)){
			$field_sql .= ", is_holiday ";
			$value_sql .= ", '" . $this -> is_holiday . "'";
		}
			
		$sql .= " $field_sql) $value_sql)";
		query_mysql($sql, $conn);
		$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
		$row = fetch_mysql_data($result);
		return $row["id"];
	}
}

class api_break_hours{
	public $title, $uid, $time_from, $time_to, $variance, $id;

	public function getDetail(){
		global $conn;
		$sql = "select id, title, uid, variance, date_format(time_from, '%H:%i') as time_from, date_format(time_to, '%H:%i') as time_to from break_hours ";
		$sql .= "where uid = uid ";

		if(isset($this -> title)) $sql .= " and title = '" . $this -> title . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> time_from)) $sql .= " and time_from = '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to = '" . $this -> time_to . "'";
		if(isset($this -> variance)) $sql .= " and variance = '" . $this -> variance . "'";
		
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update break_hours set uid = uid ";
			if(isset($this -> title)) $sql .= ", title = '" . $this -> title . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			if(isset($this -> variance)) $sql .= ", variance = '" . $this -> variance . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}
	}
	
	public function insertObject(){
		global $conn;
		$field_sql = "insert break_hours(id ";
		$value_sql = "values (NULL ";
		if(isset($this -> title)){
			$field_sql .= ", title ";
			$value_sql .= ", '" . $this -> title . "'";
		}
		if(isset($this -> uid)){
			$field_sql .= ", uid ";
			$value_sql .= ", '" . $this -> uid . "'";
		}
		if(isset($this -> time_from)){
			$field_sql .= ", time_from ";
			$value_sql .= ", '" . $this -> time_from . "'";
		}
		if(isset($this -> time_to)){
			$field_sql .= ", time_to ";
			$value_sql .= ", '" . $this -> time_to . "'";
		}
		if(isset($this -> variance)){
			$field_sql .= ", variance ";
			$value_sql .= ", '" . $this -> variance . "'";
		}
			
		$sql .= " $field_sql) $value_sql)";
		query_mysql($sql, $conn);
		$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
		$row = fetch_mysql_data($result);
		return $row["id"];
	}
}

class api_after_hours{
	public $dayname, $uid, $time_from, $time_to, $id;

	public function getDetail(){
		global $conn;
		$sql = "select id, dayname, uid, date_format(time_from, '%H:%i') as time_from, date_format(time_to, '%H:%i') as time_to from after_hours ";
		$sql .= "where uid = uid ";

		if(isset($this -> dayname)) $sql .= " and dayname = '" . $this -> dayname . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> time_from)) $sql .= " and time_from = '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to = '" . $this -> time_to . "'";
		$sql .= " order by dayname";
		
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update after_hours set uid = uid ";
			if(isset($this -> dayname)) $sql .= ", dayname = '" . $this -> dayname . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}
	}
	
	public function insertObject(){
		global $conn;
		$field_sql = "insert after_hours(id ";
		$value_sql = "values (NULL ";
		if(isset($this -> dayname)){
			$field_sql .= ", dayname ";
			$value_sql .= ", '" . $this -> dayname . "'";
		}
		if(isset($this -> uid)){
			$field_sql .= ", uid ";
			$value_sql .= ", '" . $this -> uid . "'";
		}
		if(isset($this -> time_from)){
			$field_sql .= ", time_from ";
			$value_sql .= ", '" . $this -> time_from . "'";
		}
		if(isset($this -> time_to)){
			$field_sql .= ", time_to ";
			$value_sql .= ", '" . $this -> time_to . "'";
		}
		$sql .= " $field_sql) $value_sql)";
		query_mysql($sql, $conn);
		$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
		$row = fetch_mysql_data($result);
		return $row["id"];
	}
}
?>