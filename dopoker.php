<?php
	$card12h= $_POST["card12h"] ;
	$card13h= $_POST["card13h"] ;
	$card14h= $_POST["card14h"] ;
	$card15h= $_POST["card15h"] ;
	$card16h= $_POST["card16h"] ;
	$card17h= $_POST["card17h"] ;
	$card18h= $_POST["card18h"] ;
	$card19h= $_POST["card19h"] ;
	$card1th= $_POST["card1th"] ;
	$card1jh= $_POST["card1jh"] ;
	$card1qh= $_POST["card1qh"] ;
	$card1kh= $_POST["card1kh"] ;
	$card1ah= $_POST["card1ah"] ;
	$card12d= $_POST["card12d"] ;
	$card13d= $_POST["card13d"] ;
	$card14d= $_POST["card14d"] ;
	$card15d= $_POST["card15d"] ;
	$card16d= $_POST["card16d"] ;
	$card17d= $_POST["card17d"] ;
	$card18d= $_POST["card18d"] ;
	$card19d= $_POST["card19d"] ;
	$card1td= $_POST["card1td"] ;
	$card1jd= $_POST["card1jd"] ;
	$card1qd= $_POST["card1qd"] ;
	$card1kd= $_POST["card1kd"] ;
	$card1ad= $_POST["card1ad"] ;
	$card12s= $_POST["card12s"] ;
	$card13s= $_POST["card13s"] ;
	$card14s= $_POST["card14s"] ;
	$card15s= $_POST["card15s"] ;
	$card16s= $_POST["card16s"] ;
	$card17s= $_POST["card17s"] ;
	$card18s= $_POST["card18s"] ;
	$card19s= $_POST["card19s"] ;
	$card1ts= $_POST["card1ts"] ;
	$card1js= $_POST["card1js"] ;
	$card1qs= $_POST["card1qs"] ;
	$card1ks= $_POST["card1ks"] ;
	$card1as= $_POST["card1as"] ;
	$card12c= $_POST["card12c"] ;
	$card13c= $_POST["card13c"] ;
	$card14c= $_POST["card14c"] ;
	$card15c= $_POST["card15c"] ;
	$card16c= $_POST["card16c"] ;
	$card17c= $_POST["card17c"] ;
	$card18c= $_POST["card18c"] ;
	$card19c= $_POST["card19c"] ;
	$card1tc= $_POST["card1tc"] ;
	$card1jc= $_POST["card1jc"] ;
	$card1qc= $_POST["card1qc"] ;
	$card1kc= $_POST["card1kc"] ;
	$card1ac= $_POST["card1ac"] ;
	$cards = "$card12h$card13h$card14h$card15h$card16h$card17h$card18h$card19h$card1th$card1jh$card1qh$card1kh$card1ah";
	$cards .= "$card12d$card13d$card14d$card15d$card16d$card17d$card18d$card19d$card1td$card1jd$card1qd$card1kd$card1ad";
	$cards .= "$card12s$card13s$card14s$card15s$card16s$card17s$card18s$card19s$card1ts$card1js$card1qs$card1ks$card1as";
	$cards .= "$card12c$card13c$card14c$card15c$card16c$card17c$card18c$card19c$card1tc$card1jc$card1qc$card1kc$card1ac";

	$flop12h= $_POST["flop12h"] ;
	$flop13h= $_POST["flop13h"] ;
	$flop14h= $_POST["flop14h"] ;
	$flop15h= $_POST["flop15h"] ;
	$flop16h= $_POST["flop16h"] ;
	$flop17h= $_POST["flop17h"] ;
	$flop18h= $_POST["flop18h"] ;
	$flop19h= $_POST["flop19h"] ;
	$flop1th= $_POST["flop1th"] ;
	$flop1jh= $_POST["flop1jh"] ;
	$flop1qh= $_POST["flop1qh"] ;
	$flop1kh= $_POST["flop1kh"] ;
	$flop1ah= $_POST["flop1ah"] ;
	$flop12d= $_POST["flop12d"] ;
	$flop13d= $_POST["flop13d"] ;
	$flop14d= $_POST["flop14d"] ;
	$flop15d= $_POST["flop15d"] ;
	$flop16d= $_POST["flop16d"] ;
	$flop17d= $_POST["flop17d"] ;
	$flop18d= $_POST["flop18d"] ;
	$flop19d= $_POST["flop19d"] ;
	$flop1td= $_POST["flop1td"] ;
	$flop1jd= $_POST["flop1jd"] ;
	$flop1qd= $_POST["flop1qd"] ;
	$flop1kd= $_POST["flop1kd"] ;
	$flop1ad= $_POST["flop1ad"] ;
	$flop12s= $_POST["flop12s"] ;
	$flop13s= $_POST["flop13s"] ;
	$flop14s= $_POST["flop14s"] ;
	$flop15s= $_POST["flop15s"] ;
	$flop16s= $_POST["flop16s"] ;
	$flop17s= $_POST["flop17s"] ;
	$flop18s= $_POST["flop18s"] ;
	$flop19s= $_POST["flop19s"] ;
	$flop1ts= $_POST["flop1ts"] ;
	$flop1js= $_POST["flop1js"] ;
	$flop1qs= $_POST["flop1qs"] ;
	$flop1ks= $_POST["flop1ks"] ;
	$flop1as= $_POST["flop1as"] ;
	$flop12c= $_POST["flop12c"] ;
	$flop13c= $_POST["flop13c"] ;
	$flop14c= $_POST["flop14c"] ;
	$flop15c= $_POST["flop15c"] ;
	$flop16c= $_POST["flop16c"] ;
	$flop17c= $_POST["flop17c"] ;
	$flop18c= $_POST["flop18c"] ;
	$flop19c= $_POST["flop19c"] ;
	$flop1tc= $_POST["flop1tc"] ;
	$flop1jc= $_POST["flop1jc"] ;
	$flop1qc= $_POST["flop1qc"] ;
	$flop1kc= $_POST["flop1kc"] ;
	$flop1ac= $_POST["flop1ac"] ;
	$cards .= "$flop12h$flop13h$flop14h$flop15h$flop16h$flop17h$flop18h$flop19h$flop1th$flop1jh$flop1qh$flop1kh$flop1ah";
	$cards .= "$flop12d$flop13d$flop14d$flop15d$flop16d$flop17d$flop18d$flop19d$flop1td$flop1jd$flop1qd$flop1kd$flop1ad";
	$cards .= "$flop12s$flop13s$flop14s$flop15s$flop16s$flop17s$flop18s$flop19s$flop1ts$flop1js$flop1qs$flop1ks$flop1as";
	$cards .= "$flop12c$flop13c$flop14c$flop15c$flop16c$flop17c$flop18c$flop19c$flop1tc$flop1jc$flop1qc$flop1kc$flop1ac";

	$pounds = $_POST["pounds"] ;
	if (!$pounds)
		$pounds = 0;
	$pence = $_POST["pence"] ;
	if (!$pence)
		$pence = 0;

	$str = "./poker $pounds $pence $cards" ;

	echo "$str" ;

	$handle = popen($str, 'r');
	while ($read = fread($handle, 1024))
	{
		echo $read;
	}
	pclose($handle);

?>
