#!/usr/bin/perl

use File::Basename;
#my($filename, $directories, $suffix) = fileparse($path);

#my $nl = "\n";
#my $command = "python ./playground.py";
#system (($command) || print STDERR "execution failed for " . $command . $nl);

#print join(':', split(//, ' abc', -1)), "\n";
my $path = "/home/path/dashestxt";
my @subdir = split(/\//, $path, -1);
pop @subdir;

$path = join('/', @subdir);
print $path;