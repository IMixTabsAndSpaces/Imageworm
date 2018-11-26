import sys
from PyQt5 import QtCore, QtGui

class Window(QtGui.QWidget):
    def __init__(self):
        super(Window, self).__init__()
        self.button = QtGui.QPushButton('Select')
        self.button.clicked.connect(self.handleButton)
        self.tree = QtGui.QTreeView()
        layout = QtGui.QVBoxLayout(self)
        layout.addWidget(self.tree)
        layout.addWidget(self.button)
        columns = 'One Two Three Four'.split()
        mod = QtGui.QStandardItemModel(self)
        mod.setHorizontalHeaderLabels(columns)
        for row in range(1000):
            mod.appendRow((
                QtGui.QStandardItem('A%s' % row),
                QtGui.QStandardItem('B%s' % row),
                QtGui.QStandardItem('C%s' % row),
                QtGui.QStandardItem('D%s' % row),
                ))
        self.tree.setModel(mod)
        self.tree.setSelectionMode(
            QtGui.QAbstractItemView.ExtendedSelection)

    def handleButton(self):
        mod = self.tree.model()
        columns = mod.columnCount() - 1
        flags = QtGui.QItemSelectionModel.Select
        selection = QtGui.QItemSelection()
        for start, end in ((2, 15), (25, 260), (500, 996)):
            start, end = mod.index(start, 0), mod.index(end, columns)
            if selection.indexes():
                selection.merge(QtGui.QItemSelection(start, end), flags)
            else:
                selection.select(start, end)
        self.tree.selectionModel().clear()
        self.tree.selectionModel().select(selection, flags)

if __name__ == '__main__':

    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.setGeometry(700, 50, 500, 600)
    window.show()
    sys.exit(app.exec_())