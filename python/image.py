from PIL import Image
import os
import xlwings as xw
import pathlib
# app=xw.App(visible=True,add_book=False)

wb = xw.Book(r'F: \ envs_pycharm \ Amazon загрузить изображение \ 1-2020-загрузить изображение-и варианты \ DetailPage \ DetailPage \ change_asin \ Ву Иньлиню требуется сумка для стропы для сбора данных sling bag.xlsx')

#     Insert
sht = wb.sheets[«У Иньлинь попросила собрать данные из слинг-сумки»]
print(sht.range("B2").expand('down').value)
asin_list=sht.range("B2").expand('down').value
print(len(asin_list))

def write_pic(cell,asin):
    path=f'./pic_0415/{asin}.jpg'
    print(path)
    fileName = os.path.join(os.getcwd(), path)
    img = Image.open(path).convert("RGB")
    print(img.size)
    w, h = img.size
    x_s = 70  # Установить ширину В excel я установил формат 200x200
    y_s = h * x_s / w  # Установить высокую пропорцию
    sht.pictures.add(fileName, left=sht.range(cell).left, top=sht.range(cell).top, width=x_s, height=y_s)


if __name__ == '__main__':
    print("start")
    for index,value in enumerate(asin_list):
        cell="A"+str(index+2)
        try:
            write_pic(cell,value)
        except:
            print(«Изображение этого ASIN не найдено»,value)
    wb.save()