import os
import fileinput
import time
from fnmatch import fnmatch

def ReplaceAll(filelist, old, new):
    for i, x in enumerate(filelist):
        filelist[i] = x.replace(old, new)
    return filelist


def transpose(array):
    array = list(map(list, zip(*array)))
    return array


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


def editfile(file="", parameter=None):
    """Searches file and sets the associated string equal to your second value.
    
    Ex: editfile(file=file, parameter=(('end_time','10'),....))
    file: is the target parameter file to be edited.
    parameter: is a list or tuple of sets of what to change.
    """
    def newLine(line):
        for par in parameter:
            if line.startswith(par[0]):
                return "{}={};".format(par[0],par[1])
        return line.strip()
        
    if not all(len(p) == 2 for p in parameter):
        raise RuntimeError("editfile() parameter set is not equal to 2")
    for line in fileinput.input(file,inplace=True):
        print(newLine(line))


def Makefilelist(root, pattern, fullpath):
    """ Returns all files under the root direcctory with matching pattern. """
    paths, filenames = [], []
    for path, subdirs, files in os.walk(root):
        if fullpath:
            for name in files:
                if fnmatch(name, pattern):
                    filenames.append("{}/{}".format(path,name))
        else:
            for name in files:
                if fnmatch(name, pattern):
                    paths.append(path)
                    filenames.append(name)
    if fullpath:
        return filenames
    else:
        return paths, filenames

def groupfile(filelist):
    """ group files by last 4 char in order of the number right before those last 4 char"""
    seq, ext, matches = [], [], []

    # break up folder names into sequence number and embryo tag
    for i in filelist:
        seqnum = ""
        for ii in i[-7:-4]:
            if ii.isdigit():
                seqnum +=ii
        seq.append(int(seqnum))
        ext.append(i[-4:])

    #makes a list of sorted list of all matches in ext
    for i in ext:
        matches.append([ii for ii, x in enumerate(ext) if x == i])
    matches.sort()
    matches = [sorted(matches[i]) for i in range(len(matches)) if i == 0 or matches[i] != matches[i-1]]
    return matches

def Timetrack(orginal_func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = orginal_func(*args, **kwargs)
        end_time = time.time() - start_time

        if end_time >= 60:
            end_time /= 60
            print("Process {0} Finished in {1:.3f} min.".format(orginal_func.__name__,end_time))
        else:
            print("Process {0} Finished in {1:.3f} sec.".format(orginal_func.__name__,end_time))
        return result
    return wrapper


def logger(orig_func):
    import logging
    logging.basicConfig(filename='{}.log'.format(orig_func.__name__), level=logging.INFO)

    def wrapper(*args, **kwargs):
        logging.info(
            'Ran with args: {}, and kwargs: {}'.format(args, kwargs))
        return orig_func(*args,**kwargs)
    return wrapper
