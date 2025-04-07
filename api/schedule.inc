<?
class api_schedule_master{
	public $id, $uid, $active, $created, $start_from, $include_segment, $order_by, $status;

	public function getDetail(){
		global $conn;
		$sql = "select schedule_master.*, date_format(created, '%d/%m/%Y %r') as created_formatted, date_format(start_from, '%d/%m/%Y %r') as start_from_formatted, ";
		$sql .= "ifnull(status, '') as status from schedule_master where active = 1";

		if(isset($this -> id)) $sql .= " and id = '" . $this -> id . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> start_from)) $sql .= " and start_from = '" . $this -> start_from . "'";
		if(isset($this -> include_segment)) $sql .= " and include_segment like '%" . $this -> include_segment . "%'";
		if(isset($this -> include_segment)) $sql .= " order by " . $this -> order_by;
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update schedule_master set id = id ";
			if(isset($this -> uid)) $sql .= ", uid = '" . $this -> uid . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> start_from)) $sql .= ", start_from = '" . $this -> start_from . "'";
			if(isset($this -> include_segment)) $sql .= ", include_segment = '" . $this -> include_segment . "'";
			if(isset($this -> status)) $sql .= ", status = '" . $this -> status . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}else{
			$field_sql = "insert schedule_master(id ";
			$value_sql = "values (NULL ";
			if(isset($this -> uid)){
				$field_sql .= ", uid ";
				$value_sql .= ", '" . $this -> uid . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> created)){
				$field_sql .= ", created ";
				$value_sql .= ", '" . $this -> created . "'";
			}else{
				$field_sql .= ", created ";
				$value_sql .= ", now()";
			}
			if(isset($this -> start_from)){
				$field_sql .= ", start_from ";
				$value_sql .= ", '" . $this -> start_from . "'";
			}
			if(isset($this -> start_time)){
				$field_sql .= ", start_time ";
				$value_sql .= ", '" . $this -> start_time . "'";
			}
			if(isset($this -> include_segment)){
				$field_sql .= ", include_segment ";
				$value_sql .= ", '" . $this -> include_segment . "'";
			}
			if(isset($this -> status)){
				$field_sql .= ", status ";
				$value_sql .= ", '" . $this -> status . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_schedule_details{
	public $id, $schedule_id, $source_id, $source_type, $schedule_date, $time_from, $time_to, $active;

	public function getDetail(){
		global $conn;
		$sql = "select schedule_details.*, date_format(schedule_date, '%d/%m/%Y') as schedule_date_formatted, ";
		$sql .= "date_format(schedule_date, '%W') as schedule_date_day_name from schedule_details where active = 1 ";

		if(isset($this -> id)) $sql .= " and id = '" . $this -> id . "'";
		if(isset($this -> schedule_id)) $sql .= " and schedule_id = '" . $this -> schedule_id . "'";
		if(isset($this -> source_id)) $sql .= " and source_id = '" . $this -> source_id . "'";
		if(isset($this -> source_type)) $sql .= " and source_type = '" . $this -> source_type . "'";
		if(isset($this -> schedule_date)) $sql .= " and schedule_date = '" . $this -> schedule_date . "'";
		if(isset($this -> time_from)) $sql .= " and time_from = '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to = '" . $this -> time_to . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		
		$sql .= "order by schedule_date asc, time_from asc";
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update schedule_details set id = id ";
			if(isset($this -> schedule_id)) $sql .= ", schedule_id = '" . $this -> schedule_id . "'";
			if(isset($this -> source_id)) $sql .= ", source_id = '" . $this -> source_id . "'";
			if(isset($this -> source_type)) $sql .= ", source_type = '" . $this -> source_type . "'";
			if(isset($this -> schedule_date)) $sql .= ", schedule_date = '" . $this -> schedule_date . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}else{
			$field_sql = "insert schedule_details(id ";
			$value_sql = "values (NULL ";
			if(isset($this -> schedule_id)){
				$field_sql .= ", schedule_id ";
				$value_sql .= ", '" . $this -> schedule_id . "'";
			}
			if(isset($this -> source_id)){
				$field_sql .= ", source_id ";
				$value_sql .= ", '" . $this -> source_id . "'";
			}
			if(isset($this -> source_type)){
				$field_sql .= ", source_type ";
				$value_sql .= ", '" . $this -> source_type . "'";
			}
			if(isset($this -> schedule_date)){
				$field_sql .= ", schedule_date ";
				$value_sql .= ", '" . $this -> schedule_date . "'";
			}
			if(isset($this -> time_from)){
				$field_sql .= ", time_from ";
				$value_sql .= ", '" . $this -> time_from . "'";
			}
			if(isset($this -> time_to)){
				$field_sql .= ", time_to ";
				$value_sql .= ", '" . $this -> time_to . "'";
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
?>