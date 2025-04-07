<?
	$call_mode = get_url_values("call_mode");
	if($call_mode == "hold"){
		//hold request from segment window
		//including api file
		include("api/tasks.inc");
		put_task_on_hold(get_form_values("task_id"), get_form_values("hold_date"))
?>
	<script language="javascript">parent.location.href=parent.location.href;</script>
<?
	}

	//default segment 1
	$segment_condition = " and importance = 1 and urgency = 1";
	if( get_url_values("segment") == 2) $segment_condition = " and importance = 1 and urgency = 0";
	if( get_url_values("segment") == 3) $segment_condition = " and importance = 0 and urgency = 1";
	if( get_url_values("segment") == 4) $segment_condition = " and importance = 0 and urgency = 0";
	if( get_url_values("segment") == "all") $segment_condition = "";
	
	$sql = "select tasks.task_id, tasks.title, left(tasks.title, 8) as display_title, ";
	$sql .= "(select title from types where category_id = tasks.category_id) as category_name, ";
	$sql .= "(select title from types where category_id = tasks.sub_category_id) as sub_category_name, description, filling, ";
	$sql .= "x_value/page_width as xfactor, y_value/page_height as yfactor, ";
	$sql .= "(select colour from types where category_id = tasks.category_id) as category_colour, ";
	$sql .= "(select colour from types where category_id = tasks.sub_category_id) as sub_category_colour ";
	$sql .= "from tasks where active = 1 and status = 'current' $segment_condition and ";
	$sql .= "(tasks.on_hold = '0000-00-00' OR tasks.on_hold < curdate()) and created_by = " . getSession('g_uid');
	$result = query_mysql($sql, $conn);
?>
<div id="documentsize" width="100%" style="cursor:w-resize;font-size:12;text-align:center;" onclick="resizePage();">
	<?
		$task_ids = 0;
		while($row = fetch_mysql_data($result)){
			$color = "";
			$task_ids .= "," . $row["task_id"];
			$task_titles .= "-next-" . $row["title"];
			$task_xfactors .= "," . $row["xfactor"];
			$task_yfactors .= "," . $row["yfactor"];
			if($row["sub_category_colour"] != "") $color = $row["sub_category_colour"];
			else $color = $row["category_colour"];
			
			$css = "white-space:nowrap;border:1px solid #000000;position:absolute;display:none;text-align:center;";
			if($color != "") $css .= "background-color:$color;";
	?>
		<span id="task_<?echo $row["task_id"];?>" style="<?echo $css;?>">
		<span id="task_title_<?echo $row["task_id"];?>"></span>
		<a href="#" class="tooltip">&raquo;<span style="text-align:left;z-index:100;">
		<table id="outer_table_<?echo $row["task_id"];?>">
		<tr><td nowrap><b>Task ID: </b></td><td><?echo $row["task_id"];?></td></tr>
		<tr><td><b>Title: </b></td><td nowrap><?echo $row["title"];?></td></tr>
		<?if($row["filing"] != ""){?><tr><td nowrap><b>Filing: </b></td><td nowrap><?echo $row["filing"];?></td></tr><?}?>
		<?if($row["description"] != ""){?>
			<tr><td colspan="2"><b>Description: </b></td></tr>
			<tr><td colspan="2"><?echo $row["description"];?></td></tr>
		<?}?>
		<tr>
			<td colspan="2" nowrap>
				<b><a href="#" onclick="complete_task(<?echo $row["task_id"];?>)">Click to complete</a></b>
				&nbsp;[<img src="images/edit.gif" title="Click to Edit Task" border="0" style="cursor:hand;cursor:pointer;" onclick="resize = 0;edit_task(<?echo $row["task_id"];?>);">]
			</td>
		</tr>
		</table>
		</span>
		</a>
		&nbsp;[<a href='#' onclick='javascript:holdTask("<?echo $row["task_id"];?>")' title='Click to put task on hold'><span id='task_hide_span_<?echo $row["task_id"];?>'>H</span></a>]
		</span>
		<script language="javascript">
		new Draggable('task_<?echo $row["task_id"];?>', { handle: 'handle', onEnd: function(){
			dropSpan(<?echo $row["task_id"];?>);
			} 
			});
		</script>
	<?
		}
	?>
