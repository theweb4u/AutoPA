<?if($template != "no"){?>
<div id="clock" class="clock_css"></div>
<SCRIPT LANGUAGE="JavaScript"> 
  <!-- 
  
  function display() { 
    var Today = new Date(); 
    var hours = Today.getHours(); 
    var min = Today.getMinutes(); 
    var sec = Today.getSeconds(); 
    var Time = ((hours > 12) ? hours - 12 :(hours == 0) ? 12 :hours); 
    Time += ((min < 10) ? ":0" : ":") + min; 
    Time += ((sec < 10) ? ":0" : ":") + sec; 
    Time += (hours >= 12) ? " PM" : " AM"; 
    $('clock').innerHTML = Time; 
    setTimeout("display()",1000); 
  } 

  display(); 

  --> 
</SCRIPT> 
<?}?>