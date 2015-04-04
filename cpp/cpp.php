<?php

$arr = isset($_POST['arr']) ? $_POST['arr'] : '';
if(empty($arr))
{
	exit('please input the sort number');
}
else
{
	$command = './sort' . escapeshellcmd($arr);
	$return = passthru($command);
	var_dump($return);
}
?>