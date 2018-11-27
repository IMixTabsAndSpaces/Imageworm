#!/usr/bin/perl -w
use File::Spec::Functions qw(rel2abs);
use File::Basename;
use File::Copy;
use Cwd;
$SCRIPTLOCATION=$ENV{'IW_LIB'};
#SCRIPTLOCATION=dirname(0);
printf("Scriptlocation: ".$SCRIPTLOCATION."\n");
#check that have 2 arguments tiff path and parameter file
if ($#ARGV<1){
	#printf(" usage .\\process_sequence.prl tiff_root_name matlab_parameterfile \n");
	die(" error: usage .\\process_sequence.prl tiff_root_name matlab_parameterfile \n");
}

$SUFFIX="";
if ($#ARGV==2){
	$SUFFIX=$ARGV[2];
}

$SNPATH=$SCRIPTLOCATION."/starrynite_traceonly/starrynite";

#$MCRPath = "/gpfs/fs0/l/murr/local/Matlab_Compiler_Runtime/v714/";
#$MCRPath = "/usr/local/MATLAB/R2018a/bin/matlab";
#$LAUNCHDIR=cwd;



#production version path

###$MATLABCOMMAND=  $SCRIPTLOCATION."/run_commandLineDriver.sh";



#$MATLABCOMMAND="matlab -automation -wait  -r \"commandLineDriver('";
#$MATLABCOMMAND="matlab  -nojvm -nodesktop -r \"try commandLineDriver('";
$MATLABCOMMAND="matlab  -nojvm -nodesktop -r \"try addpath('/home/zachlab/ImageWorm/tools/matlab_SN/Matlab_detector/distribution_code'); commandLineDriver('";


$TIFFLOCATION="";


#parse sequence name from tiff path
$_=$ARGV[0];
if(/(.*\/)(.*)/){
    $DIRECTORY=$1;
    $FILEROOT=$2;
    if($DIRECTORY=~/(.*\/)(.+)\//){
	$SERIESDIR = $1;
    }else{
	$SERIESDIR = $DIRECTORY;
    }
}else{
    print "Couldn't parse tiff path\n";
}




if(length($SUFFIX)==0){
printf("using fileroot - suffix ".$FILEROOT." ".$SUFFIX."\n");
$MATLABNUCLOCATION=$FILEROOT."_matlabnuclei/";
}
else{
$MATLABNUCLOCATION=$FILEROOT."_".$SUFFIX."_matlabnuclei/";
}

$SNPARAMFILE=$DIRECTORY.$FILEROOT."_".$SUFFIX."_sn_parameterfile.txt";

#detect cells with matlab
printf("running matlab nuclear detection:\n");

#go to script location so it can find supplemental files
chdir($SCRIPTLOCATION);

###USE THIS ONE FOR RUNNING MATLAB CODE DIRECTLY
printf($MATLABCOMMAND.$ARGV[1]."','".$DIRECTORY."','".$FILEROOT."','".$SUFFIX."');catch exit; end; quit;\"\n");
system($MATLABCOMMAND.$ARGV[1]."','".$DIRECTORY."','".$FILEROOT."','".$SUFFIX."');catch exit; end; quit;\"");

#printf($MATLABCOMMAND.$ARGV[1]."','".$DIRECTORY."','".$FILEROOT."','".$SUFFIX."')\"\n");
#system($MATLABCOMMAND.$ARGV[1]."','".$DIRECTORY."','".$FILEROOT."','".$SUFFIX."')\"");

#go to directory where matlab nuclei are saved
#this is just a safe location to output nuclei temp files
chdir($DIRECTORY."/".$FILEROOT."_matlabnuclei");

#go back so that lineaging occurs using launch dir as working directory
#dont need to do this for now
#chdir($LAUNCHDIR);



#zip result and put in dat directory
#make temp directory to zip
system("mkdir $SERIESDIR/matlab");
system("mkdir $SERIESDIR/MLtemp");
chdir("$SERIESDIR/MLtemp");
system("mkdir $SERIESDIR/MLtemp/nuclei");
system("mkdir $SERIESDIR/MLtemp/parameters");
system("rm *-nuclei");
system("rm log");

#lineage matlab result

printf("running lineaging:\n");
my $SNCOMMAND = $SNPATH."  ".$DIRECTORY.$TIFFLOCATION.$FILEROOT." ./ ".$SNPARAMFILE." ".$DIRECTORY.$MATLABNUCLOCATION;
print $SNCOMMAND . "\n";
my $success=0;
foreach (`$SNCOMMAND`){
    print;
    if(/End time/){
	$success=1;
	print "1\n";
	}else{print "0\n";}
}
if($success==0){
    die("SN FAILED!\n");
}

#system("mkdir ".$DIRECTORY."/".$FILEROOT."_matlabnuclei/nuclei");	
#system("mkdir ".$DIRECTORY."/".$FILEROOT."_matlabnuclei/parameters");	

#system("mv *-nuclei ".$DIRECTORY."/".$FILEROOT."_".$SUFFIX."_matlabnuclei/"."nuclei");
system("mv *-nuclei $SERIESDIR/MLtemp/nuclei/");
system("mv log $SERIESDIR/MLtemp/nuclei/");
system("cp $SNPARAMFILE $SERIESDIR/MLtemp/parameters/");

system("mv $DIRECTORY/$FILEROOT" . "_* $SERIESDIR/matlab/");



#system("cp ".$ARGV[1]." ".$DIRECTORY."/".$FILEROOT."_".$SUFFIX."_matlabnuclei/"."parameters/".$FILEROOT."-mparameters");

