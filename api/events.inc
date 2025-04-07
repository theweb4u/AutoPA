<?
class api_events{
	public $event_id, $title, $description, $event_date, $destination, $created, $created_by, $modified_by, $modified, $active, $status, $time_from, $time_to, $event_type, $google_id;
	public $custom_query;
	
	public function getDetail(){
		global $conn;
		$sql = "select events.*, date_format(events.event_date, '%d/%m/%Y') as event_date_formatted, date_format(events.created, '%d/%m/%Y') as created_formatted, ";
		$sql .= "date_format(events.modified, '%d/%m/%Y') as modified_formatted, ";
		$sql .= "date_format(events.event_date, '%d-%m-%Y') as event_date_formatted2, ";
		$sql .= "date_format(events.time_from, '%H:%i') as time_from_formatted, ";
		$sql .= "date_format(events.time_to, '%H:%i') as time_to_formatted, event_type, ";
		$sql .= "(select firstname from users where uid = events.created_by) as created_by_name, ";
		$sql .= "(select firstname from users where uid = events.modified_by) as modified_by_name ";
		$sql .= "from events where active = 1";

		if(isset($this -> event_id)) $sql .= " and event_id = '" . $this -> event_id . "'";
		if(isset($this -> title)) $sql .= " and title = '" . $this -> title . "'";
		if(isset($this -> description)) $sql .= " and description = '" . $this -> description . "'";
		if(isset($this -> event_date)) $sql .= " and event_date = '" . $this -> event_date . "'";
		if(isset($this -> custom_query)) $sql .= " and (" . $this -> custom_query . ")";
		if(isset($this -> destination)) $sql .= " and destination = '" . $this -> destination . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> created_by)) $sql .= " and created_by = '" . $this -> created_by . "'";
		if(isset($this -> modified_by)) $sql .= " and modified_by = '" . $this -> modified_by . "'";
		if(isset($this -> modified)) $sql .= " and modified = '" . $this -> modified . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> status)) $sql .= " and status = '" . $this -> status . "'";
		if(isset($this -> time_from)) $sql .= " and time_from >= '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to >= '" . $this -> time_to . "'";
		if(isset($this -> event_type)) $sql .= " and event_type = '" . $this -> event_type . "'";
		if(isset($this -> google_id)) $sql .= " and google_id = '" . $this -> google_id . "'";
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> event_id != ''){
			$sql = "update events set event_id = event_id, modified_by = '" . getSession("g_uid") . "', modified = now() ";
			if(isset($this -> title)) $sql .= ", title = '" . $this -> title . "'";
			if(isset($this -> description)) $sql .= ", description = '" . $this -> description . "'";
			if(isset($this -> event_date)) $sql .= ", event_date = '" . $this -> event_date . "'";
			if(isset($this -> destination)) $sql .= ", destination = '" . $this -> destination . "'";
			if(isset($this -> created)) $sql .= ", created = '" . $this -> created . "'";
			if(isset($this -> created_by)) $sql .= ", created_by = '" . $this -> created_by . "'";
			if(isset($this -> modified_by)) $sql .= ", modified_by = '" . $this -> modified_by . "'";
			if(isset($this -> modified)) $sql .= ", modified = '" . $this -> modified . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> status)) $sql .= ", status = '" . $this -> status . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			if(isset($this -> event_type)) $sql .= ", event_type = '" . $this -> event_type . "'";
			if(isset($this -> google_id)) $sql .= ", google_id = '" . $this -> google_id . "'";
			$sql .= " where event_id = '" . $this -> event_id . "'";
			query_mysql($sql, $conn);
			return $this -> event_id;
		}else{
			$field_sql = "insert events(event_id, created, created_by, status ";
			$value_sql = "values (NULL, now(), '" . getSession("g_uid") . "', 'current' ";
			if(isset($this -> title)){
				$field_sql .= ", title ";
				$value_sql .= ", '" . $this -> title . "'";
			}
			if(isset($this -> description)){
				$field_sql .= ", description ";
				$value_sql .= ", '" . $this -> description . "'";
			}
			if(isset($this -> event_date)){
				$field_sql .= ", event_date ";
				$value_sql .= ", '" . $this -> event_date . "'";
			}
			if(isset($this -> destination)){
				$field_sql .= ", destination ";
				$value_sql .= ", '" . $this -> destination . "'";
			}
			if(isset($this -> created)){
				$field_sql .= ", created ";
				$value_sql .= ", '" . $this -> created . "'";
			}
			if(isset($this -> created_by)){
				$field_sql .= ", created_by ";
				$value_sql .= ", '" . $this -> created_by . "'";
			}
			if(isset($this -> modified_by)){
				$field_sql .= ", modified_by ";
				$value_sql .= ", '" . $this -> modified_by . "'";
			}
			if(isset($this -> modified)){
				$field_sql .= ", modified ";
				$value_sql .= ", '" . $this -> modified . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> status)){
				$field_sql .= ", status ";
				$value_sql .= ", '" . $this -> status . "'";
			}
			if(isset($this -> time_from)){
				$field_sql .= ", time_from ";
				$value_sql .= ", '" . $this -> time_from . "'";
			}
			if(isset($this -> time_to)){
				$field_sql .= ", time_to ";
				$value_sql .= ", '" . $this -> time_to . "'";
			}
			if(isset($this -> event_type)){
				$field_sql .= ", event_type ";
				$value_sql .= ", '" . $this -> event_type . "'";
			}
			if(isset($this -> google_id)){
				$field_sql .= ", google_id ";
				$value_sql .= ", '" . $this -> google_id . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}
?>