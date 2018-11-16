
import os
import sys


def ReplaceAll(filelist, old, new):
    for i in filename:
        i.replace(old, new)
    return filelist

def breakup(array, s, idx):
    temp = []
    temp.extend(array[idx].rpartition(s))
    array[idx:idx+1] = temp
    return array 
    
def main():
    tifdir = sys.argv[1]
    channel = sys.argv[2]
    filelist = os.listdir(tifdir)
    for i in filelist:
        #s = i.replace("L100", "L1")
        s = [i]
        s = s[0].split(channel)
        s = breakup(s, "z", 0)
        s = breakup(s,"_xy05t",0)
        s[3] = "-p"
        s[1] = "-t0"
        s = "".join(s)
        print(s)
        os.rename(tifdir+i, tifdir+s)

main()