#this is going to fail if path to parameterfile has space in it
#system("cp ".$ARGV[1]." ".$DIRECTORY."/".$FILEROOT."_".$SUFFIX."_matlabnuclei/"."parameters/".$FILEROOT."-mparameters");
#system("mv ".$SNPARAMFILE." ".$DIRECTORY."/".$FILEROOT."_".$SUFFIX."_matlabnuclei/"."parameters/".$FILEROOT."-parameters");

#dont need to do this either because already in directory above the zips which is no longer $directory
#chdir($DIRECTORY);

system("zip ".$FILEROOT . ".zip "."nuclei/* "."parameters/*");

#clean up temp directory after zipped
#system("rm -rf ".$DIRECTORY."nuclei");	
#system("rm -rf ".$DIRECTORY."parameters");

system("rm -rf nuclei");	
system("rm -rf parameters");


#in production remove matlab per slice nuclei directory
#system("rm -rf ".$DIRECTORY.$MATLABNUCLOCATION);
#system("rm -rf ".$MATLABNUCLOCATION);



if(! -e($SERIESDIR."/dats")){
    system("mkdir $SERIESDIR/dats");
}
#system("mv ".$DIRECTORY.$FILEROOT.$SUFFIX.".zip ".$DIRECTORY."annot/dats");

system("cp ".$FILEROOT . ".zip ".$SERIESDIR ."/dats/$FILEROOT" . "-edit.zip");
system("mv ".$FILEROOT . ".zip ".$SERIESDIR ."/dats/");




#create xml dat file to match computed result

#first read data from config file
$XYRES="0.254";
$ZRES="1";
$SLICES="30";
open(PARAMFILEIN,"<".$ARGV[1]);

while(<PARAMFILEIN>){
    if(/zres\=\s*([\d\.]+)/){
	$ZRES=$1;
    }
    if(/xyres\=\s*([\d\.]+)/){
	$XYRES=$1;
    }
    if(/slices\=\s*([\d\.]+)/){
	$SLICES=$1;
    }
}

printf("outputing xml file with xy res ".$XYRES." z res ".$ZRES." slices ".$SLICES."\n");

open (PARAMFILE, ">".$SERIESDIR ."/dats/" . $FILEROOT . ".xml");
print PARAMFILE "<?xml version='1.0' encoding='utf-8'?>\n";
print PARAMFILE  "<embryo>\n";

my @subdir = split(/\//, $SERIESDIR, -1);
my $index = 0;
$index++ until $subdir[$index] eq 'temp';
splice(@subdir, $index, 1);
$SERIESDIR = join('/', @subdir);
@subdir = split(/\//, $DIRECTORY, -1);
$index = 0;
$index++ until $subdir[$index] eq 'temp';
splice(@subdir, $index, 1);
$DIRECTORY = join('/', @subdir);

print PARAMFILE "<image file=\"".$DIRECTORY.$TIFFLOCATION.$FILEROOT."-t001-p01.tif\"/>\n";
print PARAMFILE "<nuclei file=\"".$SERIESDIR."dats/".$FILEROOT."-edit.zip\"/>\n";
#print PARAMFILE "<image file=\"../tif/".$TIFFLOCATION.$FILEROOT."-t001-p01.tif\"/>\n";
#print PARAMFILE "<nuclei file=\"./".$FILEROOT."-edit.zip\"/>\n";

print PARAMFILE "<end index=\"60\"/>\n";
#print PARAMFILE "<resolution xyRes=\"0.254\" zRes=\"1\" planeEnd=\"30\"/> <exprCorr type=\"blot\"/>\n";
 
print PARAMFILE "<resolution xyRes=\"".$XYRES."\" zRes=\"".$ZRES."\" planeEnd=\"".$SLICES."\"/>\n"; 

print PARAMFILE "</embryo>";
close(PARAMFILE);


chdir($SCRIPTLOCATION);

#run analysis program
#system("acebatch3.pl Measure1 $FILEROOT");
#system("acebatch3.pl RedExtractor1 $FILEROOT");

#make edit copies 
#output whole new xml file from scratch, and make a second copy of zip file with edited suffix

# open (PARAMFILE, ">".$DIRECTORY."annot/dats/".$FILEROOT.$SUFFIX."_edited.xml");
# print PARAMFILE "<?xml version='1.0' encoding='utf-8'?>\n";
# print PARAMFILE  "<embryo>\n";
# print PARAMFILE "<image file=\"".$DIRECTORY.$TIFFLOCATION.$FILEROOT."-t001-p01.tif\"/>\n";
# print PARAMFILE "<nuclei file=\"".$DIRECTORY."annot/dats/".$FILEROOT.$SUFFIX."_edited.zip\"/>\n";
# print PARAMFILE "<end index=\"475\"/>\n";
# print PARAMFILE "<resolution xyRes=\"".$XYRES."\" zRes=\"".$ZRES."\" planeEnd=\"".$SLICES."\"/> <exprCorr type=\"blot\"/>\n"; 
# print PARAMFILE "<polar size=\"15\"/>\n";
# print PARAMFILE "</embryo>";
# close(PARAMFILE);

# $zipfilename=$DIRECTORY."annot/dats/".$FILEROOT.$SUFFIX.".zip";
# copy($zipfilename,$DIRECTORY."annot/dats/".$FILEROOT.$SUFFIX."_edited.zip");

#set others to read only
#system(" chmod a-w ".$zipfilename);
#system(" chmod a-w ".$DIRECTORY."annot/dats/".$FILEROOT.$SUFFIX.".xml");
