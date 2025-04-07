function complete_task(id){
	ajaxGETRequest("index.php?mode=tasks&template=no&task_id="+id+"&ajaxcall=1&call_mode=update_status", 0, removeTask);
}

function removeTask(details){
	if( details.indexOf("Requested task not available for editing") > 0 ){
		alert("Requested task not available for editing");
	}else{
		window.location.reload();
	}
}

function edit_task(task_id){
	var url = "index.php?mode=tasks&sub_mode=task_edit&task_id="+task_id+"&call_mode=edit&template=no";
	popupcenter(url, 1100, 600);
}

function showNav(filter_name, eventSrc){
	if($(filter_name).style.display == "none"){
		var arr = $(eventSrc).cumulativeOffset();
		$(filter_name).style.position = "absolute";
		$(filter_name).style.left = arr[0] - 300;
		$(filter_name).style.top = arr[1] + 10;
		$(filter_name).show();
	}else{
		$(filter_name).hide();
	}
}

function showModal(url, window_title, window_width){
	Modalbox.show(url, {title: window_title, width: window_width}); 
}

function foldedList(id){
	if($(id).style.display == "none"){
		Effect.BlindDown(id);
	}else{
		Effect.BlindUp(id);
	}
	return false;
}

function UnHoldTask(task_id, mode, submode){
	location.href = "index.php?mode="+mode+"&call_mode=unhold&task_id="+task_id+"&sub_mode="+submode;
}

function holdTask(task_id){
	var pos_arr = Element.cumulativeOffset($("task_hide_span_"+task_id));
	$("task_id").value = task_id;
	$('onhold_params').style.position = 'absolute';
	$('onhold_params').style.left = pos_arr[0];
	$('onhold_params').style.top = pos_arr[1];
	$('onhold_params').show();
}

function getWH(){
	var viewport = document.viewport.getDimensions(); // Gets the viewport as an object literal
	var height = viewport.height; // Usable window height
	return height
}

function getWW(){
	var viewport = document.viewport.getDimensions(); // Gets the viewport as an object literal
	var width = viewport.width; // Usable window width
	return width;
}

function showTaskList(obj, task_list_obj){
	var pos_arr = Element.cumulativeOffset($(obj));
	$(task_list_obj).style.position = 'absolute';
	$(task_list_obj).style.left = Number(pos_arr[0]) - 10;
	$(task_list_obj).style.top = Number(pos_arr[1]) + 10;
	$(task_list_obj).show();
}

function popupcenter(page, width, height) {
	//var midHeight = (document.body.clientHeight /3) + (height/3);
	var midWidth = (document.body.clientWidth /3) - (width/3);
	var properties = "height=" + height + ",width=" + width + ",top=0, left=" + midWidth +",toolbar=no,scrollbars=yes,menubar=no,resizable=yes,status=no,location=no,directories=no";  open(page, "", properties);
}

function ajaxGETRequest(url, hideError, fnName){
	new Ajax.Request(url,
	{
		method:'GET',
		onSuccess: function(transport){
			if(fnName != "") fnName(transport.responseText);
		},
		onFailure: function(transport){ 
			if(hideError == 0){
				alert('Error: ' + transport.responseText);
			}
	}});
}

function ajaxPOSTRequest(url, params, fnName){
	var options = {method:"post",     
		postBody:params,     
		onComplete: function(transport){
			if(fnName != ""){
				fnName(transport.responseText);
			}
		}
	}; 
	
	new Ajax.Request(url,options); 

}