import requests
import json
from BeautifulSoup import BeautifulSoup
url ='https://www.google.com/finance/historical'

reParam = {'q':'KRX:093190', 'start':'01/01/2015', 'end':'09/02/2015', 'num':'200'}
r =requests.get(url, params=reParam)


soup =BeautifulSoup(r.text)
table =soup.find('table', {'class':'gf-table historical_price'})
trs =table.findAll('tr')


stock_list =[]
for tr in trs[1:]:
	stock ={}
	cols =tr.findAll('td')
	stock['date']   =cols[0].text
	stock['open']   =cols[1].text
	stock['high']   =cols[2].text
	stock['low']    =cols[3].text
	stock['close']  =cols[4].text
	stock['volume'] =cols[5].text
	stock_list.append(stock)

j =json.dumps(stock_list)
with open('C:\Users\Juni\Desktop\list.json', 'w') as f:
	f.write(j)


#fn ='C:\Users\Juni\Desktop\list.json'

#fout = open('C:\Users\Juni\Desktop\list3.txt','w')
#count = 0
#with open(fn, 'r') as f:
#	stock_list =json.load(f)
#	for s in stock_list[:]:
#		count+=1
#		#fprint s['full_code'], s['code'][1:], s['name']	
#		data = s['code'][1:] +" "
#		fout.write(data)
#		data = s['name'].encode('utf-8') +"<br>\n"
#		fout.write(data)
		
#print count
