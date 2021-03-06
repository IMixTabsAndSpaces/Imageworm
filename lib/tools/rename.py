import os
import sys
from FSConvert import *
import datetime
import shutil
#=====================================================================================

def rename(fileArray, incorment, date):
    newfA = [""]* len(fileArray)
    max_time = 0
    max_planes = 0
    for idx, i in enumerate(fileArray):
        s = [i]
        s = breakup(s, ".tif", 0)
        s = breakup(s, "_z", 0)
        s = breakup(s, "_t", 0)
        s[1] = '-t'
        s[3] = '-p'
        temp = []
        length = 3
        #increasing time index so it doesnt start at 1 again
        s[2] = str(int(s[2]) + (idx * incorment))

        #adding correct zero padding
        #must be 3 and 2 char long 
        for ii in s[2], s[4]:
            while len(ii) >=3 and length == 2:
                ii = ii[1:]
            while len(ii) < length:
                ii = "0" + ii
            temp.append(ii)
            length -= 1     
        s[2], s[4] = temp

        #added to find max timepoint and plain to edit matlab parameter file
        if int(s[2]) > max_time:
            max_time = int(s[2])
        if int(s[4]) > max_planes:
            max_planes = int(s[4])

        s[0] = date+s[0][-3:]
        newName = s[0]
        newfA[idx] = "".join(s)

    #print("max time {}, Max_ Planes{}".format(max_time, max_planes) )
    editfile(file=os.environ['mlParamLoc'],
                parameter=(('end_time',max_time),('slices',max_planes)))
    return newfA, newName


def main(): 
    #tifdir = sys.argv[1]
    tifdir = "/media/zachlab/Windows/LinuxStorage/images/archive/temp/"
    filelist = os.listdir(tifdir)
    print(filelist)
    matches = groupfile(filelist)
    match = [[tifdir+filelist[x] for x in i] for i in matches]
    Dir = [[filelist[x] for x in i] for i in matches]
    for ID ,i in enumerate(match):
        path, filelist = [], []
        for ii in range(len(i)):
             sp, sf = Makefilelist(i[ii], "*.tif", False)
             path.append(sp)
             filelist.append(sf) 
        path = transpose(path)
        filelist = transpose(filelist)
        date =  datetime.datetime.today().strftime('%Y-%m-%d-%H-%M-')

        # for each set of matching files
        for fidx, File in enumerate(filelist):
            if None in File:
                idxlen = File.index(None)
                File = File[0:idxlen]
                newFile, outDirName = rename(File, 61, date)
                inDir = path[fidx][0:idxlen]
                outDir = [iii.replace(Dir[ID][inDir.index(iii)], outDirName) for iii in inDir]
            else:
                newFile, outDirName = rename(File, 61, date)
                if not os.path.isdir(tifdir+outDirName):
                    os.mkdir(tifdir+outDirName)
                    os.mkdir(tifdir+outDirName+"/tif")
                    os.mkdir(tifdir+outDirName+"/tifR")
                inDir = path[fidx]
                outDir = [iii.replace(Dir[ID][path[fidx].index(iii)], outDirName) for iii in inDir]
            
            for idx in range(len(File)):
                #print("{}/{}".format(inDir[idx],File[idx])+", "+"{}/{})".format(outDir[idx],newFile[idx]))
                shutil.move("{}/{}".format(inDir[idx],File[idx]), "{}/{}".format(outDir[idx],newFile[idx]))
        for ii in i:
            shutil.rmtree(ii)

if __name__ == "__main__":
    main()
