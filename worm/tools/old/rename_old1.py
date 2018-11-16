
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
    #filelist = ["c:1-2 z:10-67 t:10-60 - ChannelGFP,TxRed_Seq0000.nd2 (series.tif"]

    for i in filelist:
        s = i
        s = s.split("-60 - ")
        s[1:1] = s[1].split(".nd2 ")
        del s[3]
        s = breakup(s, ".tif", 2)
        del s[2]
        s[0], s[1] = s[1], s[0]
        s = breakup(s, "-67 t:", 1)
        s = breakup(s, "c:1-2 z:", 1)
        
        temp = []
        length = 3
        for ii in s[4], s[2]:
            while len(ii) < length:
                ii = "0" + ii
            temp.append(ii)
            length -= 1
            
        s[4], s[2] = temp
        s[1:5] = ["-t", s[4], "-p", s[2]]
        s = "".join(s)
        print(s)
        os.rename(tifdir+i, tifdir+s)

main()
