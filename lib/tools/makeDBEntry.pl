#!/usr/bin/perl

chdir "/home/zachlab/ImageWorm/DB-Java";
$numArgs = $#ARGV + 1;
#print "thanks, you gave me $numArgs command-line arguments.\n";

foreach $argnum (0 .. $#ARGV) {
   print "$ARGV[$argnum]\n";
}

my $nl = "\n";
my $sp = " ";
$LIB_HOME = $ENV{'IW_LIB'};
my $basecommand = "nice java -mx500m -cp $LIB_HOME/DB-Java/pipeline2.jar DBEntryMaker";
#new code 10/29/15 to hack access to /murrlab2 filesystem via symlink from murrlab
my $targetDir = $ARGV[0];
my $linkDir = $targetDir;
if($targetDir=~/murrlab[23]\/([^\/]+)\/images\/([^\/]+)/){
    $linkDir = "/gpfs/fs0/u/$1/images/$2";
    system "ln -s $targetDir $linkDir";
}

my $command = $basecommand . $sp . $linkDir;
# foreach $argnum (0 .. $#ARGV) {
# 	$command = $command . $sp . $ARGV[$argnum];
# }



print($command . $nl);
system (($command) || print STDERR "execution failed for " . $command . $nl);

my $dir = $ARGV[0];
if($dir=~/([^\/]+)\/?$/){
    my $seriesname = $1;
    my $annots = "NOTDIR";
    my $imagedir = "NOTDIR";
	$DBLocation = $ENV{'DBLocation'};
    if (-e "$DBLocation/$seriesname.xml"){
	foreach (`cat $DBLocation/$seriesname.xml`){
	    if(/annots loc=\"(.+)\"/){
	        $annots = $1 . "/dats";
	    }

	    if(/imageloc loc=\"(.+)\"/){
	        $imagedir = $1 . "/tif";
	    }
	}
	
	if(-e "$annots/$seriesname.xml"){
	    print STDERR "XML FILE CREATED SUCCESFULLY\n";
	}
	else{
	    open(OUT, ">$annots/$seriesname.xml") || die "couldn't write to $annots/$seriesname.xml\n";
	    print OUT "<?xml version='1.0' encoding='utf-8'?>\n";
	    print OUT "<embryo>\n";
	    print OUT "<image file=\"$imagedir/$seriesname" . "-t001-p01.tif\"/>\n";
	    print OUT "<nuclei file=\"$annots/$seriesname" . "-edit.zip\"/>\n";
	    print OUT "<start index=\"1\"/>\n";
	    print OUT "<end index=\"60\"/>\n";
	    print OUT "<axis axis=\"ADL\"/>\n";
	    print OUT "<resolution xyRes=\".087\" zRes=\".504\" planeEnd=\"66\"/>\n";
	    print OUT "</embryo>\n";
	    close OUT;
	}
	unless(-e "$annots/$seriesname.zip"){
	    system "cp home/zachlab/ImageWorm/DB-Java/emptyAcetreeZip.zip $annots/$seriesname.zip";
	    system "cp home/zachlab/ImageWorm/DB-Java/emptyAcetreeZip.zip $annots/$seriesname" . "-edit.zip";

	}
    }else{
	print STDERR "Couldn't identify DB xml file for $dir\n";
    }
}else{
    print STDERR "Couldn't parse seriesname from dir $dir\n";
}


