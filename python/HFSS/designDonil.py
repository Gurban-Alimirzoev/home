# Form implementation generated from reading ui file 'designDonil.ui'
#
# Created by: PyQt6 UI code generator 6.2.3
#
# WARNING: Any manual changes made to this file will be lost when pyuic6 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt6 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(399, 580)
        font = QtGui.QFont()
        font.setPointSize(14)
        MainWindow.setFont(font)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("C:/Users/skorp/Desktop/400-4009677_the-bullet-from-enter-the-gungeon-red-caped.ico"), QtGui.QIcon.Mode.Normal, QtGui.QIcon.State.Off)
        MainWindow.setWindowIcon(icon)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.listWidget = QtWidgets.QListWidget(self.centralwidget)
        self.listWidget.setObjectName("listWidget")
        self.verticalLayout.addWidget(self.listWidget)
        self.btnBrowse = QtWidgets.QPushButton(self.centralwidget)
        self.btnBrowse.setObjectName("btnBrowse")
        self.verticalLayout.addWidget(self.btnBrowse)
        self.runCalc = QtWidgets.QPushButton(self.centralwidget)
        self.runCalc.setCheckable(True)
        self.runCalc.setObjectName("runCalc")
        self.verticalLayout.addWidget(self.runCalc)
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setText("")
        self.label.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.progressBar = QtWidgets.QProgressBar(self.centralwidget)
        self.progressBar.setProperty("value", 0)
        self.progressBar.setTextVisible(True)
        self.progressBar.setInvertedAppearance(False)
        self.progressBar.setTextDirection(QtWidgets.QProgressBar.Direction.TopToBottom)
        self.progressBar.setObjectName("progressBar")
        self.verticalLayout.addWidget(self.progressBar)
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Подстановка S-параметров для HFSS"))
        self.btnBrowse.setText(_translate("MainWindow", "Выберете папку"))
        self.runCalc.setText(_translate("MainWindow", "Подставить данные"))
