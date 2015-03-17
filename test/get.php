<?php
$x=json_decode(stripslashes($_GET["returnsort"]),true);
foreach($x as $key => $v)
{
 
     echo "$key";
}
?>
