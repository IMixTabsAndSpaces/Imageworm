#############################################################################
"""
Author: Stephen Nehrbass
Built with PyQt5
a simple conda install command is all that is needed for dependicies.
"""
#############################################################################


from PyQt5.QtWidgets import (QApplication, QComboBox, QDialog, QFileDialog,
        QDialogButtonBox, QFormLayout, QGridLayout, QGroupBox, QHBoxLayout,
        QLabel, QLineEdit, QMenu, QMenuBar, QPushButton, QSpinBox, QTextEdit,
        QVBoxLayout)
from PyQt5.QtCore import QObject, pyqtSignal

import logging
import sys
import os
'''
logger = logging.getLogger(__name__)

class XStream(QObject):
    _stdout = None
    _stderr = None

    messageWritten = pyqtSignal(str)

    def flush( self ):
        pass

    def fileno( self ):
        return -1

    def write( self, msg ):
        if ( not self.signalsBlocked() ):
            self.messageWritten.emit(unicode(msg))

    @staticmethod
    def stdout():
        if ( not XStream._stdout ):
            XStream._stdout = XStream()
            sys.stdout = XStream._stdout
        return XStream._stdout

    @staticmethod
    def stderr():
        if ( not XStream._stderr ):
            XStream._stderr = XStream()
            sys.stderr = XStream._stderr
        return XStream._stderr
'''
class EmittingStream(QObject):

    textWritten = pyqtSignal(str)

    def write(self, text):
        self.textWritten.emit(str(text))



class Dialog(QDialog):
    NumGridRows = 3
    NumButtons = 4
    

    def __init__(self):
        super(Dialog, self).__init__()
        sys.stdout = EmittingStream(textWritten=self.normalOutputWritten)



        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'
        #creat layout widgets
        self.createMenu()
        self.createND2FileGroupBox()
        #self.createGridGroupBox()
        #self.createFormGroupBox()
        self.bigEditor = QTextEdit("Process Info:")
        self.bigEditor.setReadOnly(True)

        #bla = bigEditor.setText(SIG)
        #bla.connect(self.bigEditor, SIGNAL("returnPressed(void)"), self.updateND2)

        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.runpipline)
        buttonBox.rejected.connect(self.reject)

        #creat layout with built widgets
        mainLayout = QVBoxLayout()
        mainLayout.setMenuBar(self.menuBar)
        mainLayout.addWidget(self.ND2FileBox)
        #mainLayout.addWidget(self.gridGroupBox)
        #mainLayout.addWidget(self.formGroupBox)
        mainLayout.addWidget(self.bigEditor)
        mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)
        self.setWindowTitle("Basic Layouts")
    
    def __del__(self):
        # Restore sys.stdout
        sys.stdout = sys.__stdout__

    def normalOutputWritten(self, text):
        """Append text to the QTextEdit."""
        # Maybe QTextEdit.append() works as well, but this is how I do it:
        self.bigEditor.append(text)

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

        bntFile = QPushButton('Find File')
        bntFile.clicked.connect(self.updateND2)
        layout.addWidget(bntFile)
        self.ND2line = QLineEdit(self.nd2_file)
        layout.addWidget(self.ND2line)

        self.ND2FileBox.setLayout(layout)

    def createGridGroupBox(self):
        self.gridGroupBox = QGroupBox("Grid layout")
        layout = QGridLayout()

        for i in range(Dialog.NumGridRows):
            label = QLabel("Line %d:" % (i + 1))
            lineEdit = QLineEdit()
            layout.addWidget(label, i + 1, 0)
            layout.addWidget(lineEdit, i + 1, 1)

        self.smallEditor = QTextEdit()
        self.smallEditor.setPlainText("This widget takes up about two thirds "
                "of the grid layout.")

        layout.addWidget(self.smallEditor, 0, 2, 4, 1)

        layout.setColumnStretch(1, 10)
        layout.setColumnStretch(2, 20)
        self.gridGroupBox.setLayout(layout)

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


def Main():

    app = QApplication(sys.argv)
    dialog = Dialog()
    sys.exit(dialog.exec_())

if __name__ == '__main__':
    Main()