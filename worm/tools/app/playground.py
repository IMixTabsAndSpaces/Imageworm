
"""
ZetCode PyQt5 tutorial 

In this example, we receive data from
a QInputDialog dialog. 

Aauthor: Jan Bodnar
Website: zetcode.com 
Last edited: August 2017
"""

from PyQt5.QtWidgets import (QWidget, QFileDialog, QPushButton, QLineEdit, 
    QInputDialog, QApplication, QGridLayout)

from PyQt5.QtCore import QDate, Qt
from PyQt5.QtGui import (QFont, QTextCharFormat, QTextCursor, QTextFrameFormat,
        QTextLength, QTextTableFormat)
from PyQt5.QtWidgets import (QApplication, QCheckBox, QDialog,
        QDialogButtonBox, QGridLayout, QLabel, QLineEdit, QMainWindow,
        QMessageBox, QMenu, QTableWidget, QTableWidgetItem, QTabWidget,
        QTextEdit)
from PyQt5.QtPrintSupport import QAbstractPrintDialog, QPrintDialog, QPrinter

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        fileMenu = QMenu("&File", self)
        newAction = fileMenu.addAction("&New...")
        newAction.setShortcut("Ctrl+N")
        self.printAction = fileMenu.addAction("&Print...", self.printFile)
        self.printAction.setShortcut("Ctrl+P")
        self.printAction.setEnabled(False)
        quitAction = fileMenu.addAction("E&xit")
        quitAction.setShortcut("Ctrl+Q")
        self.menuBar().addMenu(fileMenu)
        self.nd2_file= '/media/zachlab/Windows/LinuxStorage/images/ND2_Files'

        self.initUI()
        
        
    def initUI(self): 
        grid = QGridLayout()
        grid.setSpacing(10)

        self.bntRun = QPushButton('RUN', self)
        grid.addWidget(self.bntRun,2,0)
#####################################################################################################
#                                   setting ND2 File
#####################################################################################################
        self.bntFile = QPushButton('ND2 File', self)
        grid.addWidget(self.bntFile, 1,0)
        self.bntFile.clicked.connect(self.showDialog)


        
        self.le = QLineEdit(self)
        #self.le.move(130, 22)
        grid.addWidget(self.le, 1,1)
        self.le.setText(self.nd2_file)
        self.setLayout(grid)
        self.setGeometry(300, 300, 290, 150)
        self.setWindowTitle('Input dialog')
        self.show()
        
        
    def showDialog(self):

        self.nd2_file = str(QFileDialog.getExistingDirectory(self, 'Select nd2 directory', '/home'))
        self.le.setText(self.nd2_file)
        
if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    window = MainWindow()
    window.resize(640, 480)
    window.show()
    window.createSample()
    sys.exit(app.exec_())
