from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal)
from PyQt5.QtGui import QIcon, QKeySequence
import PyQt5.QtGui
from PyQt5 import QtWidgets
import sys, os
try:
    import helpers.Ianalysis as Ianalysis
    from helpers.TextEditor_Code import Editor
except:
    import Ianalysis
    from TextEditor_Code import Editor

class EmittingStream(QObject):

    textWritten = pyqtSignal(str)

    def write(self, text):
        self.textWritten.emit(str(text))



class AddDialog(QtWidgets.QDialog):
 
    def __init__(self):
        super(AddDialog, self).__init__()
        #self.setWindowModality(QMainWindow)
        sys.stdout = EmittingStream(textWritten=self.normalOutputWritten)
        self.ParameterFile= os.environ['mlParamLoc']
        self.nd2_file= os.environ['nd2File']
        self.out_file= os.environ['targetDir']
        #creat layout widgets
        self.createToolsGroupBox()
        self.createND2FileGroupBox()
        self.createOutputGroupBox()
        self.bigEditor = QtWidgets.QTextEdit("Process Info:")
        self.bigEditor.setReadOnly(True)

        buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.runpipline)
        buttonBox.rejected.connect(self.reject)

        #creat layout with built widgets
        mainLayout = QtWidgets.QVBoxLayout()
        mainLayout.addWidget(self.ToolsBox)
        mainLayout.addWidget(self.ND2FileBox)
        mainLayout.addWidget(self.OutputBox)
        mainLayout.addWidget(self.bigEditor)
        mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)
        self.setWindowTitle("ADD ENTRY FROM ND2 FILE")
        self.setMinimumWidth(600)
    
    def __del__(self):
        # Restore sys.stdout
        sys.stdout = sys.__stdout__

    def normalOutputWritten(self, text):
        """Append text to the QTextEdit."""
        # Maybe QTextEdit.append() works as well, but this is how I do it:
        self.bigEditor.append(text)

    def createToolsGroupBox(self):
        self.ToolsBox = QtWidgets.QGroupBox()
        layout = QtWidgets.QHBoxLayout()

        bntFile = QtWidgets.QPushButton('Edit Parameters')
        bntFile.clicked.connect(self.updateParameters)
        layout.addWidget(bntFile)
        self.ToolsBox.setLayout(layout)

    def createND2FileGroupBox(self):
        self.ND2FileBox = QtWidgets.QGroupBox("ND2 File Location:")
        layout = QtWidgets.QHBoxLayout()

        bntFile = QtWidgets.QPushButton('Find File')
        bntFile.clicked.connect(self.updateND2)
        layout.addWidget(bntFile)
        self.ND2line = QtWidgets.QLineEdit(self.nd2_file)
        layout.addWidget(self.ND2line)
        self.ND2FileBox.setLayout(layout)
    
    def createOutputGroupBox(self):
        self.OutputBox = QtWidgets.QGroupBox("Output File Location:")
        layout = QtWidgets.QHBoxLayout()

        bntFile = QtWidgets.QPushButton('Find File')
        bntFile.clicked.connect(self.updateOut)
        layout.addWidget(bntFile)
        self.outline = QtWidgets.QLineEdit(self.out_file)
        layout.addWidget(self.outline)
        self.OutputBox.setLayout(layout)

    def createFormGroupBox(self):
        self.formGroupBox = QtWidgets.QGroupBox("Form layout")
        layout = QtWidgets.QFormLayout()
        layout.addRow(QtWidgets.QLabel("Line 1:"), QtWidgets.QLineEdit())
        layout.addRow(QtWidgets.QLabel("Line 2, long text:"), QtWidgets.QComboBox())
        layout.addRow(QtWidgets.QLabel("Line 3:"), QtWidgets.QSpinBox())
        self.formGroupBox.setLayout(layout)
    
    def updateParameters(self):
        self.editor = Editor(self.ParameterFile)
        self.editor.show()

    def updateND2(self):
        self.nd2_file = str(QtWidgets.QFileDialog.getExistingDirectory(self, 'Select nd2 directory', '/home'))
        self.ND2line.setText(self.nd2_file)

    def updateOut(self):
        self.out_file = str(QtWidgets.QFileDialog.getExistingDirectory(self, 'Select Output directory', '/home'))
        self.ND2line.setText(self.out_file)

    def runpipline(self):
        self.bigEditor = QtWidgets.QTextEdit('''Processing Please wait...
            This will take a while.''')
        self.outfiles = Ianalysis.Main(nd2=self.nd2_file, outDir=self.out_file)
        print(self.outfiles)

class ProgressBar(QtWidgets.QDialog):
    def __init__(self):
        super().__init__()

        self.pbar = QtWidgets.QProgressBar()
        self.pbar.setAlignment(Qt.AlignCenter)
        self.pbar.setGeometry(130, 40, 200, 25)
        self.lable = QtWidgets.QLabel("MOVING TIFF IMAGES....")
        self.lable.setGeometry(80, 40, 200, 20)
        self.lable.setAlignment(Qt.AlignCenter)
        #self.addWidget(self.lable)
        grid = QtWidgets.QGridLayout()
        grid.addWidget(self.pbar, 0,0)
        grid.addWidget(self.lable, 1, 0)
        self.setLayout(grid)
        self.setFixedSize(300, 150)
        self.setWindowTitle('MOVING FILES')

    def update(self, value):
        self.pbar.setValue(value)

    def calculateAndUpdate(self, done, total):
        progress = int(round((done / float(total)) * 100))
        self.update(progress)


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    mainWin = AddDialog()
    mainWin.show()
    sys.exit(app.exec_())
