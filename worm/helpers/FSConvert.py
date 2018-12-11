import datetime
import getpass
from lxml import etree
import xml.etree.ElementTree as ET
import pandas as pd
import os
import fileinput
import time
from fnmatch import fnmatch
import shutil
import zipfile
import sys
# ==============================================================


def Timetrack(orginal_func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = orginal_func(*args, **kwargs)
        end_time = time.time() - start_time

        if end_time >= 60:
            end_time /= 60
            print("Process {0} Finished in {1:.3f} min.".format(
                orginal_func.__name__, end_time))
        else:
            print("Process {0} Finished in {1:.3f} sec.".format(
                orginal_func.__name__, end_time))
        return result
    return wrapper


def logger(orig_func):
    import logging
    logging.basicConfig(filename='{}.log'.format(
        orig_func.__name__), level=logging.INFO)

    def wrapper(*args, **kwargs):
        logging.info(
            'Ran with args: {}, and kwargs: {}'.format(args, kwargs))
        return orig_func(*args, **kwargs)
    return wrapper


@Timetrack
def zipMove(Dpath, outF):
    print("Archive image ....")
    if not os.path.isdir(Dpath):
        raise FileExistsError("Dir Does not exist")
    Dname = os.path.basename(Dpath)
    shutil.make_archive(Dname, 'zip', Dpath)
    #print("moooving file - its bean a long day")
    #shutil.move(Dname+'.zip', outF)


class ProgressBar(object):
    def __init__(self, message, width=20, progressSymbol=u'▣ ', emptySymbol=u'□ '):
        self.width = width

        if self.width < 0:
            self.width = 0

        self.message = message
        self.progressSymbol = progressSymbol
        self.emptySymbol = emptySymbol

    def update(self, progress):
        totalBlocks = self.width
        filledBlocks = int(round(progress / (100 / float(totalBlocks))))
        emptyBlocks = totalBlocks - filledBlocks

        progressBar = self.progressSymbol * filledBlocks + \
            self.emptySymbol * emptyBlocks

        if not self.message:
            self.message = u''

        progressMessage = u'\r{0} {1}  {2}%'.format(self.message,
                                                    progressBar,
                                                    progress)

        sys.stdout.write(progressMessage)
        sys.stdout.flush()

    def calculateAndUpdate(self, done, total):
        progress = int(round((done / float(total)) * 100))
        self.update(progress)


def countFiles(directory):
    files = []

    if os.path.isdir(directory):
        for path, dirs, filenames in os.walk(directory):
            files.extend(filenames)

    return len(files)


def makedirs(dest):
    if not os.path.exists(dest):
        os.makedirs(dest)


def moveFilesWithProgress(src, dest, p):
    numFiles = countFiles(src)

    if numFiles > 0:
        makedirs(dest)

        numCopied = 0

        for path, dirs, filenames in os.walk(src):
            for directory in dirs:
                destDir = path.replace(src, dest)
                makedirs(os.path.join(destDir, directory))

            for sfile in filenames:
                srcFile = os.path.join(path, sfile)

                destFile = os.path.join(path.replace(src, dest), sfile)

                shutil.copy(srcFile, destFile)

                numCopied += 1

                p.calculateAndUpdate(numCopied, numFiles)
            print('\n')


def unZipMove(zpath, dpath):
    zfile = os.path.basename(zpath)
    zdir = os.path.splitext(zfile)[0]
    os.mkdir(os.path.join(dpath, zdir))

    zip = zipfile.ZipFile(zpath)
    zip.extractall(os.path.join(dpath, zdir))


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
                return "{}={};".format(par[0], par[1])
        return line.strip()

    if not all(len(p) == 2 for p in parameter):
        raise RuntimeError("editfile() parameter set is not equal to 2")
    for line in fileinput.input(file, inplace=True):
        print(newLine(line))


def Makefilelist(root, pattern, fullpath):
    """ Returns all files under the root direcctory with matching pattern. """
    paths, filenames = [], []
    for path, subdirs, files in os.walk(root):
        if fullpath:
            for name in files:
                if fnmatch(name, pattern):
                    filenames.append("{}/{}".format(path, name))
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
                seqnum += ii
        seq.append(int(seqnum))
        ext.append(i[-4:])

    # makes a list of sorted list of all matches in ext
    for i in ext:
        matches.append([ii for ii, x in enumerate(ext) if x == i])
    matches.sort()
    matches = [sorted(matches[i]) for i in range(len(matches))
               if i == 0 or matches[i] != matches[i-1]]
    return matches


# ===============================================================


class xmldata():
    def __init__(self, file=False):
        self.Keys = []
        self.labels = []
        self.Values = []
        if file:
            self.loadxml(file)

    def __iter__(self):
        for i in range(len(self.Keys)):
            yield self.Keys[i], self.labels[i], self.Values[i]

    def _del(self, idx=True):
        if type(idx) == bool:
            self.Keys.clear()
            self.labels.clear()
            self.Values.clear()
        else:
            del self.Keys[idx]
            del self.labels[idx]
            del self.Values[idx]

    def __str__(self):
        for k, l, V in self:
            print(k, l, V)
        return "key label value"

    def _append(self, key, label, value):
        self.Keys.append(key)
        self.labels.append(label)
        self.Values.append(value)

    def loadxml(self, filename):
        tree = ET.parse(filename)  # create an ElementTree object
        root = tree.getroot()
        for child in root:
            self._append(child.tag, list(child.keys())[
                         0], list(child.attrib.values())[0])

    def wrxml(self, outfile="output.xml"):
        ROOT = etree.Element("experiment")
        for key, label, Value in self:
            etree.SubElement(ROOT, key, {label: Value})
        ctree = etree.ElementTree(ROOT)
        ctree.write(outfile, pretty_print=True,
                    xml_declaration=True, encoding='utf-8')

    def mysqlentry(self, table_name=str):

        def go_up(path, n):
            for i in range(n):
                path = os.path.dirname(path)

            return path
        self._del(-1)
        timepts = self.Values[self.Keys.index('end')]
        timepts.zfill(3)
        first_img = self.Values[self.Keys.index('image')]
        imageloc = go_up(first_img, 2)
        annots = imageloc
        series = os.path.basename(imageloc)
        acetree = series + '.xml'
        status = 'new'
        person = getpass.getuser()
        date = datetime.datetime.today().strftime('%Y%m%d')
        self._del()
        KEYS = ('series', 'date', 'person', 'strain', 'treatments', 'redsig',
                'imageloc', 'timepts', 'annots', 'acetree', 'editedby', 'editedtimepts',
                'editedcells', 'checkedby', 'comments', 'status')

        LABEL = ('name', 'date', 'name', 'name', 'desc', 'value',
                 'loc', 'num', 'loc', 'config', 'name', 'num',
                 'num', 'name', 'text', 'case')

        VALUES = (series, date, person, 'n/a', 'n/a', 'n/a',
                  imageloc, timepts, annots, acetree, 'n/a', 'n/a',
                  'n/a', 'n/a', 'n/a', 'n/a')
        for i in range(len(KEYS)):
            self._append(KEYS[i], LABEL[i], VALUES[i])

        return self.mysqlentryRaw(table_name)

    def mysqlentryRaw(self, table_name=str):
        cmd = 'INSERT INTO {} '.format(table_name)
        cmd += str(tuple(k for k, l, v in self if not v == 'n/a')
                   ).replace("'", "")
        cmd += ' VALUES '
        cmd += str(tuple(v for k, l, v in self if not v == 'n/a'))
        cmd += ';'
        return cmd


if __name__ == "__main__":
    zipMove("/media/zachlab/Windows/LinuxStorage/images/archive/2018-11-30-08-41-xy2",
            "/media/zachlab/Windows/LinuxStorage/images/archive")
    # unZipMove('/media/zachlab/Windows/LinuxStorage/old/embryoDB.zip','/media/zachlab/Windows/LinuxStorage/old/')
