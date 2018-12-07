#!/bin/sh

#this is what you need to change 
#==========================================================
#export Storage=/mnt/Disk_2/work
export Storage=/media/zachlab/Windows/LinuxStorage
#export archiveDir=/mnt/Disk_2/work/embryo_movies
export archiveDir=/mnt/NAS/data/Documents/embryo_movies

#==========================================================

export IW_HOME=$PWD
export IW_LIB=$IW_HOME/lib
export targetDir=$Storage/images/archive
export nd2File=$Storage/images/ND2_Files
export mlParamLoc=$Storage/images/matlabParams

#for Java
export DBLocation=$Storage/embryoDB/
#as of right now i don't think these are used but if i need
#to call them here they are
export StarryNite=$IW_LIB/DB-Java/starrynite-debug
export SNDefaultParms=$IW_HOME/docs/parameters/greenParameters
export MatlabDefaultParams=$IW_LIB/DB-Java/mparmDEFAULT.txt
export ArchiveLocation=$Storage
export AnnotsLocation=$Storage
#export PATH=$PATH:$IW_HOME:$IW_LIB:$Fiji

