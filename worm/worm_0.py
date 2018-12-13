#!/usr/bin/env python
import os
import sys
import shutil
from PyQt5.QtWidgets import (QApplication, QDialog, QLineEdit, QMainWindow,
                            QPushButton, QProgressBar)
import PyQt5.QtGui as QtGui

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.button = QPushButton("Copy", self)
        self.button.clicked.connect(self.archiveEntry)
        self.show()

    def archiveEntry(self):
        self.p = ProgressBar()
        self.p.show()
        Dir = "/media/zachlab/Windows/LinuxStorage/old/embryos"
        outDir = "/media/zachlab/Windows/LinuxStorage/old/out"
        run = self.p.moveFilesWithProgress(Dir, outDir)
        # if run:
        #    self.p.close


class ProgressBar(QDialog):
    def __init__(self):
        super().__init__()
        self.pbar = QProgressBar(self)

    def timerEvent(self, e):
        if self.step >= 100:
            self.timer.stop()
            self.btn.setText('Finished')
            return
        self.pbar.setValue(self.step)

    def update(self, value):
        self.pbar.setValue(value)
        QtGui.QGuiApplication.processEvents()

    def calculateAndUpdate(self, done, total):
        progress = int(round((done / float(total)) * 100))
        self.update(progress)
        

    def countFiles(self, directory):
        files = []
        if os.path.isdir(directory):
            for path, dirs, filenames in os.walk(directory):
                files.extend(filenames)
        return len(files)

    def makedirs(self, dest):
        if not os.path.exists(dest):
            os.makedirs(dest)

    def moveFilesWithProgress(self, src, dest):
        numFiles = self.countFiles(src)
        if os.path.exists(dest):
            return 0
        if numFiles > 0:
            self.makedirs(dest)
            numCopied = 0

            for path, dirs, filenames in os.walk(src):
                for directory in dirs:
                    destDir = path.replace(src, dest)
                    self.makedirs(os.path.join(destDir, directory))

                for sfile in filenames:
                    srcFile = os.path.join(path, sfile)
                    destFile = os.path.join(path.replace(src, dest), sfile)
                    shutil.copy(srcFile, destFile)
                    numCopied += 1
                    self.calculateAndUpdate(numCopied, numFiles)
                    self.show()
            return 1
        else:
            return 0


if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = MainWindow()
    sys.exit(app.exec_())
