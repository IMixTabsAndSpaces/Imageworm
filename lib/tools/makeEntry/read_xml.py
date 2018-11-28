import pandas as pd
import xml.etree.ElementTree as ET
from lxml import etree
import os
import getpass
import datetime
#===============================================================
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

    def _del(self,idx=True):
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
        tree = ET.parse(filename) #create an ElementTree object 
        root = tree.getroot()
        for child in root:
            self._append(child.tag,list(child.keys())[0], list(child.attrib.values())[0])

    def wrxml(self, outfile="output.xml"):
        ROOT = etree.Element("experiment")
        for key, label, Value in self:
            etree.SubElement(ROOT, key, {label: Value})
        ctree = etree.ElementTree(ROOT)
        ctree.write(outfile ,pretty_print=True,xml_declaration=True, encoding='utf-8')

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
            'imageloc','timepts', 'annots', 'acetree', 'editedby', 'editedtimepts',
            'editedcells', 'checkedby', 'comments', 'status')

        LABEL = ('name', 'date', 'name', 'name', 'desc', 'value',
            'loc','num', 'loc', 'config', 'name', 'num',
            'num', 'name', 'text', 'case')
        
        VALUES = (series, date, person, 'n/a', 'n/a', 'n/a',
            imageloc, timepts, annots, acetree, 'n/a', 'n/a',
            'n/a', 'n/a', 'n/a', 'new')
        for i in range(len(KEYS)):
            self._append(KEYS[i],LABEL[i],VALUES[i])

        return self.mysqlentryRaw(table_name)

    def mysqlentryRaw(self, table_name=str):
        cmd = 'INSERT INTO {} '.format(table_name)
        cmd += str(tuple(k for k,l,v in self if not v == 'n/a'))
        cmd += '\nVALUES\n'
        cmd += str(tuple(v for k,l,v in self if not v == 'n/a'))
        return cmd
        
        

#   INSERT INTO table_name ( field1, field2,...fieldN )
#   VALUES
#   ( value1, value2,...valueN );        
            
here = os.path.dirname(os.path.abspath(__file__))
mydict = xmldata('/home/zachlab/Documents/python/Imageworm/lib/tools/makeEntry/2018-11-27-12-17-xy2.xml')
print(mydict)
print(mydict.mysqlentry('table'))
#mydict.wrxml(here+"/filename.xml")
#print(mydict)



