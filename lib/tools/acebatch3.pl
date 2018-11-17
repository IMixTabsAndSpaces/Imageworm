#!/usr/bin/perl

chdir "/home/zachlab/ImageWorm/DB-Java";
$numArgs = $#ARGV + 1;
#print "thanks, you gave me $numArgs command-line arguments.\n";

#foreach $argnum (0 .. $#ARGV) {
#   print "$ARGV[$argnum]\n";
#}
$nl = "\n";
$sp = " ";
$basecommand = 'nice java -mx500m -cp /home/zachlab/ImageWorm/DB-Java/acebatch3.jar';

$command = $basecommand;
foreach $argnum (0 .. $#ARGV) {
	$command = $command . $sp . $ARGV[$argnum];
}



print($command . $nl);
exec ($command) or print STDERR "couldn't execute " . $command . $nl;

