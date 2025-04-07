			function remove_cat_selection(){
				//function removes previous selection in case of radio buttons
				if(document.frmlisting.chk_category.length){
					for(counter=0;counter<document.frmlisting.chk_category.length;counter++){
						if(!document.frmlisting.chk_category[counter].checked){
							show_selected(document.frmlisting.chk_category[counter].value, "", false);
						}
					}
				}
			}
			
			function show_selected(cat_id, cat_name, isChecked){
				var current_category;
				var current_category_id;
				var cat_arr;
				var cat_id_arr;
				var counter;
				var new_category = "";
				var new_category_id = "";
				
				if(isChecked){
					if(document.frmlisting.category.value == ""){
						new_category = cat_name;
						new_category_id = cat_id;
					}else{
						new_category += document.frmlisting.category_names.value + ", " + cat_name;
						new_category_id = document.frmlisting.category.value + "," + cat_id;
					}
				}else{
					current_category = ","+document.frmlisting.category_names.value+",";
					current_category_id = ","+document.frmlisting.category.value+",";
					cat_arr = current_category.split(",");
					cat_id_arr = current_category_id.split(",");
					
					for(counter=0;counter<cat_arr.length;counter++){
						if(cat_id_arr[counter] != ""){
							if(cat_id_arr[counter] != cat_id){
								if(new_category == ""){
									new_category = cat_arr[counter];
									new_category_id = cat_id_arr[counter];
								}else{
									new_category += "," +  cat_arr[counter];;
									new_category_id += "," + cat_id_arr[counter];
								}
							}
						}
					}
				}
				
				document.frmlisting.category.value = new_category_id;
				document.frmlisting.category_names.value = new_category;
				if(new_category != "") document.getElementById("cat_list").innerHTML = "<b>Current Selection:</b> " + new_category;
				else document.getElementById("cat_list").innerHTML = "";
			}
			
			function show_sub(top_category, next_category){
				//getting the sub category for the requested category
				var urlstr;
				var selected_category = document.frmlisting.category.value;
				urlstr = "ineed_cat_list.php?top_category="+top_category+"&next_category="+next_category+"&selected_category="+selected_category;
				process_url(urlstr);
			}
			
			function stateChanged(){ 
				//process_url calls this function as a response action from ajax call name of function is fixed
				if (xmlHttp.readyState==4 || xmlHttp.readyState=="complete"){ 
					if (xmlHttp.responseText != ""){
						document.getElementById("category_list").innerHTML = xmlHttp.responseText;
					}else{
						alert("Request Failed");
					}
				} 
			}
			
			function advert_clicked(advert_id){
				xmlHttp=GetXmlHttpObject()
				if (xmlHttp==null){
					return;
				}
	
				xmlHttp.open("GET","advert_clicked.php?advert_id="+advert_id,true)
				xmlHttp.send(null)
			}