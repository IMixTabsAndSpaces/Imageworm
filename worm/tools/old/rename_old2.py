

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
#    filelist = ["test_Z00_T00.tif"]

    for i in filelist:
        s = [i]
        s = breakup(s, "_Z", 0)
        s = breakup(s, "_T", 2)
        s = breakup(s, ".tif", 4)
        s[2], s[4] = s[4], s[2]
        s[1] = '-t'
        s[3] = '-p'
        temp = []
        length = 3
        for ii in s[2], s[4]:
            while len(ii) < length:
                ii = "0" + ii
            temp.append(ii)
            length -= 1
          
        s[2], s[4] = temp
        s = "".join(s)
        print(s)
        os.rename(tifdir+i, tifdir+s)

main()
