<?php
	/* Add redirection so we can get stderr. */
	$randomSeed = $_POST["RandomSeed"] ;
	$inherit = $_POST["Inheritance"] ;
	$rent = $_POST["Rent"] ;
	$inheritYear = $_POST["InheritYear"] ;
	$YearSpend = $_POST["YearSpend"] ;
	$ZRPYear = $_POST["ZRPYear"] ;
	$ZRP25 = $_POST["ZRP25"] ;
	$ZRP25 = ($ZRP25 == "on") ? 1 : 0;
	$PruYear = $_POST["PruYear"] ;
	$Pru25 = $_POST["Pru25"] ;
	$Pru25 = ($Pru25 == "on") ? 1 : 0;
	$FerrantiYear = $_POST["FerrantiYear"] ;
	$SimonYear = $_POST["SimonYear"] ;
	$InflationMean = $_POST["InflationMean"] ;
	$InflationSD = $_POST["InflationSD"] ;
	$InvestMean = $_POST["InvestMean"] ;
	$InvestSD = $_POST["InvestSD"] ;
	$CashMean = $_POST["CashMean"] ;
	$CashSD = $_POST["CashSD"] ;
	$SpendDecrease = $_POST["SpendDecrease"] ;
	$FirstYear = $_POST["FirstYear"] ;
	$ZRP = $_POST["ZRP"] ;
	$Pru = $_POST["Pru"] ;
	$cash = $_POST["Cash"] ;
	$str = "./cash $randomSeed $rent $inherit $inheritYear $YearSpend $ZRPYear $ZRP25 $PruYear $Pru25 $FerrantiYear $SimonYear $InflationMean $InflationSD $InvestMean $InvestSD $CashMean $CashSD $SpendDecrease $FirstYear $ZRP $Pru $cash" ;
	//echo $str;
	$handle = popen($str, 'r');
	while ($read = fread($handle, 1024))
	{
		echo $read;
	}
	pclose($handle);
?>
