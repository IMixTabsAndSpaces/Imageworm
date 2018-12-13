#!/usr/bin/env python

import sys
import getpass
import shutil
import os
try:
    from helpers.FSConvert import Timetrack, logger, Makefilelist
except:
    from FSConvert import Timetrack, logger, Makefilelist
import multiprocessing as mp

@Timetrack
@logger
def extractTIFF(files, targetDir):
    for i in files:
        #print("ImageJ-linux64 --ij2 -macro split_nd2 \"{} {}/\"".format(i, targetDir))
        #os.system("ImageJ-linux64 -- -macro split_nd2 \"{} {}/\"".format(i, targetDir))
        print("{}/Fiji.app/ImageJ-linux64 --ij2 -macro split_nd2 \"{} {}/\"".format(os.environ['IW_LIB'], i, targetDir))
        os.system("{}/Fiji.app/ImageJ-linux64 -- -macro split_nd2 \"{} {}/\"".format(os.environ['IW_LIB'], i, targetDir))

@Timetrack
def Main(fileName=None, nd2=None, outDir=None, MATLAB=True, MakeDB=True, RedExtract=True, 
        Measure=True, RedExcel1=True, RedExcel2=False, Align=False):
    #targetDir = "/mnt/Disk_2/work/images/archive"

    #nd2File = "/mnt/Disk_2/work/images/images/ND2_Files"  #sys.argv[1]
    #mlParamLoc = "/mnt/Disk_2/work/images/images/matlabParams" #sys.argv[2]
    ROOT_DIR = os.environ['IW_HOME'] # This is your Project Root

    if nd2:
        nd2File = nd2
    else:
        nd2File = os.environ['nd2File']
    if outDir:
        targetDir = outDir
    else:
        targetDir = os.environ['targetDir']
    mlParamLoc = os.environ['mlParamLoc']
    TOOLS_DIR = os.path.join(ROOT_DIR,'lib/tools/')
    os.chdir(TOOLS_DIR)

    if not fileName:
        imagelist = Makefilelist(nd2File, "*.nd2", True)
        print("EXTRACTING IMAGES....")
        print(imagelist)
        print(nd2File)
        #creating temp folder and using it as output
        try:
            os.mkdir(targetDir+"/temp")
        except:
            print("temp dir already exists")
        finally:
            extractTIFF(imagelist, targetDir+"/temp")
    
        print("RENAMEING IMAGES....")
        print(("python rename.py {}/".format(targetDir+"/temp")))
        os.system("python rename.py {}/".format(targetDir+"/temp"))
    if MATLAB:
        if not fileName:
            print("RUNNING MATLAB....")
            for x in os.listdir(targetDir+"/temp"):
                os.system("cp {} {}/{}/matlabParams\n".format(mlParamLoc, targetDir+"/temp", x)) 
                os.system("./matlab_SN_cluster.pl {0}/tif/{1} {0}/matlabParams".format(targetDir+"/temp/"+x, x))
        else:
            print("RUNNING MATLAB....")
            for x in fileName:
                x = os.path.basename(x)
                try:
                    shutil.rmtree(os.path.join(targetDir, x, 'dats'))
                except:
                    pass
                try:
                    shutil.rmtree(os.path.join(targetDir, x, 'matlab'))
                except:
                    pass
                try:
                    os.remove(os.path.join(targetDir, x, 'matlabParams'))
                except:
                    pass
                finally:
                    os.system("cp {} {}/{}".format(mlParamLoc, targetDir, x))
                    #os.system("./matlabRunner.pl {}".format(os.path.join(targetDir,x)))
                    os.system("./matlab_SN_cluster.pl {0}/tif/{1} {0}/matlabParams".format(os.path.join(targetDir, x), x))

    if not fileName:
        print("MOVING AND MAKEING DB ENTRY....")
        files = os.listdir(targetDir+"/temp")
        for f in files:
            shutil.move(targetDir+"/temp/"+f, targetDir)
            print("./makeDBEntry.pl {}".format(targetDir+"/"+f))
            os.system("./makeDBEntry.pl {}".format(targetDir+"/"+f))
            print("./acebatch3.pl RedExtractor1 {}".format(f))
            os.system("./acebatch3.pl RedExtractor1 {}".format(f))
            print("./acebatch3.pl Measure1 {}".format(f))
            os.system("./acebatch3.pl Measure1 {}".format(f))
        #remove temp folder
        shutil.rmtree(targetDir+"/temp")
        #input("press enter to continue...")
        outfiles = [os.path.join(targetDir, f) for f in files]
        print(outfiles)
        return outfiles
    else:
        files = [os.path.basename(f) for f in fileName]
        for f in files:
            if MakeDB:
                print("./makeDBEntry.pl {}".format(targetDir+"/"+f))
                os.system("./makeDBEntry.pl {}".format(targetDir+"/"+f))
            if RedExtract:
                print("./acebatch3.pl RedExtractor1 {}".format(f))
                os.system("./acebatch3.pl RedExtractor1 {}".format(f))
            if Measure: 
                print("./acebatch3.pl Measure1 {}".format(f))
                os.system("./acebatch3.pl Measure1 {}".format(f))
            if RedExcel1:
                print("./acebatch3.pl RedExcel1 {}".format(f))
                os.system("./acebatch3.pl RedExcel1 {}".format(f))
            if RedExcel2:
                print("./acebatch3.pl RedExcel2 {}".format(f))
                os.system("./acebatch3.pl RedExcel2 {}".format(f))
            if Align:
                print("./acebatch3.pl Align1 {}".format(f))
                os.system("./acebatch3.pl Align1 {}".format(f))

#------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    outfile = Main()
    #Main(fileName=['/media/zachlab/Windows/LinuxStorage/images/archive/2018-12-11-12-14-xy6'], MATLAB=False)
    print(outfile)
    
#------------------------------------------------------------------------------------------------------
