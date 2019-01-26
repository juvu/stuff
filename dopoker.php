<?php
	$card16= $_POST["card16"] ;
	$card17= $_POST["card17"] ;
	$card18= $_POST["card18"] ;
	$card19= $_POST["card19"] ;
	$card1t= $_POST["card1t"] ;
	$card1j= $_POST["card1j"] ;
	$card1q= $_POST["card1q"] ;
	$card1k= $_POST["card1k"] ;
	$card1a= $_POST["card1a"] ;
	$card1h= $_POST["card1h"] ;
	$card1d= $_POST["card1d"] ;
	$card1c= $_POST["card1c"] ;
	$card1s= $_POST["card1s"] ;
	$card1 = "$card16$card17$card18$card19$card1t$card1j$card1q$card1k$card1a$card1h$card1d$card1c$card1s";

	$card26= $_POST["card26"] ;
	$card27= $_POST["card27"] ;
	$card28= $_POST["card28"] ;
	$card29= $_POST["card29"] ;
	$card2t= $_POST["card2t"] ;
	$card2j= $_POST["card2j"] ;
	$card2q= $_POST["card2q"] ;
	$card2k= $_POST["card2k"] ;
	$card2a= $_POST["card2a"] ;
	$card2h= $_POST["card2h"] ;
	$card2d= $_POST["card2d"] ;
	$card2c= $_POST["card2c"] ;
	$card2s= $_POST["card2s"] ;
	$card2 = "$card26$card27$card28$card29$card2t$card2j$card2q$card2k$card2a$card2h$card2d$card2c$card2s";

	$flop16= $_POST["flop16"] ;
	$flop17= $_POST["flop17"] ;
	$flop18= $_POST["flop18"] ;
	$flop19= $_POST["flop19"] ;
	$flop1t= $_POST["flop1t"] ;
	$flop1j= $_POST["flop1j"] ;
	$flop1q= $_POST["flop1q"] ;
	$flop1k= $_POST["flop1k"] ;
	$flop1a= $_POST["flop1a"] ;
	$flop1h= $_POST["flop1h"] ;
	$flop1d= $_POST["flop1d"] ;
	$flop1c= $_POST["flop1c"] ;
	$flop1s= $_POST["flop1s"] ;
	$flop1 = "$flop16$flop17$flop18$flop19$flop1t$flop1j$flop1q$flop1k$flop1a$flop1h$flop1d$flop1c$flop1s";

	$flop26= $_POST["flop26"] ;
	$flop27= $_POST["flop27"] ;
	$flop28= $_POST["flop28"] ;
	$flop29= $_POST["flop29"] ;
	$flop2t= $_POST["flop2t"] ;
	$flop2j= $_POST["flop2j"] ;
	$flop2q= $_POST["flop2q"] ;
	$flop2k= $_POST["flop2k"] ;
	$flop2a= $_POST["flop2a"] ;
	$flop2h= $_POST["flop2h"] ;
	$flop2d= $_POST["flop2d"] ;
	$flop2c= $_POST["flop2c"] ;
	$flop2s= $_POST["flop2s"] ;
	$flop2 = "$flop26$flop27$flop28$flop29$flop2t$flop2j$flop2q$flop2k$flop2a$flop2h$flop2d$flop2c$flop2s";

	$flop36= $_POST["flop36"] ;
	$flop37= $_POST["flop37"] ;
	$flop38= $_POST["flop38"] ;
	$flop39= $_POST["flop39"] ;
	$flop3t= $_POST["flop3t"] ;
	$flop3j= $_POST["flop3j"] ;
	$flop3q= $_POST["flop3q"] ;
	$flop3k= $_POST["flop3k"] ;
	$flop3a= $_POST["flop3a"] ;
	$flop3h= $_POST["flop3h"] ;
	$flop3d= $_POST["flop3d"] ;
	$flop3c= $_POST["flop3c"] ;
	$flop3s= $_POST["flop3s"] ;
	$flop3 = "$flop36$flop37$flop38$flop39$flop3t$flop3j$flop3q$flop3k$flop3a$flop3h$flop3d$flop3c$flop3s";

	$flop46= $_POST["flop46"] ;
	$flop47= $_POST["flop47"] ;
	$flop48= $_POST["flop48"] ;
	$flop49= $_POST["flop49"] ;
	$flop4t= $_POST["flop4t"] ;
	$flop4j= $_POST["flop4j"] ;
	$flop4q= $_POST["flop4q"] ;
	$flop4k= $_POST["flop4k"] ;
	$flop4a= $_POST["flop4a"] ;
	$flop4h= $_POST["flop4h"] ;
	$flop4d= $_POST["flop4d"] ;
	$flop4c= $_POST["flop4c"] ;
	$flop4s= $_POST["flop4s"] ;
	$flop4 = "$flop46$flop47$flop48$flop49$flop4t$flop4j$flop4q$flop4k$flop4a$flop4h$flop4d$flop4c$flop4s";

	$flop56= $_POST["flop56"] ;
	$flop57= $_POST["flop57"] ;
	$flop58= $_POST["flop58"] ;
	$flop59= $_POST["flop59"] ;
	$flop5t= $_POST["flop5t"] ;
	$flop5j= $_POST["flop5j"] ;
	$flop5q= $_POST["flop5q"] ;
	$flop5k= $_POST["flop5k"] ;
	$flop5a= $_POST["flop5a"] ;
	$flop5h= $_POST["flop5h"] ;
	$flop5d= $_POST["flop5d"] ;
	$flop5c= $_POST["flop5c"] ;
	$flop5s= $_POST["flop5s"] ;
	$flop5 = "$flop56$flop57$flop58$flop59$flop5t$flop5j$flop5q$flop5k$flop5a$flop5h$flop5d$flop5c$flop5s";

	if ($flop5 != "")
	{
		$str = "./poker 7 $card1 $card2 $flop1 $flop2 $flop3 $flop4 $flop5" ;
	}
	else if ($flop4 != "")
	{
		$str = "./poker 6 $card1 $card2 $flop1 $flop2 $flop3 $flop4" ;
	}
	else if ($flop3 != "")
	{
		$str = "./poker 5 $card1 $card2 $flop1 $flop2 $flop3" ;
	}
	else if ($card2 != "")
	{
		$str = "./poker 2 $card1 $card2" ;
	}

	//echo "$str" ;

	$handle = popen($str, 'r');
	while ($read = fread($handle, 1024))
	{
		echo $read;
	}
	pclose($handle);

?>
