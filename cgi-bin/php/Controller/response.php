<?php

function render($vue , $data = [])
{
	extract($data);
	$file ="cgi-bin/php/View/". $vue;
	if (!file_exists($file))
		throw new Exception ("Fichier introuvable: ". $file);
	ob_start(); //stop rendring html content and start buffering all html tags as string
	require ($file); 
	echo ob_get_clean();
}

