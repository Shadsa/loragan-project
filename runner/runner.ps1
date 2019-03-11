param($scriptname = "", $version = "v1")

$runnerpath = "./runtime/";
$testpath = "../test_unit/";
$libpath = "../libraries/";
$libname ="ThingSat_";
$mainpath="../main.ino";

Remove-Item $runnerpath"/*";

if(($scriptname -eq "")){
    Copy-Item -Path $mainpath -Destination $runnerpath;
}   
else{
    Copy-Item -Path $testpath$scriptname -Destination $runnerpath;
}
 

Copy-Item -Path $libpath$libname$version"/*" -Destination $runnerpath;



