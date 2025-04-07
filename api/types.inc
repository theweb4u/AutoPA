<?
class api_types{
	public $category_id, $title, $description, $colour, $priority, $placement_hours, $active, $created, $modified, $created_by, $modified_by, $parent_id, $type, $company_id;
	public $order_by;

	public function getDetail(){
		global $conn;
		$sql = "select types.* from types where active = 1";

		if(isset($this -> category_id)) $sql .= " and category_id = '" . $this -> category_id . "'";
		if(isset($this -> title)) $sql .= " and title = '" . $this -> title . "'";
		if(isset($this -> description)) $sql .= " and description = '" . $this -> description . "'";
		if(isset($this -> colour)) $sql .= " and colour = '" . $this -> colour . "'";
		if(isset($this -> priority)) $sql .= " and priority = '" . $this -> priority . "'";
		if(isset($this -> placement_hours)) $sql .= " and placement_hours = '" . $this -> placement_hours . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> modified)) $sql .= " and modified = '" . $this -> modified . "'";
		if(isset($this -> created_by)) $sql .= " and created_by = '" . $this -> created_by . "'";
		if(isset($this -> modified_by)) $sql .= " and modified_by = '" . $this -> modified_by . "'";
		if(isset($this -> parent_id)) $sql .= " and parent_id = '" . $this -> parent_id . "'";
		if(isset($this -> type)) $sql .= " and type = '" . $this -> type . "'";
		if(isset($this -> company_id)) $sql .= " and company_id = '" . $this -> company_id . "'";
		if(isset($this -> order_by)) $sql .= " order by " . $this -> order_by;
		
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> category_id != ''){
			$sql = "update types set category_id = category_id ";
			if(isset($this -> title)) $sql .= ", title = '" . $this -> title . "'";
			if(isset($this -> description)) $sql .= ", description = '" . $this -> description . "'";
			if(isset($this -> colour)) $sql .= ", colour = '" . $this -> colour . "'";
			if(isset($this -> priority)) $sql .= ", priority = '" . $this -> priority . "'";
			if(isset($this -> placement_hours)) $sql .= ", placement_hours = '" . $this -> placement_hours . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> created)) $sql .= ", created = '" . $this -> created . "'";
			if(isset($this -> modified)) $sql .= ", modified = '" . $this -> modified . "'";
			if(isset($this -> created_by)) $sql .= ", created_by = '" . $this -> created_by . "'";
			if(isset($this -> modified_by)) $sql .= ", modified_by = '" . $this -> modified_by . "'";
			if(isset($this -> parent_id)) $sql .= ", parent_id = '" . $this -> parent_id . "'";
			if(isset($this -> type)) $sql .= ", type = '" . $this -> type . "'";
			if(isset($this -> company_id)) $sql .= ", company_id = '" . $this -> company_id . "'";
			$sql .= " where category_id = '" . $this -> category_id . "'";
			query_mysql($sql, $conn);
			return $this -> category_id;
		}else{
			$field_sql = "insert types(category_id ";
			$value_sql = "values (NULL ";
			if(isset($this -> title)){
				$field_sql .= ", title ";
				$value_sql .= ", '" . $this -> title . "'";
			}
			if(isset($this -> description)){
				$field_sql .= ", description ";
				$value_sql .= ", '" . $this -> description . "'";
			}
			if(isset($this -> colour)){
				$field_sql .= ", colour ";
				$value_sql .= ", '" . $this -> colour . "'";
			}
			if(isset($this -> priority)){
				$field_sql .= ", priority ";
				$value_sql .= ", '" . $this -> priority . "'";
			}
			if(isset($this -> placement_hours)){
				$field_sql .= ", placement_hours ";
				$value_sql .= ", '" . $this -> placement_hours . "'";
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
			if(isset($this -> created_by)){
				$field_sql .= ", created_by ";
				$value_sql .= ", '" . $this -> created_by . "'";
			}
			if(isset($this -> modified_by)){
				$field_sql .= ", modified_by ";
				$value_sql .= ", '" . $this -> modified_by . "'";
			}
			if(isset($this -> parent_id)){
				$field_sql .= ", parent_id ";
				$value_sql .= ", '" . $this -> parent_id . "'";
			}
			if(isset($this -> type)){
				$field_sql .= ", type ";
				$value_sql .= ", '" . $this -> type . "'";
			}
			if(isset($this -> company_id)){
				$field_sql .= ", company_id ";
				$value_sql .= ", '" . $this -> company_id . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_category_placement{
	public $placement_id, $type_id, $weekday, $time_from, $time_to, $active, $is_holiday;

	public function getDetail(){
		global $conn;
		$sql = "select date_format(time_from, '%H:%i') as time_from, date_format(time_to, '%H:%i') as time_to, type_id, weekday, placement_id, is_holiday from ";
		$sql .= "category_placement where active = 1";

		if(isset($this -> placement_id)) $sql .= " and placement_id = '" . $this -> placement_id . "'";
		if(isset($this -> type_id)) $sql .= " and type_id = '" . $this -> type_id . "'";
		if(isset($this -> weekday)) $sql .= " and weekday = '" . $this -> weekday . "'";
		if(isset($this -> time_from)) $sql .= " and time_from = '" . $this -> time_from . "'";
		if(isset($this -> time_to)) $sql .= " and time_to = '" . $this -> time_to . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> is_holiday)) $sql .= " and is_holiday = '" . $this -> is_holiday . "'";
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> placement_id != ''){
			$sql = "update category_placement set placement_id = placement_id ";
			if(isset($this -> type_id)) $sql .= ", type_id = '" . $this -> type_id . "'";
			if(isset($this -> weekday)) $sql .= ", weekday = '" . $this -> weekday . "'";
			if(isset($this -> time_from)) $sql .= ", time_from = '" . $this -> time_from . "'";
			if(isset($this -> time_to)) $sql .= ", time_to = '" . $this -> time_to . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> is_holiday)) $sql .= ", is_holiday = '" . $this -> is_holiday . "'";
			$sql .= " where placement_id = '" . $this -> placement_id . "'";
			query_mysql($sql, $conn);
			return $this -> placement_id;
		}else{
			$field_sql = "insert category_placement(placement_id ";
			$value_sql = "values (NULL ";
			if(isset($this -> type_id)){
				$field_sql .= ", type_id ";
				$value_sql .= ", '" . $this -> type_id . "'";
			}
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
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
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
}
?>