#!/bin/bash
# script for execution of deployed applications
#
# Sets up the MCR environment for the current $ARCH and executes 
# the specified command.
#
exe_name=$0
exe_dir=`dirname "$0"`
echo "------------------------------------------"
echo Setting up environment variables
MCRROOT="$1"
echo ---
LD_LIBRARY_PATH=.:${MCRROOT};
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRROOT}/bin/glnxa64 ;
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRROOT}/sys/os/glnxa64;
#MCRJRE=${MCRROOT}/sys/java/jre/glnxa64/jre/lib/amd64 ;
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRJRE}/native_threads ; 
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRJRE}/server ;
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRJRE}/client ;
#LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MCRJRE} ;  
#XAPPLRESDIR=${MCRROOT}/X11/app-defaults ;
export LD_LIBRARY_PATH;
#export XAPPLRESDIR;
echo LD_LIBRARY_PATH is ${LD_LIBRARY_PATH};
#shift 1
args=
while [ $# -gt 0 ]; do
    token=`echo "$1" | sed 's/ /\\\\ /g'`   # Add blackslash before each blank
    #args="${args} ${token}"
    args+=(${token})
    shift
done
#eval "${exe_dir}"/commandLineDriver $args
echo
echo '###################################################'
echo "'${args[*]}'"
echo "'${args[1]}'"
echo "'${args[2]}'"
echo "'${args[3]}'"
echo "'${args[4]}'"
echo "'${args[5]}'"
#echo $args
cd /mnt/Disk_2/work/tools/matlab_SN/Matlab_detector/distribution_code
#matlab -nojvm -nodesktop -r "try commandLineDriver('${args[1]}','${args[2]}','${args[3]}','${args[4]}');catch exit; end;"
matlab -nojvm -nodesktop -r "commandLineDriver('${args[1]}','${args[2]}','${args[3]}','${args[4]}');exit"
#matlab -nojvm -nodesktop -r "commandLineDriver('/mnt/Disk_2/work/tools/matlabParams','/mnt/Disk_2/work/images/20180831_Video_for_Amanda_L1/tif/','20180831_Video_for_Amanda','test'); quit"
    
    
echo '###################################################'
echo

exit


#/usr/local/MATLAB/R2018a/bin/matlab /home/stephen/Disk_1/work/images/20180831_Video_for_Amanda_L4/matlabParams /home/stephen/Disk_1/work/images/20180831_Video_for_Amanda_L4/tif/ 20180831_Video_for_Amanda_L4   
