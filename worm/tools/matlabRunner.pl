#!/usr/bin/perl

print("snrunner.pl\n");

$numArgs = $#ARGV + 1;
print "thanks, you gave me $numArgs command-line arguments.\n";

foreach $argnum (0 .. $#ARGV) {

   print "$ARGV[$argnum]\n";

}
$nl = "\n";
$sp = " ";
$DIR = $ARGV[0];
if($DIR =~/.+\/([^\/]+)\/?$/){
    $SERIESROOT = $1;
}else{
    die"Couldn't parse seriesname\n";
}
$command = "nice ./matlab_SN_cluster.pl $DIR/tif/$SERIESROOT $DIR/matlabParams";
#$basecommand = 'nice /nfs/waterston/tools3/java/jdk1.5.0_06/bin/java -mx500m -cp pipeline2.jar MatlabRunner';

#$command = $basecommand;
#foreach $argnum (0 .. $#ARGV) {
#	$command = $command . $sp . $ARGV[$argnum];
#}



print($command . $nl);
exec ($command) or print STDERR "couldn't execute " . $command . $nl;

