#!/bin/sh
export IW_HOME=$PWD
export IW_LIB=$IW_HOME/lib 
export Fiji=$IW_LIB/Fiji.app
#this is what you need to change 
#==========================================================
#export Storage=/mnt/Disk_2/work
export Storage=/media/zachlab/Windows/LinuxStorage
export archiveDir=/mnt/NAS/data/Documents/embryo_movies
#==========================================================
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
#export PATH=$PATH:$IW_HOME:$IW_LIB:$Fiji