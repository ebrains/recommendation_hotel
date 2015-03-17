<?php
//key is the id value, value is 
$returnsort["1"]= 1;
$returnsort["32"]= 2;
$returnsort["71"]=3;
$returnsort["21"]=4;
$jsonstr =json_encode($returnsort); 
$url="http://localhost/get.php?value=$jsonstr"; 
$html = file_get_contents($url); 
echo $html; 
?> 

