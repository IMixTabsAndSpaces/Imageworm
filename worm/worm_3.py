#!/usr/bin/env python
import os
import sys
import shutil
import threading
import PyQt5.QtGui as QtGui

from PyQt5.QtGui import QIcon, QKeySequence
from PyQt5 import QtWidgets, QtSql, QtCore
import DB_Manager
from helpers import FSConvert
from helpers.rerunMenu import runMenu
from helpers.subMenu import AddDialog, ProgressBar

# =============================================================================


class MainWindow(QtWidgets.QMainWindow):
    root = QtCore.QFileInfo(__file__).absolutePath()
    startMoveFilesSignal = QtCore.pyqtSignal(str, str)
    database = "test2"
    tableName = "worms"

    def __init__(self):
        super(MainWindow, self).__init__()
        # this is just a default and not an required to be set
        self.nd2_file = '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'
        self.isID = False
        # create center widget stuff
        self.createActions()
        self.createTabel()
        self.createCenterWidget()
        self.setCentralWidget(self.centerWiget)

        # set up application stuff
        self.createActions()
        self.createMenus()
        self.createToolBars()
        self.createStatusBar()
        self.setWindowTitle("IMAGEWORM")
        self.readSettings()

        # multi-thread display
        self.progressbar = QtWidgets.QProgressDialog(self)
        self.progressbar.show()
        self.progressbar.hide()

        thread = QtCore.QThread(self)
        thread.start()
        self.helper = MoveFileHelper()
        self.startMoveFilesSignal.connect(self.helper.moveFilesWithProgress)
        self.helper.progressChanged.connect(self.progressbar.setValue)
        self.helper.finished.connect(self.on_finished)
        self.helper.started.connect(self.progressbar.show)
        self.helper.errorOccurred.connect(self.on_errorOcurred)
        self.helper.moveToThread(thread)

    def createTabel(self):
        self.dataTable = QtWidgets.QWidget()
        self.dbu = DB_Manager.DatabaseUtility(MainWindow.database, MainWindow.tableName)
        self.tableWidget = QtWidgets.QTableWidget()
        self.tableWidget.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.tableWidget.setShowGrid(False)
        self.tableWidget.verticalHeader().hide()
        self.tableWidget.itemSelectionChanged.connect(self.loadSelectedID)
        self.tableWidget.setSortingEnabled(True)
        self.tableWidget.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
        # self.tableWidget.setSizeAdjustPolicy(QAbstractScrollArea.AdjustToContents)
        # self.tableWidget.horizontalHeader().setStretchLastSection(False)
        # self.tableWidget.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        # self.tableWidget.selectionModel().selectionChanged.connect(self.calledMethod)

        #self.horizontalLayout = QHBoxLayout()
        # self.horizontalLayout.addWidget(self.commitButton)
        #self.verticalLayout.addLayout(self.horizontalLayout, 0, 0)
        self.grid = QtWidgets.QGridLayout()
        self.grid.addWidget(self.tableWidget, 0, 0)
        self.createSideMenu()
        self.createEditMenu()
        self.grid.addWidget(self.sideMenu, 0, 1)
        self.grid.addWidget(self.editmenu, 0, 1)
        self.dataTable.setLayout(self.grid)
        self.UpdateTree()
    def createCenterWidget(self):
        mainLayout = QtWidgets.QVBoxLayout(self)
        mainLayout.addWidget(self.dataTable)
        self.centerWiget = QtWidgets.QWidget(self)
        self.centerWiget.setLayout(mainLayout)

    def createSideMenu(self):
        self.Menu = QtWidgets.QGroupBox("Menu Tools")
        sideMenuLayout = QtWidgets.QGridLayout()
        updateButton = QtWidgets.QPushButton("Update DB")
        updateButton.clicked.connect(self.UpdateDB)
        addButton = QtWidgets.QPushButton("Add Entry")
        addButton.clicked.connect(self.processEntry)
        rerunButton = QtWidgets.QPushButton("Rerun Entry")
        rerunButton.clicked.connect(self.rerunEntry)
        treeButton = QtWidgets.QPushButton("Print Tree")
        AcetreeButton = QtWidgets.QPushButton("AceTree")
        AcetreeButton.clicked.connect(self.LaunchAceTree)
        NewAcetreeButton = QtWidgets.QPushButton("New AceTree")
        NewAcetreeButton.clicked.connect(self.LaunchNewAceTree)
        self.ArchiveButton = QtWidgets.QPushButton("Archive")
        self.ArchiveButton.clicked.connect(self.archiveEntry)
        unArchiveButton = QtWidgets.QPushButton("Retrieve")
        unArchiveButton.clicked.connect(self.RetrieveEntry)
        sideMenuLayout.addWidget(updateButton, 0, 0, 1, 2)
        sideMenuLayout.addWidget(addButton, 1, 0)
        sideMenuLayout.addWidget(rerunButton, 1, 1)
        sideMenuLayout.addWidget(self.ArchiveButton, 2, 0)
        sideMenuLayout.addWidget(unArchiveButton, 2, 1)
        sideMenuLayout.addWidget(AcetreeButton, 3, 0)
        sideMenuLayout.addWidget(treeButton, 3, 1)
        sideMenuLayout.addWidget(NewAcetreeButton, 4,0)
        sideMenuLayout.setAlignment(QtCore.Qt.AlignTop)
        self.Menu.setLayout(sideMenuLayout)

        self.filters = QtWidgets.QGroupBox("Add filter")
        layout = QtWidgets.QHBoxLayout()
        layout.addWidget(QtWidgets.QTextEdit())
        self.filters.setLayout(layout)

        self.sideGrid = QtWidgets.QGridLayout()
        self.sideGrid.addWidget(self.Menu, 0, 0)
        self.sideGrid.addWidget(self.filters, 1, 0)
        self.sideMenu = QtWidgets.QWidget()
        self.sideMenu.setMaximumWidth(300)
        self.sideMenu.setLayout(self.sideGrid)
        #self.sideMenuLayout.addWidget(self.label, 0, 0)

    def createEditMenu(self):
        self.commitButton = QtWidgets.QPushButton("Commit Edit")
        self.commitButton.clicked.connect(self.CommitEdit)
        cancelButton = QtWidgets.QPushButton("Cancel")
        cancelButton.clicked.connect(self.showSideMenu)
        self.editmenu = QtWidgets.QGroupBox()
        editMenuLayout = QtWidgets.QVBoxLayout()
        editMenuLayout.addWidget(self.commitButton)
        editMenuLayout.addWidget(cancelButton)
        self.editLabels = {}
        col = self.dbu.GetColumns()
        for c in range(len(col)):
            horizontalLayout = QtWidgets.QHBoxLayout()
            lineEdit = QtWidgets.QLineEdit()
            label = QtWidgets.QLabel(col[c][0]+":")
            horizontalLayout.addWidget(label)
            horizontalLayout.addWidget(lineEdit)
            tempW = QtWidgets.QWidget()
            tempW.setLayout(horizontalLayout)
            self.editLabels[col[c][0]] = lineEdit
            editMenuLayout.addWidget(tempW)

        self.editmenu.setLayout(editMenuLayout)
        self.editmenu.setMaximumWidth(300)
        self.editmenu.setHidden(True)

    def createActions(self):
        self.processAct = QtWidgets.QAction(QIcon(MainWindow.root + '/images/open.png'), "&Process", self,
                                  shortcut=QKeySequence.New, statusTip="Create new DB Entry",
                                  triggered=self.processEntry)

        self.addAct = QtWidgets.QAction(QIcon(MainWindow.root+'/images/new.png'), "&Add", self,
                              shortcut="Ctrl+P", statusTip="Manually add DB Entry",
                              triggered=self.addEntry)

        self.delAct = QtWidgets.QAction(QIcon(MainWindow.root + '/images/Delete.png'), "&Delete", self,
                              shortcut="Ctrl+x", statusTip="delet DB Entry: Ctrl+x",
                              triggered=self.delRow)

        self.editAct = QtWidgets.QAction(QIcon(MainWindow.root + '/images/edit.png'), "&Edit", self,
                               shortcut="Ctrl+E", statusTip="Edit Entry: Ctrl+E",
                               triggered=self.showEditMenu)

        self.exitAct = QtWidgets.QAction("E&xit", self, shortcut="Ctrl+Q",
                               statusTip="Exit the application", triggered=self.close)

        self.commit = QtWidgets.QAction("&Refresh", self, triggered=self.update)

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
                    self.editLabels[key].setText(str(values[i]))
            else:
                return value

    def CommitEdit(self):
        results = {}
        for key in self.editLabels:
            results[key] = self.editLabels[key].text()

        self.dbu.editTableEntry(results, self.getSelectedID)
        axis = results['comments']
        if axis == None or axis == "None":
            axis = "XXX"
        src = os.path.join(self.annots, 'dats', self.series+'AuxInfo.csv')
        try:
            FSConvert.editcsvaxis(src, axis)
        except:
            pass
        
        self.UpdateTree()
        self.showSideMenu()

    def UpdateTree(self):
        col = self.dbu.GetColumns()
        table = self.dbu.GetTable()
        self.tableWidget.setRowCount(len(table))
        self.tableWidget.setColumnCount(len(col))
        self.model = QtSql.QSqlTableModel(self)
        self.tableWidget.clear()
        for c in range(len(col)):
            self.tableWidget.setHorizontalHeaderItem(
                c, QtWidgets.QTableWidgetItem(col[c][0]))
            #self.tableWidget.headerItem().setText(c, col[c][0])

        # self.tableWidget.setSortingEnabled(False)
        for item in range(len(table)):
            # QTableWidgetItem(self.tableWidget)
            for value in range(len(table[item])):
                self.tableWidget.setItem(
                    item, value, QtWidgets.QTableWidgetItem(str(table[item][value])))

        #for i in range(len(self.dbu.GetColumns())):
        #    self.tableWidget.resizeColumnToContents(i)
        header = self.tableWidget.horizontalHeader()

        #for i in range(len(self.dbu.GetColumns())):
        #    header.setSectionResizeMode(i, QHeaderView.Stretch)
        #self.setFixedSize(self.layout.sizeHint())
        self.tableWidget.setSizeAdjustPolicy(QtWidgets.QAbstractScrollArea.AdjustToContents)
        self.tableWidget.resizeColumnsToContents()
        #self.tableWidget.setFixedSize(300,300)
        #self.tableWidget.resizeColumnToContents(i, QHeaderView.Stretch)

    def readSettings(self):
        settings = QtCore.QSettings("Trolltech", "Application Example")
        pos = settings.value("pos", QtCore.QPoint(200, 200))
        size = settings.value("size", QtCore.QSize(400, 400))
        self.resize(size)
        self.move(pos)
        self.setGeometry(0, 0, 2040, 1080)

    def addEntry(self):
        self.dbu.AddEntryToTable()
        self.UpdateTree()
        # self.SelectIdRow()
        self.showEditMenu()

    def processEntry(self):
        self.AddEntry = AddDialog()

        def endprocess(*args, **kwargs):
            self.dbu.AddXmlToTable(self.AddEntry.outfiles)
            self.UpdateTree()

        self.AddEntry.accepted.connect(endprocess)
        self.AddEntry.show()

    def rerunEntry(self):
        self.rerun = runMenu()
        self.rerun.show()

    @QtCore.pyqtSlot()
    def on_finished(self):
        self.UpdateDB()

    @QtCore.pyqtSlot(str)
    def on_errorOcurred(self, msg):
        QtWidgets.QMessageBox.critical(self, "Error Ocurred", msg)

    @QtCore.pyqtSlot()
    def archiveEntry(self):
        if self.isID:
            print('running archive....')
            self.progressbar.show()
            src = os.path.join(os.environ['targetDir'], self.series)
            dst = os.path.join(os.environ['archiveDir'],self.series)
            print(src, dst)
            self.startMoveFilesSignal.emit(src, dst)
            self.progressbar.hide()
 
    @QtCore.pyqtSlot()
    def RetrieveEntry(self):
        if self.isID:
            print('running retrieve.....')
            self.progressbar.show()
            src = os.path.join(os.environ['archiveDir'],self.series)
            dst = os.path.join(os.environ['targetDir'], self.series)
            self.startMoveFilesSignal.emit(src, dst)
            self.progressbar.hide()

    def LaunchAceTree(self):
        if self.isID:
            pathAceTree = os.path.join(
                os.environ['IW_LIB'], 'DB-Java/AceTree.jar')
            #pathAceTree = os.path.join(os.environ['IW_LIB'], 'AceTree/AceTree.jar')
            cmd = ('java -jar ' + pathAceTree + ' ' +
                   self.annots + '/dats/'+self.acetree + '&')
            os.system(cmd)
    
    def LaunchNewAceTree(self):
        if self.isID:
            #pathAceTree = os.path.join(
            #    os.environ['IW_LIB'], 'DB-Java/AceTree.jar')
            pathAceTree = os.path.join(os.environ['IW_LIB'], 'AceTree/AceTree.jar')
            cmd = ('java -jar ' + pathAceTree + ' ' +
                   self.annots + '/dats/'+self.acetree + '&')
            os.system(cmd)

    def UpdateDB(self):
        series_names = self.dbu.GetCol("series")

        local_dir = os.environ['targetDir']
        local_vids = os.listdir(local_dir)

        for v in local_vids:
            if v not in series_names:
                self.dbu.AddXmlToTable([os.path.join(local_dir, v)])

            id = self.dbu.GetSeriesID(v)
            row = self.dbu.GetRow(id)
            if int(row[16]) != 1:
                self.dbu.editTableEntry({'status': str(1)}, id)
            #temp fix for editing axis
            try:
                src = os.path.join(os.environ['targetDir'], row[1], 'dats', row[1]+'AuxInfo.csv')
                #src = os.path.join(row[9], 'dats', row[1]+'AuxInfo.csv')
                axis = FSConvert.getcsvaxis(src)
                self.dbu.editTableEntry({'comments': str(axis)}, id)
            except:
                pass
        archive_dir = os.environ['archiveDir']

        if not os.path.isdir(archive_dir):
            QtWidgets.QMessageBox.warning("please connect to NAS!")
            return

        archive_vids = os.listdir(archive_dir)
        for v in archive_vids:
            if v not in series_names:
                self.dbu.AddXmlToTable([os.path.join(archive_dir, v)])

            id = self.dbu.GetSeriesID(v)
            if int(self.dbu.GetRow(id)[16]) != 2:
                self.dbu.editTableEntry({'status': str(2)}, id)

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
    def status(self):
        return self.loadSelectedID(16)

    @property
    def series(self):
        return self.loadSelectedID(1)

