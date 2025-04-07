<?
class api_tasks{
	public $task_id, $title, $description, $category_id, $sub_category_id, $time_allocated, $importance, $urgency, $time_split, $time_split_min, $break_time, $filling, $due_date, $specific_time, $repeat_start, $repeat_finish, $productive, $destination, $allocate_1, $allocate_2, $is_template, $created, $created_by, $modified_by, $modified, $active, $order_by, $template_id, $status, $after_hours, $on_hold, $task_type, $x_value, $y_value, $page_width, $page_height;

	public function getDetail(){
		global $conn;
		$sql = "select tasks.*, (select title from types where types.category_id = tasks.category_id) as category_name, ";
		$sql .= "(select title from types where types.category_id = tasks.sub_category_id) as sub_category_name, ";
		$sql .= "(select title from types where types.category_id = tasks.destination) as destination_name, ";
		$sql .= "(select firstname from users where uid = tasks.created_by) as created_by_name, ";
		$sql .= "(select firstname from users where uid = tasks.modified_by) as modified_by_name, ";
		$sql .= "(select firstname from users where uid = tasks.allocate_1) as allocate_1_by_name, ";
		$sql .= "IF(due_date = '0000-00-00 00:00:00', '', date_format(due_date, '%d/%m/%Y %H:%i')) as due_date_formatted, ";
		$sql .= "IF(due_date = '0000-00-00 00:00:00', '', date_format(due_date, '%d-%m-%Y %H:%i')) as due_date_formatted2, ";
		$sql .= "IF(on_hold = '0000-00-00', '', date_format(on_hold, '%d-%m-%Y')) as on_hold_formatted, ";
		$sql .= "if( length(tasks.title) > 100, concat(left(tasks.title, 100), '...'), tasks.title) as display_title, ";
		$sql .= "(select firstname from users where uid = tasks.allocate_2) as allocate_2_by_name, ";
		$sql .= "IF(created = '0000-00-00 00:00:00', '', date_format(created, '%d/%m/%Y')) as created_formatted, ";
		$sql .= "IF(modified = '0000-00-00 00:00:00', '', date_format(modified, '%d/%m/%Y')) as modified_formatted ";
		$sql .= "from tasks where active = 1";

		if(isset($this -> task_id)) $sql .= " and task_id = '" . $this -> task_id . "'";
		if(isset($this -> title)) $sql .= " and title like '%" . $this -> title . "%'";
		if(isset($this -> description)) $sql .= " and description = '" . $this -> description . "'";
		if(isset($this -> category_id)) $sql .= " and category_id = '" . $this -> category_id . "'";
		if(isset($this -> sub_category_id)) $sql .= " and sub_category_id = '" . $this -> sub_category_id . "'";
		if(isset($this -> time_allocated)) $sql .= " and time_allocated = '" . $this -> time_allocated . "'";
		if(isset($this -> importance)) $sql .= " and importance = '" . $this -> importance . "'";
		if(isset($this -> urgency)) $sql .= " and urgency = '" . $this -> urgency . "'";
		if(isset($this -> time_split)) $sql .= " and time_split = '" . $this -> time_split . "'";
		if(isset($this -> time_split_min)) $sql .= " and time_split_min = '" . $this -> time_split_min . "'";
		if(isset($this -> break_time)) $sql .= " and break_time = '" . $this -> break_time . "'";
		if(isset($this -> filling)) $sql .= " and filling = '" . $this -> filling . "'";
		if(isset($this -> due_date)) $sql .= " and due_date = '" . $this -> due_date . "'";
		if(isset($this -> specific_time)) $sql .= " and specific_time = '" . $this -> specific_time . "'";
		if(isset($this -> repeat_start)) $sql .= " and repeat_start = '" . $this -> repeat_start . "'";
		if(isset($this -> repeat_finish)) $sql .= " and repeat_finish = '" . $this -> repeat_finish . "'";
		if(isset($this -> productive)) $sql .= " and productive = '" . $this -> productive . "'";
		if(isset($this -> destination)) $sql .= " and destination = '" . $this -> destination . "'";
		if(isset($this -> allocate_1)) $sql .= " and allocate_1 = '" . $this -> allocate_1 . "'";
		if(isset($this -> allocate_2)) $sql .= " and allocate_2 = '" . $this -> allocate_2 . "'";
		if(isset($this -> is_template)) $sql .= " and is_template = '" . $this -> is_template . "'";
		if(isset($this -> created)) $sql .= " and created = '" . $this -> created . "'";
		if(isset($this -> created_by)) $sql .= " and created_by = '" . $this -> created_by . "'";
		if(isset($this -> modified_by)) $sql .= " and modified_by = '" . $this -> modified_by . "'";
		if(isset($this -> modified)) $sql .= " and modified = '" . $this -> modified . "'";
		if(isset($this -> template_id)) $sql .= " and template_id = '" . $this -> template_id . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> status)) $sql .= " and status = '" . $this -> status . "'";
		if(isset($this -> after_hours)) $sql .= " and after_hours = '" . $this -> after_hours . "'";
		if(isset($this -> task_type)){
			if( $this -> task_type == "completed") $sql .= " and status = 'complete'";
			if( $this -> task_type == "not_completed") $sql .= " and status = 'current'";
			if( $this -> task_type == "on_hold") $sql .= " and on_hold != '0000-00-00'";
		}
		if(isset($this -> order_by)) $sql .= " order by " . $this -> order_by;
		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateUrgencyImportance(){
		global $conn;
		$sql = "update tasks set ";
		$sql .= "importance = if(ceil( x_value / (page_width/10)) > 10, 10, ceil( x_value / (page_width/10))), ";
		$sql .= "urgency = if(ceil( y_value / (page_height/10)) > 10, 10, ceil( y_value / (page_height/10))) ";
		$sql .= "where task_id = '" . $this -> task_id . "'";
		query_mysql($sql, $conn);
	}
	
	public function updatePosition(){
		global $conn;
		$sql = "update tasks set ";
		$sql .= "x_value = ceil( importance * (page_width/11)), ";
		$sql .= "y_value = ceil( urgency * (page_height/11)) ";
		$sql .= "where task_id = '" . $this -> task_id . "'";
		query_mysql($sql, $conn);
	}
	
	public function updateObject(){
		global $conn;
		if( $this -> task_id != ''){
			$sql = "update tasks set task_id = task_id, modified_by = '" . getSession("g_uid") . "', modified = now() ";
			if(isset($this -> title)) $sql .= ", title = '" . $this -> title . "'";
			if(isset($this -> description)) $sql .= ", description = '" . $this -> description . "'";
			if(isset($this -> category_id)) $sql .= ", category_id = '" . $this -> category_id . "'";
			if(isset($this -> sub_category_id)) $sql .= ", sub_category_id = '" . $this -> sub_category_id . "'";
			if(isset($this -> time_allocated)) $sql .= ", time_allocated = '" . $this -> time_allocated . "'";
			if(isset($this -> importance)) $sql .= ", importance = '" . $this -> importance . "'";
			if(isset($this -> urgency)) $sql .= ", urgency = '" . $this -> urgency . "'";
			if(isset($this -> time_split)) $sql .= ", time_split = '" . $this -> time_split . "'";
			if(isset($this -> time_split_min)) $sql .= ", time_split_min = '" . $this -> time_split_min . "'";
			if(isset($this -> break_time)) $sql .= ", break_time = '" . $this -> break_time . "'";
			if(isset($this -> filling)) $sql .= ", filling = '" . $this -> filling . "'";
			if(isset($this -> due_date)) $sql .= ", due_date = '" . $this -> due_date . "'";
			if(isset($this -> specific_time)) $sql .= ", specific_time = '" . $this -> specific_time . "'";
			if(isset($this -> repeat_start)) $sql .= ", repeat_start = '" . $this -> repeat_start . "'";
			if(isset($this -> repeat_finish)) $sql .= ", repeat_finish = '" . $this -> repeat_finish . "'";
			if(isset($this -> productive)) $sql .= ", productive = '" . $this -> productive . "'";
			if(isset($this -> destination)) $sql .= ", destination = '" . $this -> destination . "'";
			if(isset($this -> allocate_1)) $sql .= ", allocate_1 = '" . $this -> allocate_1 . "'";
			if(isset($this -> allocate_2)) $sql .= ", allocate_2 = '" . $this -> allocate_2 . "'";
			if(isset($this -> is_template)) $sql .= ", is_template = '" . $this -> is_template . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> template_id)) $sql .= ", template_id = '" . $this -> template_id . "'";
			if(isset($this -> status)) $sql .= ", status = '" . $this -> status . "'";
			if(isset($this -> after_hours)) $sql .= ", after_hours = '" . $this -> after_hours . "'";
			if(isset($this -> on_hold)) $sql .= ", on_hold = '" . $this -> on_hold . "'";
			if(isset($this -> x_value)) $sql .= ", x_value = '" . $this -> x_value . "'";
			if(isset($this -> y_value)) $sql .= ", y_value = '" . $this -> y_value . "'";
			if(isset($this -> page_width)) $sql .= ", page_width = '" . $this -> page_width . "'";
			if(isset($this -> page_height)) $sql .= ", page_height = '" . $this -> page_height . "'";
			$sql .= " where task_id = '" . $this -> task_id . "'";
			query_mysql($sql, $conn);
			return $this -> task_id;
		}else{
			$field_sql = "insert tasks(task_id, created, created_by, status ";
			$value_sql = "values (NULL, now(), '" . getSession("g_uid") . "', 'current' ";
			
			//getting page width / height from existing tasks
			$sql = "select page_width, page_height from tasks where page_width != 0 limit 1";
			$result = query_mysql($sql, $conn);
			if( get_total_rows($result) != 0){
				$row = fetch_mysql_data($result);
				$field_sql .= ", page_width, page_height ";
				$value_sql .= ", '" . $row["page_width"] . "', '" . $row["page_height"] . "' ";
			}
			
			if(isset($this -> title)){
				$field_sql .= ", title ";
				$value_sql .= ", '" . $this -> title . "'";
			}
			if(isset($this -> description)){
				$field_sql .= ", description ";
				$value_sql .= ", '" . $this -> description . "'";
			}
			if(isset($this -> category_id)){
				$field_sql .= ", category_id ";
				$value_sql .= ", '" . $this -> category_id . "'";
			}
			if(isset($this -> sub_category_id)){
				$field_sql .= ", sub_category_id ";
				$value_sql .= ", '" . $this -> sub_category_id . "'";
			}
			if(isset($this -> time_allocated)){
				$field_sql .= ", time_allocated ";
				$value_sql .= ", '" . $this -> time_allocated . "'";
			}
			if(isset($this -> importance)){
				$field_sql .= ", importance ";
				$value_sql .= ", '" . $this -> importance . "'";
			}
			if(isset($this -> urgency)){
				$field_sql .= ", urgency ";
				$value_sql .= ", '" . $this -> urgency . "'";
			}
			if(isset($this -> time_split)){
				$field_sql .= ", time_split ";
				$value_sql .= ", '" . $this -> time_split . "'";
			}
			if(isset($this -> time_split_min)){
				$field_sql .= ", time_split_min ";
				$value_sql .= ", '" . $this -> time_split_min . "'";
			}
			if(isset($this -> break_time)){
				$field_sql .= ", break_time ";
				$value_sql .= ", '" . $this -> break_time . "'";
			}
			if(isset($this -> filling)){
				$field_sql .= ", filling ";
				$value_sql .= ", '" . $this -> filling . "'";
			}
			if(isset($this -> due_date)){
				$field_sql .= ", due_date ";
				$value_sql .= ", '" . $this -> due_date . "'";
			}
			if(isset($this -> specific_time)){
				$field_sql .= ", specific_time ";
				$value_sql .= ", '" . $this -> specific_time . "'";
			}
			if(isset($this -> repeat_start)){
				$field_sql .= ", repeat_start ";
				$value_sql .= ", '" . $this -> repeat_start . "'";
			}
			if(isset($this -> repeat_finish)){
				$field_sql .= ", repeat_finish ";
				$value_sql .= ", '" . $this -> repeat_finish . "'";
			}
			if(isset($this -> productive)){
				$field_sql .= ", productive ";
				$value_sql .= ", '" . $this -> productive . "'";
			}
			if(isset($this -> destination)){
				$field_sql .= ", destination ";
				$value_sql .= ", '" . $this -> destination . "'";
			}
			if(isset($this -> allocate_1)){
				$field_sql .= ", allocate_1 ";
				$value_sql .= ", '" . $this -> allocate_1 . "'";
			}
			if(isset($this -> allocate_2)){
				$field_sql .= ", allocate_2 ";
				$value_sql .= ", '" . $this -> allocate_2 . "'";
			}
			if(isset($this -> is_template)){
				$field_sql .= ", is_template ";
				$value_sql .= ", '" . $this -> is_template . "'";
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
			if(isset($this -> template_id)){
				$field_sql .= ", template_id ";
				$value_sql .= ", '" . $this -> template_id . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> status)){
				$field_sql .= ", status ";
				$value_sql .= ", '" . $this -> status . "'";
			}
			if(isset($this -> after_hours)){
				$field_sql .= ", after_hours ";
				$value_sql .= ", '" . $this -> after_hours . "'";
			}
			if(isset($this -> on_hold)){
				$field_sql .= ", on_hold ";
				$value_sql .= ", '" . $this -> on_hold . "'";
			}
			if(isset($this -> x_value)){
				$field_sql .= ", x_value ";
				$value_sql .= ", '" . $this -> x_value . "'";
			}
			if(isset($this -> y_value)){
				$field_sql .= ", y_value ";
				$value_sql .= ", '" . $this -> y_value . "'";
			}
			if(isset($this -> page_width)){
				$field_sql .= ", page_width ";
				$value_sql .= ", '" . $this -> page_width . "'";
			}
			if(isset($this -> page_height)){
				$field_sql .= ", page_height ";
				$value_sql .= ", '" . $this -> page_height . "'";
			}
			$sql = " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_task_preferences{
	public $id, $fieldname, $uid, $active, $section, $required;

	public function getDetail(){
		global $conn;
		$sql = "select task_preferences.* from task_preferences where active = 1";

		if(isset($this -> id)) $sql .= " and id = '" . $this -> id . "'";
		if(isset($this -> fieldname)) $sql .= " and fieldname = '" . $this -> fieldname . "'";
		if(isset($this -> uid)) $sql .= " and uid = '" . $this -> uid . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";
		if(isset($this -> section)) $sql .= " and section = '" . $this -> section . "'";
		if(isset($this -> required)) $sql .= " and required = '" . $this -> required . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update task_preferences set id = id ";
			if(isset($this -> fieldname)) $sql .= ", fieldname = '" . $this -> fieldname . "'";
			if(isset($this -> uid)) $sql .= ", uid = '" . $this -> uid . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			if(isset($this -> section)) $sql .= ", section = '" . $this -> section . "'";
			if(isset($this -> required)) $sql .= ", required = '" . $this -> required . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}else{
			$field_sql = "insert task_preferences(id ";
			$value_sql = "values (NULL ";
			if(isset($this -> fieldname)){
				$field_sql .= ", fieldname ";
				$value_sql .= ", '" . $this -> fieldname . "'";
			}
			if(isset($this -> uid)){
				$field_sql .= ", uid ";
				$value_sql .= ", '" . $this -> uid . "'";
			}
			if(isset($this -> active)){
				$field_sql .= ", active ";
				$value_sql .= ", '" . $this -> active . "'";
			}
			if(isset($this -> section)){
				$field_sql .= ", section ";
				$value_sql .= ", '" . $this -> section . "'";
			}
			if(isset($this -> required)){
				$field_sql .= ", required ";
				$value_sql .= ", '" . $this -> required . "'";
			}
			$sql .= " $field_sql) $value_sql)";
			query_mysql($sql, $conn);
			$result = query_mysql("SELECT LAST_INSERT_ID() as id", $conn);
			$row = fetch_mysql_data($result);
			return $row["id"];
		}
	}
}

