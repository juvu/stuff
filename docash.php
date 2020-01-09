<?php
	/* Add redirection so we can get stderr. */
	$randomSeed = $_POST["RandomSeed"] ;
	$inherit = $_POST["Inheritance"] ;
	$rent = $_POST["Rent"] ;
	$inheritYear = $_POST["InheritYear"] ;
	$YearSpend = $_POST["YearSpend"] ;
	$ZurichYear = $_POST["ZurichYear"] ;
	$Zurich25 = $_POST["Zurich25"] ;
	$Zurich25 = ($Zurich25 == "on") ? 1 : 0;
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
	$Zurich = $_POST["ZRP"] ;
	$Zurich = $_POST["Zurich"] ;
	$Pru = $_POST["Pru"] ;
	$cash = $_POST["Cash"] ;
	$FerrantiAmount = $_POST["FerrantiAmount"] ;
	$SimonAmount = $_POST["SimonAmount"] ;
	$StatePension = $_POST["StatePension"] ;
	$TaxAllowance = $_POST["TaxAllowance"] ;
	$str = "./cash $randomSeed $rent $inherit $inheritYear $YearSpend $ZurichYear $Zurich25 $PruYear $Pru25 $FerrantiYear $SimonYear $InflationMean $InflationSD $InvestMean $InvestSD $CashMean $CashSD $SpendDecrease $FirstYear $ZRP $Zurich $Pru $cash $FerrantiAmount $SimonAmount $StatePension $TaxAllowance" ;
	//echo $str;
	$handle = popen($str, 'r');
	while ($read = fread($handle, 1024))
	{
		echo $read;
	}
	pclose($handle);
?>
