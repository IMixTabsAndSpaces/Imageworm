
import os
import sys


def ReplaceAll(filelist, old, new):
    for i in filename:
        i.replace(old, new)
    return filelist

def breakup(array, s, idx):
    temp = []
    trip = array[idx].rpartition(s)
    if trip[0] == '':
        temp.extend(trip[1:3])
    elif trip[2] == '':
        temp.extend(trip[0:2])
    else:
        temp.extend(trip)
    array[idx:idx+1] = temp
    return array 
    
def main():

    tifdir = sys.argv[1]
    #channel = sys.argv[2]
    filelist = os.listdir(tifdir)
    
    for i in filelist:
        s = i.split(".nd2 - ")
        s = breakup(s, s[0], 1)
        del s[1]
        s = breakup(s, ".tif", 1)
        s = breakup(s, "_z0", 1)
        s = breakup(s, "_t", 1)
        s[1] = '-t'
        s[3] = '-p'
        s = "".join(s)
        s = "".join(s)
        print(s)
        os.rename(tifdir+i, tifdir+s)

main()
