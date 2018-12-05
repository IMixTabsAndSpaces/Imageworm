#!/usr/bin/env python
from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal, QItemSelectionModel)
from PyQt5.QtGui import QIcon, QKeySequence
import PyQt5.QtGui as QtGui
from PyQt5.QtWidgets import (QAction, QApplication, QFileDialog, QMainWindow,
        QMessageBox, QTextEdit, QVBoxLayout, QGroupBox, QHBoxLayout, QPushButton,
        QLineEdit, QDialog, QWidget, QTableWidget, QLabel, QPushButton, QComboBox,
        QTreeWidget, QTreeWidgetItem, QDialogButtonBox, QFormLayout, QMenu, QMenuBar,
        QGridLayout, QSpinBox, QAbstractScrollArea, QSizePolicy, QHeaderView,
        QTableWidgetItem, QAbstractItemView)
from PyQt5.QtSql import QSqlTableModel
import DB_Manager, sys, os
from helpers.subMenu import AddDialog
from helpers.rerunMenu import runMenu
from helpers.FSConvert import zipMove, unZipMove
#=============================================================================

class MainWindow(QMainWindow):
    root = QFileInfo(__file__).absolutePath()

    def __init__(self):
        super(MainWindow, self).__init__()
        #this is just a default and not an required to be set
        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'
        self.isID=False
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

        self.tableWidget = QTableWidget()
        self.tableWidget.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.tableWidget.setShowGrid(False)
        self.tableWidget.verticalHeader().hide()
        self.tableWidget.itemSelectionChanged.connect(self.loadSelectedID)
        self.tableWidget.setSortingEnabled(True)
        self.tableWidget.setEditTriggers(QAbstractItemView.NoEditTriggers)
        #self.tableWidget.setSizeAdjustPolicy(QAbstractScrollArea.AdjustToContents)
        #self.tableWidget.horizontalHeader().setStretchLastSection(False)
        #self.tableWidget.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        #self.tableWidget.selectionModel().selectionChanged.connect(self.calledMethod)

        #self.horizontalLayout = QHBoxLayout()
        #self.horizontalLayout.addWidget(self.commitButton)
        #self.verticalLayout.addLayout(self.horizontalLayout, 0, 0)
        self.grid = QGridLayout()
        self.grid.addWidget(self.tableWidget, 0, 0)
        self.createSideMenu()
        self.createEditMenu()
        self.grid.addWidget(self.sideMenu, 0, 1)
        self.grid.addWidget(self.editmenu, 0, 1)
        self.dataTable.setLayout(self.grid)
        self.UpdateTree()

    def createCenterWidget(self):
        mainLayout = QVBoxLayout(self)
        mainLayout.addWidget(self.dataTable)
        self.centerWiget = QWidget(self)
        self.centerWiget.setLayout(mainLayout)

    def createSideMenu(self):
        self.Menu = QGroupBox("Menu Tools")
        sideMenuLayout = QGridLayout()
        updateButton = QPushButton("Update DB")
        updateButton.clicked.connect(self.UpdateDB)
        addButton = QPushButton("Add Entry")
        addButton.clicked.connect(self.processEntry)
        rerunButton = QPushButton("Rerun Entry")
        rerunButton.clicked.connect(self.rerunEntry)
        treeButton = QPushButton("Print Tree")
        AcetreeButton = QPushButton("Launch AceTree")
        AcetreeButton.clicked.connect(self.LaunchAceTree)
        ArchiveButton = QPushButton("Archive")
        ArchiveButton.clicked.connect(self.archiveEntry)
        unArchiveButton = QPushButton("Retrieve")
        unArchiveButton.clicked.connect(self.RetrieveEntry)
        sideMenuLayout.addWidget(updateButton, 0, 0, 1, 2)
        sideMenuLayout.addWidget(addButton, 1, 0)
        sideMenuLayout.addWidget(rerunButton, 1, 1)
        sideMenuLayout.addWidget(ArchiveButton, 2,0)
        sideMenuLayout.addWidget(unArchiveButton, 2, 1)
        sideMenuLayout.addWidget(AcetreeButton, 3, 0)
        sideMenuLayout.addWidget(treeButton, 3, 1)
        sideMenuLayout.setAlignment(Qt.AlignTop)
        self.Menu.setLayout(sideMenuLayout)
        
        self.filters = QGroupBox("Add filter")
        layout = QHBoxLayout()
        layout.addWidget(QTextEdit())
        self.filters.setLayout(layout)

        self.sideGrid = QGridLayout()
        self.sideGrid.addWidget(self.Menu, 0, 0)
        self.sideGrid.addWidget(self.filters, 1, 0)
        self.sideMenu = QWidget()
        self.sideMenu.setMaximumWidth(300)
        self.sideMenu.setLayout(self.sideGrid)
        #self.sideMenuLayout.addWidget(self.label, 0, 0)

    def createEditMenu(self):
        self.commitButton = QPushButton("Commit Edit")
        self.commitButton.clicked.connect(self.CommitEdit)
        cancelButton = QPushButton("Cancel")
        cancelButton.clicked.connect(self.showSideMenu)
        self.editmenu = QGroupBox()
        editMenuLayout = QVBoxLayout()
        editMenuLayout.addWidget(self.commitButton)
        editMenuLayout.addWidget(cancelButton)
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
        self.editmenu.setMaximumWidth(300)
        self.editmenu.setHidden(True)

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
    
    def showSideMenu(self):
        self.editmenu.setHidden(True)
        self.sideMenu.setHidden(False)
    
    def showEditMenu(self):
        self.sideMenu.setHidden(True)
        self.editmenu.setHidden(False)
        #if self.isID:
        #    values = self.dbu.GetRow(self.getSelectedID)
        #    for i, key in enumerate(self.editLabels):
        #        self.editLabels[key].setText(str(values[0][i]))
    
    def loadSelectedID(self, col=0):
        getSelected = self.tableWidget.selectedItems()
        if getSelected:
            baseNode = getSelected[col]
            #self.getSelectedID = baseNode.text(0)
            value = baseNode.text()
            if col == 0:
                self.getSelectedID = value
                self.isID = True
                values = self.dbu.GetRow(self.getSelectedID)
                for i, key in enumerate(self.editLabels):
                    self.editLabels[key].setText(str(values[0][i]))
            else:
                return value
            
    def CommitEdit(self):
        results = {}
        for key in self.editLabels:
            results[key] = self.editLabels[key].text()

        self.dbu.editTableEntry(results, self.getSelectedID)
        self.UpdateTree()
        self.showSideMenu()
           
    def UpdateTree(self):
        col = self.dbu.GetColumns()
        table = self.dbu.GetTable()
        self.tableWidget.setRowCount(len(table))
        self.tableWidget.setColumnCount(len(col))
        self.model = QSqlTableModel(self)
        self.tableWidget.clear()
        for c in range(len(col)):
            self.tableWidget.setHorizontalHeaderItem(c, QTableWidgetItem(col[c][0]))
            #self.tableWidget.headerItem().setText(c, col[c][0])

        
        #self.tableWidget.setSortingEnabled(False)
        for item in range(len(table)):
            #QTableWidgetItem(self.tableWidget)
            for value in range(len(table[item])):
                self.tableWidget.setItem(item, value, QTableWidgetItem(str(table[item][value])))

        #for i in range(len(self.dbu.GetColumns())):
        #    self.tableWidget.resizeColumnToContents(i)
        header = self.tableWidget.horizontalHeader()
        
        #for i in range(len(self.dbu.GetColumns())):
        #    header.setSectionResizeMode(i, QHeaderView.Stretch)

            #self.tableWidget.resizeColumnToContents(i, QHeaderView.Stretch)

    def readSettings(self):
        settings = QSettings("Trolltech", "Application Example")
        pos = settings.value("pos", QPoint(200, 200))
        size = settings.value("size", QSize(400, 400))
        self.resize(size)
        self.move(pos)
        self.setGeometry(0, 0, 2040, 1080)

    def addEntry(self):
        self.dbu.AddEntryToTable()
        self.UpdateTree()
        #self.SelectIdRow()
        self.showEditMenu()

    def processEntry(self):
        self.AddEntry = AddDialog()
        def endprocess(*args, **kwargs):
            self.dbu.AddXmlToTable(self.AddEntry.outfiles)
            self.UpdateTree()

        self.AddEntry.accepted.connect(endprocess)
        self.AddEntry.show()
    
    def rerunEntry(self):
        rerun = runMenu()
        rerun.exec_()
    
    def archiveEntry(self):
        if self.isID:
            zipMove(self.annots, os.environ['archiveDir'])
            self.dbu.editTableEntry({'status': str(0)}, self.getSelectedID)
            self.UpdateTree()

    def RetrieveEntry(self):
        if self.isID:
            unZipMove(self.series, os.environ['targetDir'])
            self.dbu.editTableEntry({'status': str(1)}, self.getSelectedID)
            self.UpdateTree()

    def LaunchAceTree(self):
        pathAceTree = os.path.join(os.environ['IW_LIB'], 'AceTree/AceTree.jar')
        cmd = ('java -jar ' + pathAceTree + ' ' +self.annots+ '/dats/'+self.acetree)
        os.system(cmd)

    def UpdateDB(self):
        series_names = self.dbu.GetCol("series")
        add_file = []
        
        local_dir = os.environ['targetDir']
        local_vids = os.listdir(local_dir)

        for v in local_vids:
            if v not in series_names:
                add_file.append(os.path.join(local_dir, v))

        archive_dir = os.environ['archiveDir']
        archive_vids = os.listdir(archive_dir)
        for v in archive_vids:
            if v not in series_names:
                add_file.append(os.path.join(archive_dir, v))
        print(add_file)
        self.dbu.AddXmlToTable(add_file)
        self.UpdateTree()

    def delRow(self):
        if self.isID:
            self.dbu.delEntry(self.getSelectedID)
        self.UpdateTree()

    @property
    def annots(self):
        return self.loadSelectedID(9)
    
    @property
    def acetree(self):
        return self.loadSelectedID(10)
    
    @property
    def series(self):
        return self.loadSelectedID(1)

if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())
