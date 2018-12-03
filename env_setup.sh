#!/bin/sh
export IW_HOME=$PWD
export IW_LIB=$IW_HOME/lib 
export Fiji=$IW_LIB/Fiji.app
export Storage=/mnt/Disk_2/work
export targetDir=$Storage/images/archive
export nd2File=$Storage/images/ND2_Files  #sys.argv[1]
export mlParamLoc=$Storage/images/matlabParams #sys.argv[2]

#for Java
export DBLocation=$Storage/embryoDB/
export StarryNite=$IW_LIB/DB-Java/starrynite-debug
export SNDefaultParms=$IW_HOME/docs/parameters/greenParameters
export MatlabDefaultParams=$IW_LIB/DB-Java/mparmDEFAULT.txt
export ArchiveLocation=$Storage
export AnnotsLocation=$Storage
echo $StarryNite
echo $SNDefaultParms
echo $MatlabDefaultParams
#export PATH=$PATH:$IW_HOME:$IW_LIB:$Fiji