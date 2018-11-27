#!/bin/sh
export IW_HOME=$PWD
export IW_LIB=$IW_HOME/lib 
export Fiji=$IW_LIB/Fiji.app
export targetDir=/media/zachlab/Windows/LinuxStorage/images/archive
export nd2File=/media/zachlab/Windows/LinuxStorage/images/ND2_Files  #sys.argv[1]
export mlParamLoc=/media/zachlab/Windows/LinuxStorage/images/matlabParams #sys.argv[2]

#for Java
export DBLocation=/media/zachlab/Windows/LinuxStorage/embryoDB/
export StarryNite=$IW_LIB/DB-Java/starrynite-debug
export SNDefaultParms=$IW_HOME/docs/parameters/greenParameters
export MatlabDefaultParams=$IW_LIB/DB-Java/mparmDEFAULT.txt
export ArchiveLocation=/media/zachlab/Windows/LinuxStorage/
export AnnotsLocation=/media/zachlab/Windows/LinuxStorage/
echo $StarryNite
echo $SNDefaultParms
echo $MatlabDefaultParams
#export PATH=$PATH:$IW_HOME:$IW_LIB:$Fiji