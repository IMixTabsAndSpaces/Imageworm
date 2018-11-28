#!/usr/bin/env python

from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal, QItemSelectionModel)
from PyQt5.QtGui import QIcon, QKeySequence
import PyQt5.QtGui as QtGui
from PyQt5.QtWidgets import (QAction, QApplication, QFileDialog, QMainWindow,
        QMessageBox, QTextEdit, QVBoxLayout, QGroupBox, QHBoxLayout, QPushButton,
        QLineEdit, QDialog, QWidget, QTableWidget, QLabel, QPushButton, QComboBox,
        QTreeWidget, QTreeWidgetItem, QDialogButtonBox, QFormLayout, QMenu, QMenuBar,
        QGridLayout, QSpinBox, QAbstractScrollArea)

import DB_Manager, sys, os
from subMenu import AddDialog
class MainWindow(QMainWindow):
    root = QFileInfo(__file__).absolutePath()

    def __init__(self):
        super(MainWindow, self).__init__()
        #this is just a default and not an required to be set
        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'

        #create center widget stuff
        self.createActions()
        self.createTabel()
        self.createCenterWidget()        
        self.setCentralWidget(self.centerWiget)

        #set up application stuff
        self.createActions()
        self.createMenus()
        self.createToolBars()
        self.createStatusBar()
        self.setWindowTitle("IMAGEWORM")
        self.readSettings()
    
    def createTabel(self):
        self.dataTable = QWidget()
        database = "test2"
        tableName = "worms"
        self.dbu = DB_Manager.DatabaseUtility(database, tableName)
        self.label = QLabel("Label")
        self.label.setAlignment(Qt.AlignCenter)
        self.treeWidget = QTreeWidget()
        self.treeWidget.itemSelectionChanged.connect(self.loadSelectedID)
        #self.treeWidget.selectionModel().selectionChanged.connect(self.calledMethod)
        self.commitButton = QPushButton("CommitEdit")
        self.commitButton.clicked.connect(self.CommitEdit)
        self.editButton = QPushButton("Edit")
        self.editButton.clicked.connect(self.showEditMenu)
        self.verticalLayout = QGridLayout()
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.addWidget(self.commitButton)
        self.horizontalLayout.addWidget(self.editButton)
        self.verticalLayout.addLayout(self.horizontalLayout, 0, 0)
        self.verticalLayout.addWidget(self.label, 1, 0)
        self.menu = QWidget()
        self.menu.setMaximumWidth(300)
        self.menu.setLayout(self.verticalLayout)
        self.grid = QGridLayout()
        self.grid.addWidget(self.treeWidget, 0, 0)
        self.grid.addWidget(self.menu, 0, 1)
        self.dataTable.setLayout(self.grid)
        self.UpdateTree()
    
    def showEditMenu(self):
        self.editmenu = QWidget()
        editMenuLayout = QVBoxLayout()
        self.editLabels = {}
        col = self.dbu.GetColumns()
        for c in range(len(col)):
            horizontalLayout = QHBoxLayout()
            lineEdit = QLineEdit()
            label = QLabel(col[c][0]+":")
            horizontalLayout.addWidget(label)
            horizontalLayout.addWidget(lineEdit)
            tempW = QWidget()
            tempW.setLayout(horizontalLayout)
            self.editLabels[col[c][0]] = lineEdit
            editMenuLayout.addWidget(tempW)

        self.editmenu.setLayout(editMenuLayout)
        self.label.setHidden(True)
        self.verticalLayout.addWidget(self.editmenu, 1, 0)
        if self.isID:
            self.dbu.GetRow
            for key in self.editLabels:
            results[key] = self.editLabels[key].text()
    
    def loadSelectedID(self):
        getSelected = self.treeWidget.selectedItems()
        if getSelected:
            baseNode = getSelected[0]
            self.getSelectedID = baseNode.text(0)
            self.isID = True
            print(self.getSelectedID)

    def CommitEdit(self):
        results = {}
        for key in self.editLabels:
            results[key] = self.editLabels[key].text()

        self.dbu.editTableEntry(results)
        self.UpdateTree()

    def UpdateTree(self):
        col = self.dbu.GetColumns()
        table = self.dbu.GetTable()
        for c in range(len(col)):
            self.treeWidget.headerItem().setText(c, col[c][0])

        self.treeWidget.clear()
        for item in range(len(table)):
            QTreeWidgetItem(self.treeWidget)
            for value in range(len(table[item])):
                self.treeWidget.topLevelItem(item).setText(value, str(table[item][value]))

    def createCenterWidget(self):
        mainLayout = QVBoxLayout(self)
        mainLayout.addWidget(self.dataTable)
        self.centerWiget = QWidget(self)
        self.centerWiget.setLayout(mainLayout)

    def createActions(self): 
        self.processAct = QAction(QIcon(MainWindow.root + '/images/open.png'), "&Process", self,
                shortcut=QKeySequence.New, statusTip="Create new DB Entry",
                triggered=self.processEntry)

        self.addAct = QAction(QIcon(MainWindow.root+'/images/new.png'), "&Add", self,
                shortcut="Ctrl+P", statusTip="Manually add DB Entry", 
                triggered=self.addEntry)
        
        self.delAct = QAction(QIcon(MainWindow.root + '/images/Delete.png'), "&Delete", self,
                shortcut="Ctrl+x", statusTip="delet DB Entry: Ctrl+x",
                triggered=self.delRow)
        
        self.editAct = QAction(QIcon(MainWindow.root+ '/images/edit.png'), "&Edit", self, 
                shortcut="Ctrl+E", statusTip="Edit Entry: Ctrl+E",
                triggered=self.showEditMenu)
        
        self.exitAct = QAction("E&xit", self, shortcut="Ctrl+Q",
                statusTip="Exit the application", triggered=self.close)

        self.commit = QAction("&Refresh", self, triggered=self.update)

    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu("&File")
        self.fileMenu.addAction(self.processAct)
        self.fileMenu.addAction(self.addAct)
        self.fileMenu.addAction(self.editAct)
        self.fileMenu.addAction(self.exitAct)

    def createToolBars(self):
        self.fileToolBar = self.addToolBar("File")
        self.fileToolBar.addAction(self.processAct)
        self.fileToolBar.addAction(self.addAct)
        self.fileToolBar.addAction(self.editAct)
        self.fileToolBar.addAction(self.delAct)
   
    def createStatusBar(self):
        self.statusBar().showMessage("Ready")
    
    def readSettings(self):
        settings = QSettings("Trolltech", "Application Example")
        pos = settings.value("pos", QPoint(200, 200))
        size = settings.value("size", QSize(400, 400))
        self.resize(size)
        self.move(pos)
        self.setGeometry(0, 0, 2040, 1080)
    '''
    def calledMethod(self,newIndex,oldIndex=None):
        try: #if qItemSelection
            newIndex=newIndex.indexes()[0]
        except: #if qModelIndex
            pass

    def SelectIdRow(self):
        widget=self.treeWidget
        itemOrText='3'
        #oldIndex=widget.selectionModel().currentIndex()
        try: #an item is given--------------------------------------------
            newIndex=widget.model().indexFromItem(itemOrText)
            print(newIndex)
        except: #a text is given and we are looking for the first match---
            listIndexes=widget.model().match(widget.model().index(0, 0),
                            Qt.DisplayRole,
                            itemOrText,
                            Qt.MatchStartsWith)
            try:
                newIndex=listIndexes[0]
            except:
                return
        widget.selectionModel().select( #programmatical selection---------
                newIndex,
                QItemSelectionModel.ClearAndSelect)
    '''
    def addEntry(self):
        self.dbu.AddEntryToTable()
        self.UpdateTree()
        self.SelectIdRow()
        self.showEditMenu()

    def processEntry(self):
        AddEntry = AddDialog()
        AddEntry.exec_()
        self.dbu.AddXmlToTable(AddEntry.outfiles)
        self.UpdateTree()
        
    
    def delRow(self):
        if self.isID:
            self.dbu.delEntry(self.getSelectedID)
        self.UpdateTree()

if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())