class MoveFileHelper(QtCore.QObject):
    progressChanged = QtCore.pyqtSignal(int)
    started = QtCore.pyqtSignal()
    finished = QtCore.pyqtSignal()
    errorOccurred = QtCore.pyqtSignal(str)

    def calculateAndUpdate(self, done, total):
        progress = int(round((done / float(total)) * 100))
        self.progressChanged.emit(progress)

    @staticmethod
    def countFiles(directory):
        count = 0
        if os.path.isdir(directory):
            for path, dirs, filenames in os.walk(directory):
                count += len(filenames)
        return count

    @staticmethod
    def makedirs(dest):
        if not os.path.exists(dest):
            os.makedirs(dest)

    @QtCore.pyqtSlot(str, str)
    def moveFilesWithProgress(self, src, dest):
        numFiles = MoveFileHelper.countFiles(src)
        if os.path.exists(dest):
            self.errorOccurred.emit("Dest exist")
            return 
        if numFiles > 0:
            self.started.emit()
            MoveFileHelper.makedirs(dest)
            numCopied = 0
            for path, dirs, filenames in os.walk(src):
                for directory in dirs:
                    destDir = path.replace(src, dest)
                    MoveFileHelper.makedirs(os.path.join(destDir, directory))

                for sfile in filenames:
                    srcFile = os.path.join(path, sfile)
                    destFile = os.path.join(path.replace(src, dest), sfile)
                    shutil.move(srcFile, destFile)
                    numCopied += 1
                    self.calculateAndUpdate(numCopied, numFiles)
            if os.path.exists(src):
                shutil.rmtree(src)
            self.finished.emit()           


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())
