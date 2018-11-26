from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream)
from PyQt5.QtGui import QIcon, QKeySequence
from PyQt5.QtWidgets import (QAction, QApplication, QFileDialog, QMainWindow,
        QMessageBox, QTextEdit, QVBoxLayout, QGroupBox, QHBoxLayout, QPushButton,
        QLineEdit, QDialog, QWidget, QTableWidget, QLabel, QPushButton,
        QTreeWidget, QTreeWidgetItem)
from PyQt5.QtSql import QSqlTableModel

import DB_Manager, sys

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'

        self.createND2FileGroupBox()

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
        tableName = "student1"
        self.dbu = DB_Manager.DatabaseUtility(database, tableName)
        self.verticalLayout_2 = QVBoxLayout()
        self.verticalLayout = QVBoxLayout()
        self.label = QLabel("Label")
        self.label.setAlignment(Qt.AlignCenter)
        self.horizontalLayout = QHBoxLayout()
        self.lineEdit = QLineEdit()
        self.horizontalLayout.addWidget(self.lineEdit)
        self.commitButton = QPushButton("Commit")
        self.commitButton.clicked.connect(self.Commit)
        self.horizontalLayout.addWidget(self.commitButton)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.treeWidget = QTreeWidget()
        self.verticalLayout.addWidget(self.treeWidget)
        self.verticalLayout_2.addLayout(self.verticalLayout)
        self.dataTable.setLayout(self.verticalLayout_2)

        self.UpdateTree()

    
    def Commit(self):
        text = self.lineEdit.text()
        self.dbu.AddEntryToTable(text)
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
        mainLayout.addWidget(self.dataTable)#self.ND2FileBox)
        
        self.centerWiget = QWidget(self)
        self.centerWiget.setLayout(mainLayout)


    def createND2FileGroupBox(self):
        self.ND2FileBox = QGroupBox("ND2 File Location")
        layout = QHBoxLayout()
        bntFile = QPushButton('&Find File')
        bntFile.clicked.connect(self.updateND2)
        layout.addWidget(bntFile)
        self.ND2line = QLineEdit(self.nd2_file)
        layout.addWidget(self.ND2line)
        self.ND2FileBox.setLayout(layout)

    def createActions(self):
        root = QFileInfo(__file__).absolutePath()

        self.addAct = QAction(QIcon(root + '/images/new.png'), "&Add", self,
                shortcut=QKeySequence.New, statusTip="Create new DB entry")#,
                #triggered=self.addEntry)
        
        self.exitAct = QAction("E&xit", self, shortcut="Ctrl+Q",
                statusTip="Exit the application", triggered=self.close)

        self.commit = QAction("&Refresh", self, triggered=self.update)
            #text = self.lineEdit.text()
            #self.dbu.AddEntryToTable(text)
            #self.UpdateTree())

    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu("&File")
        self.fileMenu.addAction(self.addAct)
        self.fileMenu.addAction(self.exitAct)

    def createToolBars(self):
        self.fileToolBar = self.addToolBar("File")
        self.fileToolBar.addAction(self.addAct)

    def updateND2(self):
        self.nd2_file = str(QFileDialog.getExistingDirectory(self, 'Select nd2 directory', '/home'))
        self.ND2line.setText(self.nd2_file)
    
    def createStatusBar(self):
        self.statusBar().showMessage("Ready")
    
    def readSettings(self):
        settings = QSettings("Trolltech", "Application Example")
        pos = settings.value("pos", QPoint(200, 200))
        size = settings.value("size", QSize(400, 400))
        self.resize(size)
        self.move(pos)

    def maybeSave(self):
        if self.textEdit.document().isModified():
            ret = QMessageBox.warning(self, "Application",
                    "The document has been modified.\nDo you want to save "
                    "your changes?",
                    QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel)

            if ret == QMessageBox.Save:
                return False#self.save()

            if ret == QMessageBox.Cancel:
                return False

        return True

if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())


    