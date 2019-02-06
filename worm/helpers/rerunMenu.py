from PyQt5.QtCore import (QFile, QFileInfo, QPoint, QRect, QSettings, QSize,
        Qt, QTextStream, QObject, pyqtSignal)
from PyQt5.QtGui import QIcon, QKeySequence, QStandardItemModel
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


SERIES, SENDER, DATE = range(3)
class runMenu(QtWidgets.QDialog):
 
    def __init__(self):
        super(runMenu, self).__init__()
        #self.setWindowModality(QMainWindow)
        #sys.stdout = EmittingStream(textWritten=self.normalOutputWritten)
        self.ParameterFile= os.environ['mlParamLoc']
        self.series_file= []
        self.out_file= os.environ['targetDir']
        #creat layout widgets
        self.bigEditor = QtWidgets.QTextEdit("Process Info:")
        self.bigEditor.setReadOnly(True)
        self.createToolsGroupBox()
        self.createParamGroupBox()
        self.row = 0


        buttonBox = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.runpipline)
        buttonBox.rejected.connect(self.reject)

        #creat layout with built widgets
        mainLayout = QtWidgets.QVBoxLayout()
        mainLayout.addWidget(self.ToolsBox)
        mainLayout.addWidget(self.ParamBox)
        mainLayout.addWidget(self.bigEditor)
        mainLayout.addWidget(buttonBox)
        self.setLayout(mainLayout)
        self.setWindowTitle("RUN ANALYSIS")
    
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

    def createParamGroupBox(self):
        self.ParamBox = QtWidgets.QGroupBox("Options")
        layout = QtWidgets.QGridLayout()

        bntFile = QtWidgets.QPushButton('Find File')
        bntFile.clicked.connect(self.updateSeries)
        layout.addWidget(bntFile, 0, 1, 1, 3)
        self.listView = QtWidgets.QTreeView()
        self.listView.setRootIsDecorated(False)
        self.listView.setAlternatingRowColors(True)
        self.model = QStandardItemModel(0, 1)
        self.model.setHeaderData(SERIES, Qt.Horizontal, "Series")
        self.listView.setModel(self.model)
        layout.addWidget(self.listView, 1, 1, 6, 3)
        self.MATLAB = QtWidgets.QCheckBox("MATLAB")
        self.MATLAB.setChecked(True)
        self.MakeDB = QtWidgets.QCheckBox("MakeDB")
        self.MakeDB.setChecked(True)
        self.RedExtract = QtWidgets.QCheckBox("RedExtract")
        self.RedExtract.setChecked(True)
        self.Measure = QtWidgets.QCheckBox("Measure")
        self.Measure.setChecked(True)
        self.RedExcel1 = QtWidgets.QCheckBox("RedExcel1")
        self.RedExcel2 = QtWidgets.QCheckBox("RedExcel2")
        box = QtWidgets.QGroupBox()
        Horizontal = QtWidgets.QHBoxLayout()
        self.Align = QtWidgets.QCheckBox("Align")
        self.AlignED = QtWidgets.QLineEdit()
        self.AlignED.setMaximumWidth(30)
        Horizontal.addWidget(self.Align)
        Horizontal.addWidget(self.AlignED)
        box.setLayout(Horizontal)
        layout.addWidget(self.MATLAB, 0, 0)
        layout.addWidget(self.MakeDB, 1, 0)
        layout.addWidget(self.RedExtract, 2, 0)
        layout.addWidget(self.Measure, 3, 0)
        layout.addWidget(self.RedExcel1, 4, 0)
        layout.addWidget(self.RedExcel2, 5, 0)
        layout.addWidget(box, 6, 0)
        self.ParamBox.setLayout(layout)
    
    def updateParameters(self):
        self.editor = Editor(self.ParameterFile)
        self.editor.show()

    def updateSeries(self):
        f = str(QtWidgets.QFileDialog.getExistingDirectory(self, 'Select Series', self.out_file))
        if f not in self.series_file:
            self.series_file.append(f)
            self.model.insertRow(self.row)
            self.model.setData(self.model.index(0, SERIES), os.path.basename(f))
            self.row += 1
        #self.seriesline.setText(self.series_file)

    def runpipline(self):
        if self.series_file:
            self.bigEditor = QtWidgets.QTextEdit('''Processing Please wait...
                This will take a while.''')
            Ianalysis.Main(fileName=self.series_file, MATLAB=self.MATLAB.isChecked(), 
                MakeDB=self.MakeDB.isChecked(), RedExtract=self.RedExtract.isChecked(), 
                Measure=self.Measure.isChecked(), RedExcel1=self.RedExcel1.isChecked(), 
                RedExcel2=self.RedExcel2.isChecked(), Align=self.Align.isChecked(), AlignN=self.AlignED.text())

if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    mainWin = runMenu()
    mainWin.show()
    sys.exit(app.exec_())
