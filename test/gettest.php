<?php
//key is the id value, value is the sort order
$returnsort["1"]= 1;
$returnsort["32"]= 2;
$returnsort["71"]=3;

$jsonstr =json_encode($returnsort); 
$url="http://localhost/get.php?value=$jsonstr"; 
$html = file_get_contents($url); 
echo $html; 
?> 

