import time
import sys
import getpass
import os

start_time = time.time()
print("RUNNING: %s" % sys.argv[0])

user = getpass.getuser()
targetDir = "/home/zachlab/ImageWorm/images/output"
print("OUTPUTTING TO: {}".format(targetDir))
nd2File = "/home/zachlab/ImageWorm/work/images/raw/"  #sys.argv[1]
mlParamLoc = "/home/zachlab/ImageWorm/images/matlabParams" #sys.argv[2]
nd2Prefix, extention = os.path.splitext(nd2File)
#check to see if lifFile has extention .lif
#if len(sys.argv) < 3:

#    sys.exit("please specify a .lif file and a paramater file") 
if extention != ".nd2":
    sys.exit("arg[1] is not a .lif file")

path = os.path.dirname(nd2Prefix)
DIR = nd2Prefix
seriesStem = os.path.basename(nd2Prefix)
print(path, seriesStem)

#print("Extracting images from: {}".format(lifFile))
#os.system("java -Xms2g -Xmx5g -cp /mnt/Disk_2/work/tools/worm.jar edu.murrlab.image.translate.LeicaImporter {}".format(lifFile))

os.system("cp -rf {}*/ {}".format(DIR, targetDir))
os.chdir("/mnt/Disk_2/work/tools/")

print("python ./rename.py {}/{}".format(targetDir, seriesStem))


os.system("python ./rename.py {}/{}".format(targetDir, seriesStem))

for x in os.listdir(targetDir):
    print("value of x: {}".format(x))
    os.system("cp {} {}/{}/matlabParams\n".format(mlParamLoc, targetDir, x)) 
    os.chdir("/mnt/Disk_2/work/tools/")
    print("./standard_pipeline.sh {}/{} {}\n".format(targetDir, x ,x))
    os.system("./standard_pipeline.sh {}/{} {}\n".format(targetDir, x ,x)); 

end_time = time.time() - start_time
print("Finished in {}".format(end_time))

