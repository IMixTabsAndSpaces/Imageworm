from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal)
from PyQt5.QtGui import QIcon, QKeySequence
import PyQt5.QtGui
from PyQt5.QtWidgets import (QAction, QApplication, QFileDialog, QMainWindow,
        QMessageBox, QTextEdit, QVBoxLayout, QGroupBox, QHBoxLayout, QPushButton,
        QLineEdit, QDialog, QWidget, QTableWidget, QLabel, QPushButton, QComboBox,
        QTreeWidget, QTreeWidgetItem, QDialogButtonBox, QFormLayout, QMenu, QMenuBar,
        QGridLayout, QSpinBox, QAbstractScrollArea)
import sys, os

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
