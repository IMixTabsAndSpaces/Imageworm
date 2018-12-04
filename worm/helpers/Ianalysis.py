#!/usr/bin/env python

import sys
import getpass
import shutil
import os
try:
    from helpers.FSConvert import Timetrack, logger, Makefilelist
except:
    from FSConvert import Timetrack, logger, Makefilelist
ROOT_DIR = os.environ['IW_HOME'] # This is your Project Root
print(ROOT_DIR)

@Timetrack
@logger
def extractTIFF(files, targetDir):
    for i in files:
        #print("ImageJ-linux64 --ij2 -macro split_nd2 \"{} {}/\"".format(i, targetDir))
        #os.system("ImageJ-linux64 -- -macro split_nd2 \"{} {}/\"".format(i, targetDir))
        print("{}/Fiji.app/ImageJ-linux64 --ij2 -macro split_nd2 \"{} {}/\"".format(os.environ['IW_LIB'], i, targetDir))
        os.system("{}/Fiji.app/ImageJ-linux64 -- -macro split_nd2 \"{} {}/\"".format(os.environ['IW_LIB'], i, targetDir))

        
@Timetrack
def Main():
    #targetDir = "/mnt/Disk_2/work/images/archive"
    #nd2File = "/mnt/Disk_2/work/images/images/ND2_Files"  #sys.argv[1]
    #mlParamLoc = "/mnt/Disk_2/work/images/images/matlabParams" #sys.argv[2]
    targetDir = os.environ['targetDir']
    nd2File = os.environ['nd2File']  #sys.argv[1]
    mlParamLoc = os.environ['mlParamLoc'] #sys.argv[2]
    
    imagelist = Makefilelist(nd2File, "*.nd2", True)
    print("EXTRACTING IMAGES....")
    #creating temp folder and using it as output
    try:
        os.mkdir(targetDir+"/temp")
    except:
        print("temp dir already exists")
    finally:
        extractTIFF(imagelist, targetDir+"/temp")
    
    print("RENAMEING IMAGES....")
    TOOLS_DIR = os.path.join(ROOT_DIR,'lib/tools/')
    os.chdir(TOOLS_DIR)
    os.system("python rename.py {}/".format(targetDir+"/temp"))
    
    print("RUNNING MATLAB....")
    for x in os.listdir(targetDir+"/temp"):
        os.system("cp {} {}/{}/matlabParams\n".format(mlParamLoc, targetDir+"/temp", x)) 
        os.system("./matlabRunner.pl {}".format(targetDir+"/temp/"+x))

    print("MOVING AND MAKEING DB ENTRY....")
    files = os.listdir(targetDir+"/temp")
    for f in files:
        shutil.move(targetDir+"/temp/"+f, targetDir)
        print("./makeDBEntry.pl {}".format(targetDir+"/"+f))
        os.system("./makeDBEntry.pl {}".format(targetDir+"/"+f))
        print("./acebatch3.pl RedExtractor1 {}".format(f))
        #os.system("./acebatch3.pl RedExtractor1 {}".format(f))
        print("./acebatch3.pl Measure1 {}".format(f))
        #os.system("./acebatch3.pl Measure1 {}".format(f))
    #remove temp folder
    shutil.rmtree(targetDir+"/temp")
    #input("press enter to continue...")
    outfiles = [os.path.join(targetDir, f) for f in files]
    return outfiles
#------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    outfile = Main()
    print(outfile)
    
#------------------------------------------------------------------------------------------------------
