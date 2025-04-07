class api_groups{
	public $group_id, $group_name, $company_id, $created, $modified, $active, $total_users;

	public function getDetail(){
		global $conn;
		$sql = "select groups.* from groups where active = 1";

		if(isset($this -> group_id)) $sql .= " and group_id = '" . $this -> group_id . "'";
		if(isset($this -> group_name)) $sql .= " and group_name = '" . $this -> group_name . "'";
		if(isset($this -> company_id)) $sql .= " and company_id = '" . $this -> company_id . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> modified)) $sql .= " and modified = '" . $this -> modified . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> total_users)) $sql .= " and total_users = '" . $this -> total_users . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> group_id != ''){
			$sql = "update groups set group_id = group_id ";
			if(isset($this -> group_name)) $sql .= ", group_name = '" . $this -> group_name . "'";
			if(isset($this -> company_id)) $sql .= ", company_id = '" . $this -> company_id . "'";
			if(isset($this -> created)) $sql .= ", created = '" . $this -> created . "'";
			if(isset($this -> modified)) $sql .= ", modified = '" . $this -> modified . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> total_users)) $sql .= ", total_users = '" . $this -> total_users . "'";
			$sql .= " where group_id = '" . $this -> group_id . "'";
			query_mysql($sql, $conn);
			return $this -> group_id;
		}else{
			$field_sql = "insert groups(group_id ";
			$value_sql = "values (NULL ";
			if(isset($this -> group_name)){
				$field_sql .= ", group_name ";
				$value_sql .= ", '" . $this -> group_name . "'";
			}
			if(isset($this -> company_id)){
				$field_sql .= ", company_id ";
				$value_sql .= ", '" . $this -> company_id . "'";
			}
			if(isset($this -> created)){
				$field_sql .= ", created ";
				$value_sql .= ", '" . $this -> created . "'";
			}
			if(isset($this -> modified)){
				$field_sql .= ", modified ";
				$value_sql .= ", '" . $this -> modified . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> total_users)){
				$field_sql .= ", total_users ";
				$value_sql .= ", '" . $this -> total_users . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

