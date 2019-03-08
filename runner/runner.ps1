param($scriptname = "", $version = "")

$runnerpath = "./runtime/";
$testpath = "../test_unit/";
$libpath = "../libraries/";
$libname ="ThingSat";

Copy-Item -Path $testpath+$scriptname -Destination C:\test2\;



