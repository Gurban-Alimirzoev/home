import pandas as pd
from bs4 import BeautifulSoup
import requests
import pathlib

class find_pictures_and_discriptions:
    def __init__(self) -> None:
        self.db = pd.DataFrame(['Image', 'Name'])
        pass
    
    def GetNameUnit(self, url):

        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('div', 'vc_tta-panel-body')
        print(nav)
        for link in nav[0].find_all('a'):
            self.GetNameProducts(self.link.get('href'))
            if self.row > 10:
                break
        pass

    def GetNameProducts(self, url):

        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('div', 'clearfix product-wrapper border')
        for link in nav[0].find_all('a'):
            self.GetImage(self.link.get('href'))
            self.GetDiscription(self.link.get('href'))
            self.GetName(self.link.get('href'))
            if self.row > 10:
                break
        pass


    def GetImage(self, url):

        pass

    def GetDiscriptionAndName(self, url):
        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        discription = soup.find_all('div', 'woocommerce-product-details__short-description')
        lis = []
        
        for iter in discription[0]:
            out = BeautifulSoup(iter, 'html.parser')
            lis.append(out.text) 

        name = soup.find_all('h1', 'product_title entry-title')
        for iter in name[0]:
            out = BeautifulSoup(iter, 'html.parser')
            lis.append(out.text)  

        self.write(lis)
        pass

    
    def WriteData(self):
        current_dir = pathlib.Path.cwd()
        self.db.to_excel(current_dir)
        pass


url_inter = 'https://www.nvc-international.com.sg/products/#1541378743700-1dfe2c4e-977f592b-2c33'
url_exter = 'https://www.nvc-international.com.sg/products/#exterior592b-2c33'



if __name__ == "__main__":
    a = find_pictures_and_discriptions()
    a.GetNameUnit(url_inter)
