#!/usr/bin/perl

#chdir "/home/zachlab/ImageWorm/DB-Java";
my $SCRIPTLOCATION=$ENV{'IW_LIB'};
chdir $SCRIPTLOCATION;
$numArgs = $#ARGV + 1;
#print "thanks, you gave me $numArgs command-line arguments.\n";

#foreach $argnum (0 .. $#ARGV) {
#   print "$ARGV[$argnum]\n";
#}
$nl = "\n";
$sp = " ";
$basecommand = 'nice java -mx500m -cp ./DB-Java/acebatch3.jar';

$command = $basecommand;
foreach $argnum (0 .. $#ARGV) {
	$command = $command . $sp . $ARGV[$argnum];
}

use Cwd qw();
my $path = Cwd::cwd();
print "$path\n";
print($command . $nl);
exec ($command) or print STDERR "couldn't execute " . $command . $nl;