</div>

<script language="javascript">
	function dropSpan(task_id){
		resize = 0;
		var arr = $('task_' + task_id).cumulativeOffset();
		
		//copying the span to avoid description hiding behind other tasks after movement.
		var attr = {
				style: "white-space:nowrap;border:1px solid #000000;position:absolute;text-align:center;"
			}
		var spanElement = new Element('span', attr);
		spanElement.style.backgroundColor = $('task_' + task_id).style.backgroundColor;
		spanElement.id = 'task_' + task_id;
		spanElement.style.top = arr[1];
		spanElement.style.left = arr[0];
		spanElement.style.width = $('task_' + task_id).style.width;
		spanElement.innerHTML = $('task_' + task_id).innerHTML;
		$('task_' + task_id).remove();
		document.body.insert(spanElement);
		new Draggable('task_'+task_id, { handle: 'handle', onEnd: function(){
		dropSpan(task_id);
		} 
		});
		
		ajaxGETRequest("ajax.php?task_id="+task_id+"&x_value="+arr[0]+"&y_value="+arr[1]+"&page_width="+page_width+"&page_height="+height, 1, '');
	}
	
	var resize = 1;
	var task_ids = "<?echo $task_ids;?>";
	var task_titles = "<?echo $task_titles;?>";
	var task_xfactors = "<?echo $task_xfactors;?>";
	var task_yfactors = "<?echo $task_yfactors;?>";
	var task_id_arr = task_ids.split(",");
	var task_title_arr = task_titles.split("-next-");
	var task_xfactor_arr = task_xfactors.split(",");
	var task_yfactor_arr = task_yfactors.split(",");
	var newTitle;
	var strip_counter;
	var font_size = 12;
	var width = getWW();
	var page_width = width;
	width = Math.floor( (width*15) / 100);
	var width_orginal = width;
	var height = getWH();
	var position_set = false;
	if( task_ids != 0 ){
		reGroupTasks();
	}
	
	function reGroupTasks(){
		for( var counter = 1; counter <= task_id_arr.length - 1; counter++ ){
			$("task_" + task_id_arr[counter]).style.width = "";
			if( position_set == false ){
				$("task_" + task_id_arr[counter]).style.left = Math.floor(task_xfactor_arr[counter] * page_width);
				$("task_" + task_id_arr[counter]).style.top = Math.floor(task_yfactor_arr[counter] * height);
			}
			$("task_" + task_id_arr[counter]).style.fontSize = font_size + "px";
			$("task_title_" + task_id_arr[counter]).innerHTML = task_title_arr[counter];
			if( $("task_" + task_id_arr[counter]).getWidth() <= width ){
				$("task_" + task_id_arr[counter]).style.width = width;
			}else{
				strip_counter = 1;
				while( $("task_" + task_id_arr[counter]).getWidth() > width ){
					newTitle = $("task_title_" + task_id_arr[counter]).textContent;
					newTitle = newTitle.replace("...", "");
					newTitle = (strip_counter == 1)? newTitle.substring(0, Number(newTitle.length) - 4) : newTitle.substring(0, Number(newTitle.length) - 1);
					$("task_title_" + task_id_arr[counter]).innerHTML = newTitle + "...";
					strip_counter++;
				}
			}
			$("task_" + task_id_arr[counter]).show();
		}
		position_set = true;
	}
	
	function resizePage(){
		if(resize == 1){
			if(parent.document.frmresize.resize[0].checked){
				width = Number(width) + 10;
				if( font_size < 20 ) font_size = Number(font_size) + 1;
			}else{
				if( width > width_orginal ) width = Number(width) - 10;
				if( font_size > 12 ) font_size = Number(font_size) - 1;
			}
			reGroupTasks();
		}
		resize = 1;
	}
</script>
<?show_hold_option();?>