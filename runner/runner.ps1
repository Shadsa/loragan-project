param($scriptname = $mainpath, $version = "v1")

$runnerpath = "./runtime/";
$testpath = "../test_unit/";
$libpath = "../libraries/";
$libname ="ThingSat_";
$mainpath="../main.ino";

Remove-Item $runnerpath"/*"
Copy-Item -Path $testpath$scriptname -Destination $runnerpath;
Copy-Item -Path $libpath$libname$version"/*" -Destination $runnerpath;



