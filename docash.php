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
	$InflationMin = $_POST["InflationMin"] ;
	$InflationMax = $_POST["InflationMax"] ;
	$InvestMin = $_POST["InvestMin"] ;
	$InvestMax = $_POST["InvestMax"] ;
	$CashMin = $_POST["CashMin"] ;
	$CashMax = $_POST["CashMax"] ;
	$SpendDecrease = $_POST["SpendDecrease"] ;
	$str = "./cash $randomSeed $rent $inherit $inheritYear $YearSpend $ZRPYear $ZRP25 $PruYear $Pru25 $FerrantiYear $SimonYear $InflationMin $InflationMax $InvestMin $InvestMax $CashMin $CashMax $SpendDecrease" ;
	//echo $str;
	$handle = popen($str, 'r');
	while ($read = fread($handle, 1024))
	{
		echo $read;
	}
	pclose($handle);
?>
