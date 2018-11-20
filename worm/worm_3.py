from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal)
from PyQt5.QtGui import QIcon, QKeySequence
import PyQt5.QtGui
from PyQt5.QtWidgets import (QAction, QApplication, QFileDialog, QMainWindow,
        QMessageBox, QTextEdit, QVBoxLayout, QGroupBox, QHBoxLayout, QPushButton,
        QLineEdit, QDialog, QWidget, QTableWidget, QLabel, QPushButton, QComboBox,
        QTreeWidget, QTreeWidgetItem, QDialogButtonBox, QFormLayout, QMenu, QMenuBar,
        QGridLayout, QSpinBox, QAbstractScrollArea)
import DB_Manager, sys, os
class EmittingStream(QObject):

    textWritten = pyqtSignal(str)

    def write(self, text):
        self.textWritten.emit(str(text))



class AddDialog(QDialog):
 
    def __init__(self):
        super(AddDialog, self).__init__()
        sys.stdout = EmittingStream(textWritten=self.normalOutputWritten)

        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'
        #creat layout widgets
        self.createND2FileGroupBox()
        self.bigEditor = QTextEdit("Process Info:")
        self.bigEditor.setReadOnly(True)

        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.runpipline)
        buttonBox.rejected.connect(self.reject)

        #creat layout with built widgets
        mainLayout = QVBoxLayout()
        mainLayout.addWidget(self.ND2FileBox)
        mainLayout.addWidget(self.bigEditor)
        mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)
        self.setWindowTitle("ADD ENTRY FROM ND2 FILE")
    
    def __del__(self):
        # Restore sys.stdout
        sys.stdout = sys.__stdout__

    def normalOutputWritten(self, text):
        """Append text to the QTextEdit."""
        # Maybe QTextEdit.append() works as well, but this is how I do it:
        self.bigEditor.append(text)

    def createND2FileGroupBox(self):
        self.ND2FileBox = QGroupBox("ND2 File Location")
        layout = QHBoxLayout()

        bntFile = QPushButton('Find File')
        bntFile.clicked.connect(self.updateND2)
        layout.addWidget(bntFile)
        self.ND2line = QLineEdit(self.nd2_file)
        layout.addWidget(self.ND2line)
        self.ND2FileBox.setLayout(layout)

    def createFormGroupBox(self):
        self.formGroupBox = QGroupBox("Form layout")
        layout = QFormLayout()
        layout.addRow(QLabel("Line 1:"), QLineEdit())
        layout.addRow(QLabel("Line 2, long text:"), QComboBox())
        layout.addRow(QLabel("Line 3:"), QSpinBox())
        self.formGroupBox.setLayout(layout)
    
    def updateND2(self):
        self.nd2_file = str(QFileDialog.getExistingDirectory(self, 'Select nd2 directory', '/home'))
        self.ND2line.setText(self.nd2_file)

    def runpipline(self):
        #print("here")
        os.system("python ./nd2pipline.py")


class MainWindow(QMainWindow):
    root = QFileInfo(__file__).absolutePath()

    def __init__(self):
        super(MainWindow, self).__init__()
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
    
        #self.verticalLayout_2 = QVBoxLayout()
        #self.verticalLayout = QVBoxLayout()
        self.label = QLabel("Label")
        self.label.setAlignment(Qt.AlignCenter)
        #self.horizontalLayout = QHBoxLayout()
        #self.lineEdit = QLineEdit()
        #self.horizontalLayout.addWidget(self.lineEdit)


        self.treeWidget = QTreeWidget()
        self.commitButton = QPushButton("Commit")
        self.commitButton.clicked.connect(self.Commit)

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
    
    def Commit(self):
        results = {}
        for key in self.editLabels:
            results[key] = self.editLabels[key].text()
        self.dbu.AddEntryToTable(results['Series'],results['Timepts'])
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
        
        self.exitAct = QAction("E&xit", self, shortcut="Ctrl+Q",
                statusTip="Exit the application", triggered=self.close)

        self.commit = QAction("&Refresh", self, triggered=self.update)

    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu("&File")
        self.fileMenu.addAction(self.processAct)
        self.fileMenu.addAction(self.addAct)
        self.fileMenu.addAction(self.exitAct)

    def createToolBars(self):
        self.fileToolBar = self.addToolBar("File")
        self.fileToolBar.addAction(self.processAct)
        self.fileToolBar.addAction(self.addAct)
   
    def createStatusBar(self):
        self.statusBar().showMessage("Ready")
    
    def readSettings(self):
        settings = QSettings("Trolltech", "Application Example")
        pos = settings.value("pos", QPoint(200, 200))
        size = settings.value("size", QSize(400, 400))
        self.resize(size)
        self.move(pos)
        self.setGeometry(0, 0, 2040, 1080)

    def addEntry(self):
        pass

    def processEntry(self):
        self.AddEntry = AddDialog()
        self.AddEntry.show()
if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())