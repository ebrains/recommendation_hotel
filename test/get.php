<?php
$x=json_decode(stripslashes($_GET["value"]),true);//value need to be the same parameter as url request!
foreach($x as $key => $v)
{
	echo "<h2>$key</h2>";
}

?>
