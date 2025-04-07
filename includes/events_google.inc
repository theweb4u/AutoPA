<?
	include("api/users.inc");
	$obj = new api_users;
	$obj -> uid = getSession("g_uid");
	$result = $obj -> getDetail();
	
	if( get_total_rows($result) != 0){
		$row = fetch_mysql_data($result);
		$google_user_id = $row["google_user_id"];
		$google_user_pwd = $row["google_user_pwd"];
	}
	
	if($google_user_id != "" || get_form_values("google_login") == 1){
		if( get_form_values("google_login") == 1 ){
			$google_user_id = get_form_values("username");
			$google_user_pwd = get_form_values("password");
			if( get_form_values("save2user") == "1" ){
				$obj = new api_users;
				$obj -> uid = getSession("g_uid");
				$obj -> google_user_id = filter_value( $google_user_id );
				$obj -> google_user_pwd = filter_value( $google_user_pwd );
				$obj -> modified = date('Y-m-d H:i:s');
				$obj -> updateObject();
			}
		}
		
		// load library
		require_once 'Zend/Loader.php';
		Zend_Loader::loadClass('Zend_Gdata');
		Zend_Loader::loadClass('Zend_Gdata_ClientLogin');
		Zend_Loader::loadClass('Zend_Gdata_Calendar');
		Zend_Loader::loadClass('Zend_Http_Client');
		
		// create authenticated HTTP client for Calendar service
		$gcal = Zend_Gdata_Calendar::AUTH_SERVICE_NAME;
		try {
			$client = Zend_Gdata_ClientLogin::getHttpClient($google_user_id, $google_user_pwd, $gcal);
		} catch (Zend_Gdata_App_Exception $e) {
			$login_error = $e->getResponse();
		}
		echo $login_error;
		if( $login_error == ""){
			$gcal = new Zend_Gdata_Calendar($client);
			
			// generate query to get event list
			$query = $gcal->newEventQuery();
			$query->setUser('default');
			$query->setVisibility('private');
			$query->setProjection('composite');
			$query->setSortOrder('a');
			$query->setOrderby('starttime');   
			$query->setStartMin(date('Y-m-d'));   
			$newdate = strtotime ( $cnfg_event_max_days_google  . ' day' , strtotime ( date('Y-m-d') ) ) ;
			$newdate = date ( 'Y-m-j' , $newdate );
			$query->setStartMax($newdate);
			$query->setParam('singleevents','true'); 
			
			// get and parse calendar feed
			try {
			  $feed = $gcal->getCalendarEventFeed($query);
			} catch (Zend_Gdata_App_Exception $e) {
			  $list_error = $e->getResponse();
			}
		}
	}
?>

<?
	//if not already has a google user
	if($google_user_id == ""){
?>
<form method="post" action="index.php" name="editform">
	<input type="hidden" name="google_login" value="1">
	<input type="hidden" name="mode" value="events">
	<input type="hidden" name="sub_mode" value="events_google">
	
	<table class="tblEdit" width="700px" border="0">
		<tr>
			<td colspan="2" class="editHeader">Please enter you google login...<br><br></td>
		</tr>
		<?if($login_error != ""){?>
		<tr>
			<td colspan="4" align="center"><b><font color="red"><?echo $login_error;?></font></b></td>
		</tr>
		<?}?>
		<tr>
			<td class="frm_caption">Username:</td>
			<td class="frm_input">
				<input type="text" name="username" value="" size="55" class="textbox">
			</td>
		</tr>
		<tr>
			<td class="frm_caption">Password:</td>
			<td class="frm_input">
				<input type="password" name="password" value="" size="55" class="textbox">
			</td>
		</tr>
		<tr>
			<td class="frm_input" colspan="2">
				<input type="submit" class="button" value="Login to Google Calendar" style="width: 250px;">
				<input type="checkbox" value="1" checked name="save2user"> (Check this to save to your profile)
			</td>
		</tr>
	</table>
</form>
<?	
	}else{
		//showing event list from google
?>
<table class="tblDisplay" width="1100px">
	<tr class="header">
		<td>Title</td>
		<td>Description</td>
		<td class="action">Location</td>
		<td class="action">From</td>
		<td class="action">To</td>
		<td class="action">Created</td>
		<td class="action">@</td>
	</tr>
<?
	if($list_error == ""){
	if( $feed->totalResults == "0" ){
?>
	<tr class="noResult">
		<td colspan="7">No Current Events found to display</td>
	</tr>
<?	
	}else{
		$event_obj = new api_events;
		
		$class = "even";
		foreach ($feed as $event) {
			foreach ($event->when as $when) {
				$date = new DateTime($when->startTime);
				$date2 = new DateTime($when->endTime);
				$date_from = $date->format('Y-m-d');
				$time_from = $date->format('H:i:s');
				$time_to = $date2->format('H:i:s');
			}
			$location = $event -> where[0];
			$description = $event -> content;
			$title = $event->title;
			
			if($class = "even") $class = "odd";
			else $class = "even";	
			echo "<tr class=" . $class . ">";
			echo "<td>$title</td>";
			echo "<td>$description</td>";
			echo "<td nowrap>$location</td>";
			echo "<td nowrap>$date_from  $time_from</td>";
			echo "<td>$time_to</td>";
			$event_obj -> google_id = $event -> id;
			$result_event = $event_obj -> getDetail();
			$event_id = 0;
			if( get_total_rows($result_event) != 0){
				$row_event = fetch_mysql_data($result_event);
				$event_id = $row_event["event_id"];
			}
			
			if( get_url_values('google_id') == urldecode($event -> id)){
				//updating the event
				$event_new_obj = new api_events;
				if( $event_id != 0) $event_new_obj -> event_id = $event_id;
				else $event_new_obj -> google_id = $event -> id;
				$event_new_obj -> title = $title;
				$event_new_obj -> event_type = "google";
				$event_new_obj -> description = $description;
				$event_new_obj -> event_date = $date_from;
				if($location != ""){
					$location_obj = new api_types;
					$location_obj -> title = $location;
					$location_obj -> created_by = getSession('g_uid');
					$location_obj -> type = "location";
					$result_location = $location_obj -> getDetail();
					if( get_total_rows($result_location) != 0){
						$row_location = fetch_mysql_data($result_location);
						$event_new_obj -> destination = $row_location["category_id"];
					}else{
						//creating a new location
						$location_obj -> title = filter_value($location);
						$location_obj -> created_by = getSession('g_uid');
						$location_obj -> created = date('Y-m-d H:i:s');
						$location_obj -> type = "location";
						$event_new_obj -> destination = $location_obj -> updateObject();
					}
					
				}
				$event_new_obj -> time_from = $time_from;
				$event_new_obj -> time_to = $time_to;
				$event_id = $event_new_obj -> updateObject();
			}
			
			if( $event_id != 0) echo "<td><img src='images/tick.gif'></td>";
			else echo "<td>-</td>";
			
			if($date_error == 0){
				echo "<td style='text-align:center;'><a href='index.php?mode=events&sub_mode=events_google&google_id=" . $event -> id . "'><img src='images/disk.gif' title='Save Event' border='0'></a></td>";
			}else{
				echo "<td style='text-align:center;'><img src='images/exclaim.gif' title='Unexpected date format'></td>";
			}
			echo "</tr>";
		}
	}
	}else{
?>
	<tr class="noResult">
		<td colspan="7">Error Occurred: <br><?echo $list_error;?></td>
	</tr>
<?	
	}
?>
</table>		
<?
	}
?>