class api_task_schedule{
	public $id, $task_id, $schedule_id, $priority, $x_value, $y_value, $active;

	public function getDetail(){
		global $conn;
		$sql = "select task_schedule.* from task_schedule where active = 1";

		if(isset($this -> id)) $sql .= " and id = '" . $this -> id . "'";
		if(isset($this -> task_id)) $sql .= " and task_id = '" . $this -> task_id . "'";
		if(isset($this -> schedule_id)) $sql .= " and schedule_id = '" . $this -> schedule_id . "'";
		if(isset($this -> priority)) $sql .= " and priority = '" . $this -> priority . "'";
		if(isset($this -> x_value)) $sql .= " and x_value = '" . $this -> x_value . "'";
		if(isset($this -> y_value)) $sql .= " and y_value = '" . $this -> y_value . "'";
		if(isset($this -> active)) $sql .= " and active = '" . $this -> active . "'";

		$result = query_mysql($sql, $conn);
		return $result;
	}

	public function updateObject(){
		global $conn;
		if( $this -> id != ''){
			$sql = "update task_schedule set id = id ";
			if(isset($this -> task_id)) $sql .= ", task_id = '" . $this -> task_id . "'";
			if(isset($this -> schedule_id)) $sql .= ", schedule_id = '" . $this -> schedule_id . "'";
			if(isset($this -> priority)) $sql .= ", priority = '" . $this -> priority . "'";
			if(isset($this -> x_value)) $sql .= ", x_value = '" . $this -> x_value . "'";
			if(isset($this -> y_value)) $sql .= ", y_value = '" . $this -> y_value . "'";
			if(isset($this -> active)) $sql .= ", active = '" . $this -> active . "'";
			$sql .= " where id = '" . $this -> id . "'";
			query_mysql($sql, $conn);
			return $this -> id;
		}else{
			$field_sql = "insert task_schedule(id ";
			$value_sql = "values (NULL ";
			if(isset($this -> task_id)){
				$field_sql .= ", task_id ";
				$value_sql .= ", '" . $this -> task_id . "'";
			}
			if(isset($this -> schedule_id)){
				$field_sql .= ", schedule_id ";
				$value_sql .= ", '" . $this -> schedule_id . "'";
			}
			if(isset($this -> priority)){
				$field_sql .= ", priority ";
				$value_sql .= ", '" . $this -> priority . "'";
			}
			if(isset($this -> x_value)){
				$field_sql .= ", x_value ";
				$value_sql .= ", '" . $this -> x_value . "'";
			}
			if(isset($this -> y_value)){
				$field_sql .= ", y_value ";
				$value_sql .= ", '" . $this -> y_value . "'";
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