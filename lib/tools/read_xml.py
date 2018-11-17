import pandas as pd
import xml.etree.ElementTree as ET
from lxml import etree

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
            

mydict = xmldata('Input.xml')
mydict.wrxml("filename.xml")


