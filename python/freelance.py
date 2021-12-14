from bs4 import BeautifulSoup
import requests
import pandas as pd

class Parser:
    def __init__(self) -> None:
        self.prefix = "https://www.vidal.fr/"
        self.db = pd.DataFrame({'medic':[], 'atribut': []})
        self.row = 0

    def write(self, data):
        self.row +=1
        self.db.loc[self.row] = data


    def go_to_copy(self, url):
        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('div', 'name')
        lis = []
        
        for iter in nav[0]:
            out = BeautifulSoup(iter, 'html.parser')
            lis.append(out.text) 

        nav = soup.find_all('span', 'cip13')
        for iter in nav[0]:
            out = BeautifulSoup(iter, 'html.parser')
            lis.append(out.text)  

        self.write(lis)


    def go_to_medi(self, url):
        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('div', 'consume-info')
        for link in nav[0].find_all('a'):
            self.go_to_copy(self.prefix + link.get('href'))


    def go_to_unit(self, url):
        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('div', 'list')
        for link in nav[0].find_all('a'):
            self.go_to_medi(self.prefix + link.get('href'))
            if self.row > 10:
                break

    def go_to_alfa(self):
        url = "https://www.vidal.fr/medicaments/gammes/liste-0-9.html"
        page = requests.get(url)
        soup = BeautifulSoup(page.text, "html.parser")
        nav = soup.find_all('ul', 'alpha')
        for link in nav[0].find_all('a'):
            self.go_to_unit(self.prefix + link.get('href'))
            if self.row > 10:
                break
        self.db.to_excel("medic.xls", index=False)


a = Parser()
a.go_to_alfa()