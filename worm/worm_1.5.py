from PyQt5 import QtCore, QtGui, QtWidgets, QtSql

class Ui_Form(object):

    def __init__(self):
        object.__init__(self)
        self.db = QtSql.QSqlDatabase.addDatabase('QMYSQL')
        self.db.setHostName("localhost")
        self.db.setDatabaseName("test2")
        self.db.setUserName("root")
        self.db.setPassword("Qq1!Ww2@")

    def loadData(self):
        status = self.db.open()
        if status == False:
            QtWidgets.QMessageBox.warning(self, "Error", self.db.lastError().text(), QtWidgets.QMessageBox.Discard)
        else:
            self.tableWidget.setColumnCount(3)
            self.tableWidget.setHorizontalHeaderLabels(['id', 'name', 'surname'])
            row = 0
            sql = "SELECT * FROM student"
            query = QtSql.QSqlQuery(sql)
            while query.next():
                self.tableWidget.insertRow(row)
                id = QtWidgets.QTableWidgetItem(str(query.value(0)))
                nome = QtWidgets.QTableWidgetItem(str(query.value(1)))
                surname = QtWidgets.QTableWidgetItem(str(query.value(2)))
                self.tableWidget.setItem(row, 0, id)
                self.tableWidget.setItem(row, 1, nome)
                self.tableWidget.setItem(row, 2, surname)
                row = row + 1
        #self.db.close()

    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(382, 316)
        self.tableWidget = QtWidgets.QTableWidget(Form)
        self.tableWidget.setGeometry(QtCore.QRect(20, 50, 319, 201))
        self.tableWidget.setAlternatingRowColors(True)
        self.tableWidget.setRowCount(16)
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(3)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, item)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, item)
        item = QtWidgets.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(2, item)
        self.tableWidget.horizontalHeader().setVisible(True)
        self.tableWidget.verticalHeader().setVisible(False)
        self.pushButton = QtWidgets.QPushButton(Form)
        self.pushButton.setGeometry(QtCore.QRect(140, 270, 75, 23))
        self.pushButton.setObjectName("pushButton")
        self.pushButton.clicked.connect(self.loadData)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        item = self.tableWidget.horizontalHeaderItem(0)
        item.setText(_translate("Form", "Code"))
        item = self.tableWidget.horizontalHeaderItem(1)
        item.setText(_translate("Form", "Name"))
        item = self.tableWidget.horizontalHeaderItem(2)
        item.setText(_translate("Form", "Surname"))
        self.pushButton.setText(_translate("Form", "Load"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Form = QtWidgets.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())