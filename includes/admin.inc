<?
if( getSession("g_is_admin") == 1 ){
	//including api file
	if( !class_exists("api_allowed_user_groups") ) include("api/users.inc");

	if($sub_mode == "delete"){
		$obj = new api_users;
		$obj -> uid = filter_value( get_url_values("user_id") );
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			$obj -> active = 0;
			$obj -> updateObject();
		}else{
			$error = "Requested user not available for editing";
		}
	}else if($sub_mode == "user_edit"){
		include("includes/registration.inc");
	}else{
?>
<table class="tblDisplay" width="800px">
	<tr class="header">
		<td class="action">@</td>
		<td class="action">Created</td>
		<td>Name</td>
		<td class="action">Username</td>
		<td class="action">Email</td>
		<td class="action">Status</td>
		<td class="action">@</td>
	</tr>
	<?
	$obj = new api_users;
	$obj -> active = 1;
	$result = $obj -> getDetail();
	if( get_total_rows($result) != 0){
		$class = "even";
		while($row = fetch_mysql_data($result)){
			if($class == "even") $class = "odd";
			else $class = "even";	
	?>
	<tr class="<?echo $class;?>">
		<td>
			<a href="index.php?mode=admin&sub_mode=user_edit&user_id=<?echo $row["uid"];?>&call_mode=edit">
				<img src="images/edit.gif" title="Edit Users" border="0"></a>
		</td>
		<td><?echo $row["created_formatted"];?></td>
		<td><?echo $row["lastname"] . ", " . $row["firstname"];?></td>
		<td><?echo $row["username"];?></td>
		<td nowrap><?echo $row["email"];?></td>
		<td nowrap><?echo $row["status"];?></td>
		<td>
			<a href="index.php?mode=admin&user_id=<?echo $row["uid"];?>&sub_mode=delete" onclick="return confirm('Are you sure to delete this User?');">
				<img src="images/trash.gif" title="Delete Events" border="0"></a>
		</td>
	</tr>
	<?
		}
	}else{
	?>
	<tr class="noResult">
		<td colspan="8">No Users found to display</td>
	</tr>
	<?}?>
</table>
<?
	}
}
?>