import sys  # sys нужен для передачи argv в QApplication
from PyQt6 import QtWidgets
import designDonil
import sys  # sys нужен для передачи argv в QApplication
import os  # Отсюда нам понадобятся методы для отображения содержимого директорий
from pathlib import Path

class ExampleApp(QtWidgets.QMainWindow, designDonil.Ui_MainWindow):
    def __init__(self):
        # Это здесь нужно для доступа к переменным, методам
        # и т.д. в файле designDonil.py
        super().__init__()
        self.setupUi(self)  # Это нужно для инициализации нашего дизайна
        self.btnBrowse.clicked.connect(self.browse_folder)  # Выполнить функцию browse_folder
                                                            # при нажатии кнопки
        self.runCalc.clicked.connect(self.calculations)

    def browse_folder(self):
        self.listWidget.clear()  # На случай, если в списке уже есть элементы
        self.directory = QtWidgets.QFileDialog.getExistingDirectory(self, "Выберите папку")
        # открыть диалог выбора директории и установить значение переменной
        # равной пути к выбранной директории

        if self.directory:  # не продолжать выполнение, если пользователь не выбрал директорию
            for file_name in os.listdir(self.directory):  # для каждого файла в директории
                self.listWidget.addItem(file_name)   # добавить файл в listWidget

    def calculations(self):
        self.progressBar.setProperty("value", 1)
        listAntEl = [
            '1_2.cti',
            '1_3.cti',
            '1_4.cti',
            '1_5.cti',
            '1_6.cti',
            '2_3.cti',
            '2_4.cti',
            '2_5.cti',
            '2_6.cti',
            '3_4.cti',
            '3_5.cti',
            '3_6.cti',
            '4_5.cti',
            '4_6.cti',
            '5_6.cti'
        ]

        home = Path(self.directory)
        dateDict = {() : {():()} }

        for name in listAntEl:
            file = open(home / name)
            oneOne = file.readlines()[314:615]
            file = open(home / name)
            oneTwo = file.readlines()[617:918]
            file = open(home / name)
            twoOne = file.readlines()[920:1221]
            file = open(home / name)
            twoTwo = file.readlines()[1223:1524]
            dateDict.update({name: {'1_1': oneOne,
            '1_2' : oneTwo,
            '2_1' : twoOne,
            '2_2' : twoTwo}})

        endString = 'END\n'
        beginString = 'BEGIN\n'
        antennaName = 'antenna.cit'

        file = open(home / antennaName)
        lines = file.readlines()[:]
        beginOfAnt = lines[0 : 348]
        endOfAnt = lines[11256 : ]

        with open(home / antennaName, 'w') as file:
            file.writelines(beginOfAnt)

            file.writelines(beginString)
            file.writelines(dateDict['1_2.cti']['1_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_2.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_3.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_4.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_5.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_6.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_2.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_2.cti']['2_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_3.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_4.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_5.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_6.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_3.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_3.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_3.cti']['2_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_4.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_5.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_6.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_4.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_4.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_4.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_4.cti']['2_2'])
            file.writelines(endString)

            file.writelines(beginString)
            file.writelines(dateDict['4_5.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['4_6.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_5.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_5.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_5.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['4_5.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['4_5.cti']['2_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['5_6.cti']['1_2'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['1_6.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['2_6.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['3_6.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['4_6.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['5_6.cti']['2_1'])
            file.writelines(endString)
            
            file.writelines(beginString)
            file.writelines(dateDict['5_6.cti']['2_2'])
            file.writelines(endString)    
            
            file.writelines(endOfAnt)

            self.progressBar.setProperty("value", 100)
            self.label.setText("Готово!")

def gui():
    app = QtWidgets.QApplication(sys.argv)  # Новый экземпляр QApplication
    window = ExampleApp()  # Создаём объект класса ExampleApp
    window.show()  # Показываем окно
    app.exec()  # и запускаем приложение

gui()  # то запускаем функцию main()      

