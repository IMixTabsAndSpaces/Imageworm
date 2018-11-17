import time
import sys
import getpass
import os

start_time = time.time()
print("RUNNING: %s" % sys.argv[0])
user = getpass.getuser()
targetDir = "/mnt/Disk_2/work/images"
print("OUTPUTTING TO: {}".format(targetDir))
lifFile = sys.argv[1]
mlParamLoc = sys.argv[2]
lifPrefix, extention = os.path.splitext(lifFile)
#check to see if lifFile has extention .lif
if len(sys.argv) < 3:
    sys.exit("please specify a .lif file and a paramater file") 
elif extention != ".lif":
    sys.exit("arg[1] is not a .lif file")

path = os.path.dirname(lifPrefix)
DIR = lifPrefix
seriesStem = os.path.basename(lifPrefix)
print(path, seriesStem)

print("Extracting images from: {}".format(lifFile))
os.system("java -Xms2g -Xmx5g -cp /mnt/Disk_2/work/tools/worm.jar edu.murrlab.image.translate.LeicaImporter {}".format(lifFile))

os.system("mv {}*/ {}".format(DIR, targetDir))
for x in os.listdir(targetDir):
    print(x)
    os.system("cp {} {}/{}/matlabParams\n".format(mlParamLoc, targetDir, x)) 
    os.chdir("/mnt/Disk_2/work/tools/")
    os.system("./standard_pipeline.sh {}/{}\n".format(targetDir, x ,x)); 
end_time = time.time() - start_time
print("Finished in {}".format(end_time))
