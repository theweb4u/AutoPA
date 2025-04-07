<?
	$view_type = "U r g e n t ";
	if( $sub_mode == "urgent" ) $view_type = "I m p o r t a n t";
?>
<?
	function task_list($segment_condition){
		global $conn;
		global $task_ids;
		global $task_titles;
		global $task_factors;
		global $sub_mode;
		$sql = "select tasks.task_id, tasks.title, left(tasks.title, 8) as display_title, ";
		$sql .= "(select title from types where category_id = tasks.category_id) as category_name, ";
		$sql .= "(select title from types where category_id = tasks.sub_category_id) as sub_category_name, description, filling, ";
		$sql .= "x_value/page_width as xfactor, y_value/page_height as yfactor, ";
		$sql .= "(select colour from types where category_id = tasks.category_id) as category_colour, ";
		$sql .= "(select colour from types where category_id = tasks.sub_category_id) as sub_category_colour ";
		$sql .= "from tasks where active = 1 and status = 'current' $segment_condition and ";
		$sql .= "(tasks.on_hold = '0000-00-00' OR tasks.on_hold < curdate()) and created_by = " . getSession('g_uid');
		$result = query_mysql($sql, $conn);

		while($row = fetch_mysql_data($result)){
			$color = "";
			$task_ids .= "," . $row["task_id"];
			$task_titles .= "-next-" . $row["title"];
			if( $sub_mode == "urgent" ){
				$task_factors .= "," . $row["yfactor"];
			}else{
				$task_factors .= "," . $row["xfactor"];
			}
			if($row["sub_category_colour"] != "") $color = $row["sub_category_colour"];
			else $color = $row["category_colour"];
			
			$css = "white-space:nowrap;border:1px solid #000000;position:absolute;display:none;";
			if($color != "") $css .= "background-color:$color;";
?>
		<span id="task_<?echo $row["task_id"];?>" style="<?echo $css;?>">
		<span id="task_title_<?echo $row["task_id"];?>"></span>
		<a href="#" class="tooltip">&raquo;<span style="text-align:left;z-index:100;">
		<table>
		<tr><td nowrap style="text-align: left;"><b>Task ID: </b></td><td style="text-align: left;"><?echo $row["task_id"];?></td></tr>
		<tr><td style="text-align: left;"><b>Title: </b></td><td nowrap style="text-align: left;"><?echo $row["title"];?></td></tr>
		<?if($row["filing"] != ""){?><tr><td nowrap><b>Filing: </b></td><td nowrap><?echo $row["filing"];?></td></tr><?}?>
		<?if($row["description"] != ""){?>
			<tr><td colspan="2" style="text-align: left;"><b>Description: </b></td></tr>
			<tr><td colspan="2" style="text-align: left;"><?echo $row["description"];?></td></tr>
		<?}?>
		<tr>
			<td colspan="2" style="text-align: left;" nowrap>
				<b><a href="#" onclick="complete_task(<?echo $row["task_id"];?>)">Click to complete</a></b>
				[<img src="images/edit.gif" title="Click to Edit Task" border="0" style="cursor:hand;cursor:pointer;" onclick="resize = 0;edit_task(<?echo $row["task_id"];?>);">]
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
	}
	$task_ids = 0;
	$fieldname = "importance";
	if( $sub_mode == "urgent" ) $fieldname = "urgency";
?>

<table class="tblDisplay" width="800px">
	<tr class="header">
		<td colspan='10'>
		<input type="checkbox" name="multiple" id="multiple" value="1"> Check to move multiple tasks (<small>All tasks after the moved tasks would be moved automatically</small>)
		</td>
	</tr>
	<tr class="header" id="tblRow">
		<td width="1px"> <?echo $view_type;?> <br><br>1 &darr; 2</td>
		<td>
			<div id="div_a_1" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 1');?></div>
			<div id="div_a_2" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 2');?></div>
		</td>
		<td width="1px"> <?echo $view_type;?> <br><br>3 &darr; 4</td>
		<td>
			<div id="div_b_1" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 3');?></div>
			<div id="div_b_2" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 4');?></div>
		</td>
		<td width="1px"> <?echo $view_type;?> <br><br>5 &darr; 6</td>
		<td>
			<div id="div_c_1" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 5');?></div>
			<div id="div_c_2" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 6');?></div>
		</td>
		<td width="1px"> <?echo $view_type;?> <br><br>7 &darr; 8</td>
		<td>
			<div id="div_d_1" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 7');?></div>
			<div id="div_d_2" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 8');?></div>
		</td>
		<td width="1px"> <?echo $view_type;?> <br><br>9 &darr; 1 0</td>
		<td>
			<div id="div_e_1" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 9');?></div>
			<div id="div_e_2" style="border: 1px solid grey;"><?task_list(' and ' . $fieldname . ' = 10');?></div>
		</td>
	</tr>
</table>
<script language="javascript">
	function dropSpan(task_id){
		var current_col = "e";
		current_col_count = 5;
		var y_pos;
		
		var arr = $('task_' + task_id).cumulativeOffset();
		
		//aligning the left to one of the col. (col where the left of obj is placed)
		div_ids = "e,d,c,b";
		div_ids = div_ids.split(",");
		for( var counter=0; counter < div_ids.length; counter++ ){
			pos_arr = Element.cumulativeOffset($("div_"+div_ids[counter]+"_1"));
			if( arr[0] < pos_arr[0] ){
				current_col = (counter == (div_ids.length-1))? "a" : div_ids[counter+1];
				current_col_count = 5 - Number(counter+1);
			}
		}
		pos_arr = Element.cumulativeOffset($("div_"+current_col+"_1"));
		$('task_' + task_id).style.left = pos_arr[0];
		
		//calculating the relative y factor ( 2x(currentcol - 1) x height + distance from the top div )
		y_pos = ((current_col_count - 1)*2) * height;
		parentDiv = $("div_"+current_col+"_1");
		pos_arr = Element.cumulativeOffset( parentDiv );
		y_pos = Number(y_pos) + Number( Number(arr[1]) - Number(pos_arr[1]) );
		
		//copying the span to avoid description hiding behind other tasks after movement.
		pos_arr = Element.cumulativeOffset($('task_' + task_id));
		var attr = {
				style: "white-space:nowrap;border:1px solid #000000;position:absolute;"
			}
		var spanElement = new Element('span', attr);
		spanElement.style.backgroundColor = $('task_' + task_id).style.backgroundColor;
		spanElement.id = 'task_' + task_id;
		spanElement.style.top = pos_arr[1];
		spanElement.style.left = pos_arr[0];
		spanElement.style.width = $('task_' + task_id).style.width;
		spanElement.innerHTML = $('task_' + task_id).innerHTML;
		$('task_' + task_id).remove();
		parentDiv.insert(spanElement);
		new Draggable('task_'+task_id, { handle: 'handle', onEnd: function(){
		dropSpan(task_id);
		} 
		});
		ajaxGETRequest("ajax.php?task_id="+task_id+"&pos="+y_pos+"&sub_mode=<?echo $sub_mode;?>&page_height="+(height*10)+"&multiple="+$("multiple").checked, 1, '');
		if( $("multiple").checked ) setTimeout("location.href = 'index.php?mode=view&sub_mode=<?echo $sub_mode;?>';",100);
	}
	
	var height = getWH();
	var page_height = height;
	height = height - 160; 
	height = Math.floor( height / 2 );
	var width = getWW();
	width = Math.floor( (Number(width) - 130) / 5 );
	
	var div_ids = "a,b,c,d,e";
	div_ids = div_ids.split(",");
	for( var counter=0; counter < div_ids.length; counter++ ){
		for( var counter2=1; counter2 < 3; counter2++ ){
			$('div_' + div_ids[counter] + '_' + counter2).style.height = height;
			$('div_' + div_ids[counter] + '_' + counter2).style.width = width;
		}
	}
	
	var task_ids = "<?echo $task_ids;?>";
	var task_titles = "<?echo $task_titles;?>";
	var task_factors = "<?echo $task_factors;?>";
	var task_id_arr = task_ids.split(",");
	var task_title_arr = task_titles.split("-next-");
	var task_factor_arr = task_factors.split(",");
	var newTitle;
	var strip_counter;
	var font_size = 12;
	var parentDiv;
	var pos_arr;
	var current_col_count;
	var current_div_count;
	var temp_arr;
	var top_pos;
	
	for( var counter = 1; counter < task_id_arr.length; counter++ ){
		parentDiv = $("task_" + task_id_arr[counter]).up(0);
		pos_arr = Element.cumulativeOffset(parentDiv);
		$("task_" + task_id_arr[counter]).style.width = "";
		$("task_" + task_id_arr[counter]).style.left = pos_arr[0];
		
		//calculating the top => [top div top pos] + [factor_value x (heightx10)] - [2x(current col - 1) * height] - [current div count - 1 x height]
		for( var counter2 = 0; counter2 < div_ids.length; counter2++ ){
			temp_arr = parentDiv.id.split("_");
			if( div_ids[counter2] == temp_arr[1] ){
				current_col_count = counter2;
				current_div_count = temp_arr[2];
			}
		}
		top_pos = Number(pos_arr[1]) + Number( Number( Math.ceil(task_factor_arr[counter] * (height*10)) ) - Number( (current_col_count*2)*height ) );
		top_pos = top_pos - (height * (current_div_count-1));
		$("task_" + task_id_arr[counter]).style.top = top_pos;
		
		$("task_" + task_id_arr[counter]).style.fontSize = font_size + "px";
		$("task_title_" + task_id_arr[counter]).innerHTML = task_title_arr[counter];
		if( $("task_" + task_id_arr[counter]).getWidth() <= Number(width) - 10 ){
			$("task_" + task_id_arr[counter]).style.width = Number(width) - 10;
		}else{
			strip_counter = 1;
			while( $("task_" + task_id_arr[counter]).getWidth() > Number(width) - 10 ){
				newTitle = $("task_title_" + task_id_arr[counter]).textContent;
				newTitle = newTitle.replace("...", "");
				newTitle = (strip_counter == 1)? newTitle.substring(0, Number(newTitle.length) - 4) : newTitle.substring(0, Number(newTitle.length) - 1);
				$("task_title_" + task_id_arr[counter]).innerHTML = newTitle + "...";
				strip_counter++;
			}
		}
		$("task_" + task_id_arr[counter]).show();
	}
</script>
<?show_hold_option();?>