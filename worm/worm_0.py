from PyQt5.QtWidgets import (QApplication, QComboBox, QDialog, QFileDialog,
        QDialogButtonBox, QFormLayout, QGridLayout, QGroupBox, QHBoxLayout,
        QLabel, QLineEdit, QMenu, QMenuBar, QPushButton, QSpinBox, QTextEdit,
        QVBoxLayout, QMainWindow)
from PyQt5.QtCore import QObject, pyqtSignal, QFileInfo
import sys
import os

class Dialog(QDialog):

    def __init__(self):
        super(Dialog, self).__init__()
        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'

        #initshalize
        self.createActions()
        self.createMenu()
        self.createToolBars()
        self.createND2FileGroupBox()
        

        #create main layout
        mainLayout = QVBoxLayout()
        mainLayout.setMenuBar(self.menuBar)
        #add widgets
        mainLayout.addWidget(self.ND2FileBox)
        #create
        self.setLayout(mainLayout)
        self.setWindowTitle("IMAGE WORM")
        
    
    def createMenu(self):
        self.menuBar = QMenuBar()
        self.fileMenu = QMenu("&File", self)
        self.exitAction = self.fileMenu.addAction("E&xit")
        self.exitAction.setShortcut("Ctrl+Q")
        self.menuBar.addMenu(self.fileMenu)
        self.exitAction.triggered.connect(self.accept)
    
    def createND2FileGroupBox(self):
        self.ND2FileBox = QGroupBox("ND2 File Location")
        layout = QHBoxLayout()
        bntFile = QPushButton('&Find File')
        bntFile.clicked.connect(self.updateND2)
        layout.addWidget(bntFile)
        self.ND2line = QLineEdit(self.nd2_file)
        layout.addWidget(self.ND2line)
        self.ND2FileBox.setLayout(layout)

    def updateND2(self):
        self.nd2_file = str(QFileDialog.getExistingDirectory(self, 'Select nd2 directory', '/home'))
        self.ND2line.setText(self.nd2_file)
    
    def createActions(self):
        root = QFileInfo(__file__).absolutePath()

        self.newAct = self.QAction(QIcon(root + '/images/new.png'), "&New", self,
                shortcut=QKeySequence.New, statusTip="Create a new file",
                triggered=self.newFile)

    def createToolBars(self):
        self.fileToolBar = self.addToolBar("File")
        self.fileToolBar.addAction(self.newAct)


def Main():
    app = QApplication(sys.argv)
    dialog = Dialog()
    sys.exit(dialog.exec_())

if __name__ == '__main__':
    Main()
