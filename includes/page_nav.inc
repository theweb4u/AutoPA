<?if($template != "no"){?>
<table width="100%">
	<tr>
		<td align="left">
			<span class="logoText1">Time</span> <span class="logoText2">Managment</span>
		</td>
		<td align="right">
			<?
			if( getSession("g_uid") != ""){
				if( getSession("g_welcome_msg") != "" ){
					echo getSession("g_welcome_msg");
					setSession("g_welcome_msg", "");
				}else{
					echo "<b>Logged On Since: " . getSession("g_logged_on") . "</b>";
				}
			?>
			<span class="note"><br><a href="index.php?mode=logout">Sign Out</a></span>
			<?
			}
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2"><hr class="nav_bar"></td>
	</tr>
</table>
<?if( getSession("g_uid") != ""){?>
<?
	$colspan = 15;
	if( getSession("g_is_admin") == 1 ) $colspan = 16;
?>
<table width="100%">
	<tr>
		<td class="<?if($mode == "tasks"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "tasks"){?>onclick="location.href='index.php?mode=tasks'"<?}?>>Task List</td>
		<td class="page_nav_sep">&nbsp;</td>
		<td class="<?if($mode == "events"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "events"){?>onclick="location.href='index.php?mode=events'"<?}?>>Events</td>
		<td class="page_nav_sep">&nbsp;</td>
		<td class="<?if($mode == "view"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "view"){?>onclick="location.href='index.php?mode=view'"<?}?>>View</td>
		<td class="page_nav_sep">&nbsp;</td>
		<td class="<?if($mode == "schedule"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "schedule"){?>onclick="location.href='index.php?mode=schedule'"<?}?>>Schedule</td>
		<td class="page_nav_sep">&nbsp;</td>
		<td class="<?if($mode == "calendar"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "calendar"){?>onclick="location.href='index.php?mode=calendar'"<?}?>>Calendar</td>
		<td class="page_nav_sep">&nbsp;</td>
		<td class="<?if($mode == "preferences"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "preferences"){?>onclick="location.href='index.php?mode=preferences&sub_mode=change_password'"<?}?>>Preferences</td>
		<td class="page_nav_sep">&nbsp;</td>
		<?if( getSession("g_is_admin") == 1 ){?>
		<td class="<?if($mode == "admin"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "admin"){?>onclick="location.href='index.php?mode=admin'"<?}?>>Admin</td>
		<?}?>
		<td>&nbsp;</td>
		<td class="<?if($mode == "help"){echo "page_nav_active";}else{echo "page_nav";}?>" <?if($mode != "help"){?>onclick="location.href='index.php?mode=help'"<?}?>>Help?</td>
	</tr>
	
	<?if($mode == "preferences"){?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=preferences&sub_mode=update_profile" <?if($sub_mode == "update_profile"){?>style="text-decoration:underline;"<?}?>>Update Profile</a> | 
				<a href="index.php?mode=preferences&sub_mode=change_password" <?if($sub_mode == "change_password"){?>style="text-decoration:underline;"<?}?>>Change Password</a> | 
				<!--<a href="index.php?mode=preferences&sub_mode=display_pref" <?if($sub_mode == "display_pref"){?>style="text-decoration:underline;"<?}?>>Display</a> | -->
				<a href="index.php?mode=preferences&sub_mode=schedule_pref" <?if($sub_mode == "schedule_pref"){?>style="text-decoration:underline;"<?}?>>Schedule</a> | 
				<a href="index.php?mode=preferences&sub_mode=category" <?if($sub_mode == "category"){?>style="text-decoration:underline;"<?}?>>Category</a> | 
				<a href="index.php?mode=preferences&sub_mode=location" <?if($sub_mode == "location"){?>style="text-decoration:underline;"<?}?>>Location</a>
			</td>
		</tr>
	<?}?>
	
	<?if($mode == "tasks"){?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=tasks" <?if($sub_mode == ""){?>style="text-decoration:underline;"<?}?>>Task List</a> | 
				<a href="index.php?mode=tasks&sub_mode=task_edit" <?if($sub_mode == "task_edit"){?>style="text-decoration:underline;"<?}?>>New Task</a> | 
				<a href="index.php?mode=tasks&sub_mode=task_from_template" <?if($sub_mode == "task_from_template"){?>style="text-decoration:underline;"<?}?>>New Task From Template</a>  
				<span id="csv_option" style="display:none;"> | <a href="csv/<?echo getSession("g_uid")?>_tasks_list.csv" target="_blank" title="Download current task list">Download Task List</a></span> 
			</td>
		</tr>
	<?}?>
	
	<?if($mode == "schedule"){?>
	<?
		$call_mode = get_form_values("call_mode");
		if($call_mode == "") $call_mode = get_url_values("call_mode");
	?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=schedule" <?if($sub_mode == "" && $call_mode != "new"){?>style="text-decoration:underline;"<?}?>>Schedule List</a> | 
				<a href="index.php?mode=schedule&call_mode=new" <?if($call_mode == "new"){?>style="text-decoration:underline;"<?}?>>New Schedule</a>
			</td>
		</tr>
	<?}?>
	
	<?if($mode == "events"){?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=events" <?if($sub_mode == ""){?>style="text-decoration:underline;"<?}?>>Event List</a> | 
				<a href="index.php?mode=events&sub_mode=event_edit" <?if($sub_mode == "event_edit"){?>style="text-decoration:underline;"<?}?>>New Event</a> | 
				<a href="index.php?mode=events&sub_mode=events_google" <?if($sub_mode == "events_google"){?>style="text-decoration:underline;"<?}?>>Events From Google</a> | 
				<a href="index.php?mode=events&call_mode=delete_bulk_events" onclick="return confirm('You are about to delete all events before now.  Continue?')">Delete Finished Events</a> | 
			</td>
		</tr>
	<?}?>
	
	<?if($mode == "admin"){?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=admin" <?if($sub_mode == ""){?>style="text-decoration:underline;"<?}?>>User List</a> | 
				<a href="index.php?mode=admin&sub_mode=user_edit" <?if($sub_mode == "user_edit"){?>style="text-decoration:underline;"<?}?>>New User</a>
			</td>
		</tr>
	<?}?>
	
	<?if($mode == "view" &&  $cnfg_urgent_important_measured_by == "numbers"){?>
		<tr>
			<td colspan="<?echo $colspan;?>" style="padding-top: 4px;" class="subLinks">
				<a href="index.php?mode=view" <?if($sub_mode == ""){?>style="text-decoration:underline;"<?}?>>Complete View</a> | 
				<a href="index.php?mode=view&sub_mode=important" <?if($sub_mode == "important"){?>style="text-decoration:underline;"<?}?>>Urgent View</a> | 
				<a href="index.php?mode=view&sub_mode=urgent" <?if($sub_mode == "urgent"){?>style="text-decoration:underline;"<?}?>>Important View</a>
				<span id="on_hold_task_list_span" style="display:none;"> | <a href="#" onclick="showTaskList('on_hold_task_list_span', 'task_list_on_hold');">Show Tasks on Hold</a></span>
			</td>
		</tr>
	<?}?>
</table>
<?}?>
<?}?>