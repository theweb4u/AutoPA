<?
	//including api file
	include("api/tasks.inc");
	
	$call_mode = get_url_values("call_mode");
	if($call_mode == "unhold"){
		//unhold request
		$obj = new api_tasks;
		$obj -> task_id = filter_value( get_url_values("task_id") );
		$obj -> created_by = getSession('g_uid');
		$result = $obj -> getDetail();
		if( get_total_rows($result) != 0){
			$obj -> on_hold = 'null';
			$obj -> updateObject();
		}else{
			$error = "Requested task not available for editing";
		}
	}
	
	if($call_mode == "hold"){
		//hold request from alternate view
		put_task_on_hold(get_form_values("task_id"), get_form_values("hold_date"));
	}
	
	$obj = new api_tasks;
	$obj -> created_by = getSession('g_uid');
	$obj -> active = 1;
	$obj -> order_by = "title";
	$obj -> task_type = "on_hold";
	$result = $obj -> getDetail();
	if( get_total_rows($result) != 0){
		$show_hold_option = 1;
		$html = '<table id="task_list_on_hold" class="tblDisplay" style="display:none;z-index:1002" width="300px">';
		$html .= '<tr class="header"><td><a href="#" onclick="$(\'task_list_on_hold\').hide();">Close</a></td></tr>';
		while($row = fetch_mysql_data($result)){
			$html .= '<tr class="odd"><td><a href="#" onclick="javascript:UnHoldTask(' . $row["task_id"] . ', \'view\', \'' . $sub_mode . '\')" ';
			$html .= 'title="Task on hold till ' . $row["on_hold_formatted"] . '.  Click to release">' . $row["display_title"];
			$html .= ' [ ' . $row["task_id"] . ' ] ' . '</a></td></tr>';
		}
		$html .= "</table>";
	}
	echo $html;
	
if( $sub_mode == "" ){	
?>
<table class="tblDisplay" width="100%">
	<tr class="header">
		<td colspan="2">
			<form style="display:inline" name="frmresize">
				Resize Controller: 
				<input type="radio" name="resize" value="+">+ OR 
				<input type="radio" name="resize" value="-" checked>-
			</form>
		</td>
	</tr>
	
	<?if( $cnfg_urgent_important_measured_by == "numbers" ){?>
		<tr class="header">
			<td colspan="2" title="Important">(<a href="#" title="Add New Task" onclick="javascript:popupcenter('index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit', 900, 500)">+</a>) Important &darr;</td>
		</tr>
		<tr class="header">
			<td width="1px">U r g e n t &rarr;</td>
			<td>
				<iframe id="frm_segment" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="index.php?mode=view_segment&segment=all&template=no"></iframe>
			</td>
		</tr>
	<?}else{?>
		<?if( get_url_values("zoom_segment") != ""){?>
			<?
				$current_segment = get_url_values("zoom_segment");
				switch ($current_segment) {
					case 2:
						$title = "Segment 2";
						$caption = "Not Urgent but Important";
						$add_task_link = "index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&important=1&zoom_segment=2";
						$frame_source = "index.php?mode=view_segment&segment=2&template=no";
						break;
					case 3:
						$title = "Segment 3";
						$caption = "Urgent but Not Important";
						$add_task_link = "index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&urgent=1&zoom_segment=3";
						$frame_source = "index.php?mode=view_segment&segment=3&template=no";
						break;
					case 4:
						$title = "Segment 4";
						$caption = "Not Urgent and Not Important";
						$add_task_link = "index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&zoom_segment=4";
						$frame_source = "index.php?mode=view_segment&segment=4&template=no";
						break;
					default:
						// default is segment 1
						$title = "Segment 1";
						$caption = "Urgent and Important";
						$add_task_link = "index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&urgent=1&important=1&zoom_segment=1";
						$frame_source = "index.php?mode=view_segment&segment=1&template=no";
				}
			?>
			<tr class="header">
				<td colspan="2" title="<?echo $title;?>"><?echo $caption;?> (<a href="#" title="Add New Task" onclick="javascript:popupcenter('<?echo $add_task_link;?>', 900, 500)">+</a>) [<a href="index.php?mode=view">Back</a>]</td>
			</tr>
			<tr>
				<td colspan="2">
					<iframe id="frm_segment" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="<?echo $frame_source;?>"></iframe>
				</td>
			</tr>
		<?}else{?>
		<tr class="header">
			<td width="50%" title="Segment 1">Urgent and Important (<a href="#" title="Add New Task" onclick="javascript:popupcenter('index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&urgent=1&important=1', 900, 500)">+</a>) [<a href="index.php?mode=view&zoom_segment=1">Zoom</a>]</td>
			<td width="50%" title="Segment 2">Not Urgent but Important (<a href="#" title="Add New Task" onclick="javascript:popupcenter('index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&important=1', 900, 500)">+</a>) [<a href="index.php?mode=view&zoom_segment=2">Zoom</a>]</td>
		</tr>
		<tr>
			<td>
				<iframe id="frm_segment1" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="index.php?mode=view_segment&segment=1&template=no"></iframe>
			</td>
			<td>
				<iframe id="frm_segment2" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="index.php?mode=view_segment&segment=2&template=no"></iframe>
			</td>
		</tr>
		<tr class="header">
			<td width="50%" title="Segment 3">Urgent but Not Important (<a href="#" title="Add New Task" onclick="javascript:popupcenter('index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit&urgent=1', 900, 500)">+</a>) [<a href="index.php?mode=view&zoom_segment=3">Zoom</a>]</td>
			<td width="50%" title="Segment 4">Not Urgent and Not Important (<a href="#" title="Add New Task" onclick="javascript:popupcenter('index.php?call_mode=from_view&template=no&mode=tasks&sub_mode=task_edit', 900, 500)">+</a>) [<a href="index.php?mode=view&zoom_segment=4">Zoom</a>]</td>
		</tr>
		<tr>
			<td>
				<iframe id="frm_segment3" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="index.php?mode=view_segment&segment=3&template=no"></iframe>
			</td>
			<td>
				<iframe id="frm_segment4" width="100%" frameborder="1" scrolling="yes" marginwidth="0" marginheight="0" src="index.php?mode=view_segment&segment=4&template=no"></iframe>
			</td>
		</tr>
		<?}?>
	<?}?>
</table>

<script language="javascript">
var height = getWH();

<?if( get_url_values("zoom_segment") != "" ){?>
height = height - 190;
$('frm_segment').style.height = height;
<?}else if( $cnfg_urgent_important_measured_by == "numbers" ){?>
height = height - 210;
$('frm_segment').style.height = height;
<?}else{?>
height = height - 30;
height = Math.floor( (Number(height) - 200) / 2 );
$('frm_segment1').style.height = height;
$('frm_segment2').style.height = height;
$('frm_segment3').style.height = height;
$('frm_segment4').style.height = height;
<?}?>
</script>

<?
}else{
	include("includes/alternate_view.inc");
}
?>

<?if( $show_hold_option == 1 ){?>
<script language="javascript">$("on_hold_task_list_span").show();</script>
<?}?>