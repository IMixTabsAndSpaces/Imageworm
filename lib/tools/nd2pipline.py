
import sys
import getpass
import shutil
import os
from FSConvert import Timetrack, logger, Makefilelist


@Timetrack
@logger
def extractTIFF(files, targetDir):
    for i in files:
        print("/home/zachlab/ImageWorm/Fiji.app/ImageJ-linux64 --ij2 -macro split_nd2 \"{} {}/\"".format(i, targetDir))
        os.system("/home/zachlab/ImageWorm/Fiji.app/ImageJ-linux64 -- -macro split_nd2 \"{} {}/\"".format(i, targetDir))

        
@Timetrack
def Main():
    targetDir = "/media/zachlab/Windows/LinuxStorage/images/archive"
    #print("OUTPUTTING TO: {}".format(targetDir))
    nd2File = "/media/zachlab/Windows/LinuxStorage/images/ND2_Files"  #sys.argv[1]
    mlParamLoc = "/media/zachlab/Windows/LinuxStorage/images/matlabParams" #sys.argv[2]
    
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
    os.chdir("/home/zachlab/ImageWorm/tools/")
    os.system("python rename.py {}/".format(targetDir+"/temp"))
    
    print("RUNNING MATLAB....")
    for x in os.listdir(targetDir+"/temp"):
        os.system("cp {} {}/{}/matlabParams\n".format(mlParamLoc, targetDir+"/temp", x)) 
        os.system("./matlabRunner.pl {}".format(targetDir+"/temp/"+x))
        #os.system("./standard_pipeline.sh {}/{} {}\n".format(targetDir+"/temp", x ,x)) 
    
    print("MOVING AND MAKEING DB ENTRY....")
    files = os.listdir(targetDir+"/temp")
    for f in files:
        shutil.move(targetDir+"/temp/"+f, targetDir)
        os.system("./makeDBEntry.pl {}".format(targetDir+"/"+f))
        print("./acebatch3.pl RedExtractor1 {}".format(f))
        os.system("./acebatch3.pl RedExtractor1 {}".format(f))
        print("./acebatch3.pl Measure1 {}".format(f))
        os.system("./acebatch3.pl Measure1 {}".format(f))
    #remove temp folder
    shutil.rmtree(targetDir+"/temp")
    #input("press enter to continue...")

#------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    Main()
#------------------------------------------------------------------------------------------------------
