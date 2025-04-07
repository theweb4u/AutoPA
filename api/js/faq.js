	function displayFAQContent(varContent){
		$('faq_content').innerHTML = varContent;
	}
	
	function refreshFAQContent(urlParam){
		ajaxGETRequest("ajax_handler_student.php?mode=faq"+urlParam, 1, displayFAQContent);
	}
	
	function newFAQ(){
		$('faq_content').innerHTML = $('faq_form').innerHTML;
	}
	
	function SaveFAQ(){
		if($('txtfaq').value == ""){
			alert("No FAQ found to be posted, Please try again!!");
		}else{
			ajaxPOSTRequest("ajax_handler_student.php", "mode=faq&faq="+$('txtfaq').value, displayFAQContent);
		}
	}
	
	function myFAQ(urlParam){
		ajaxGETRequest("ajax_handler_student.php?mode=faq&show_user=1"+urlParam, 1, displayFAQContent);
	}