# Sample batch job script (pipetest4.sh)
# Use the bash shell to interpret this job script
#! /bin/bash

# enable matlab usage on cluster
#module ()
#{
#eval `/usr/bin/modulecmd bash $*`
#}

#module load modules modules-init modules-gs
#module load lmutil matlab
#export LM_LICENSE_FILE=27000@lmas.engr.washington.edu
#export LM_PROJECT=cheh3Ael_WATERSTON

# launch from sage (ssh sage.gs.washington.edu)
# in this case I point at a pipeline config file somewhere on /nfs/waterston
# from one of my home directory locations on /nfs/gs/home
# the "terminal output" will be put into a file in this directory
# biowolp@sage:~/0tmp/sage $ qsub -M biowolp@u.washington.edu pipetest4.sh /nfs/waterston/biowolp/pipelineTestConfig.xml
#

##################################MARKER1
echo "\nlaunch basicPipeline"
#echo $1
#echo $2
cd /home/zachlab/ImageWorm/tools
./matlabRunner.pl $1
echo "./makeDBEntry.pl" $1
./makeDBEntry.pl $1
#./acebatch3.pl RedExtractor1 $2
#./acebatch3.pl Measure1 $2
echo "Finished Pipeline \n"




#chgrp -Rf users $1
#chgrp -Rf users /mnt/Disk_2/work/embryoDB
##################################MARKER2


##
## direct output to the launch directory
#$ -cwd


#$ -m e

## Exit with return code 0, otherwise the job would exit with the
## return code of the failed ls command.
exit 0
##

