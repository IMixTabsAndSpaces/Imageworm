#!/usr/bin/perl -w
# Version of FullPipeline which reads from an LIF file.

use strict;
use Parallel::ForkManager;

my $start = time;

my $MAX_PROCESSES = 8; 
my $pm = Parallel::ForkManager->new($MAX_PROCESSES);

my $lifFile = $ARGV[0];
my $mlParamLoc = $ARGV[1];

# get current user
my $user = "stephen";
foreach(`whoami`){
  chomp;
  if(/.+/){
  	$user = $_;
  }
}

#my $targetDir = "/gpfs/fs0/u/$user/images";
#my $targetDir = "/murrlab2/$user/images";
my $targetDir = "/mnt/Disk_2/work/images";
print STDERR "Outputting to $targetDir\n";

die if not $lifFile =~ /(.*)\.lif/;
die if not -e $mlParamLoc;

# this is "everything except the trailing .lif"
my $lifPrefix = $1;

my $dir = $lifPrefix;
if($dir=~/.+\/$/){
  chop $dir;
}

if($dir=~/([^\/]+)$/){
  my $seriesStem = $1;
  print "$dir $seriesStem\n";

#    system "./LeicaTifRename.pl $dir\n";
  # extract images from .lif file
  #print "extracting images from .lif file $lifFile\n";
  #system("java -Xms2g -Xmx5g -cp /mnt/Disk_2/work/tools/worm.jar edu.murrlab.image.translate.LeicaImporter " . $lifFile);

  # at this point, files should be in the same
  # directory where the .lif file is
  # move files to that user's directory

  # XXX only works with a limited number of embryos
  foreach (1..16) {
    my $dir = $lifPrefix . "_L" . $_;
    if (-e $dir) {
	print STDERR "mv $dir $targetDir\n";
	system("mv $dir $targetDir");
    }else{
	print STDERR "couldn't find $dir to move to $targetDir\n";
    }
  }

  # pipeline each series
  my $search = $targetDir;
  foreach(`ls $search`){
	  my $extendStem = $seriesStem . "_L";
	  if(/$extendStem\d/){
	      print "pipelining series $_\n";
	      chomp;
	      # Forks and returns the pid for the child:
	      #my $pid = $pm->start and next;
	      system "cp $mlParamLoc $targetDir/$_/matlabParams\n";
	      chdir "/mnt/Disk_2/work/tools/";
	      #system "./standard_pipeline.sh $search/$_ $_\n";
	      print "./standard_pipeline.sh $search/$_ $_\n";
	      #$pm->finish; # Terminates the child process
	  }
  }
  #$pm->wait_all_children;
}

my $duration = time - $start;
print "Execution time: $duration s\n";
