import requests
import json
from BeautifulSoup import BeautifulSoup
from datetime import timedelta, date, datetime
url ='https://www.google.com/finance/historical'
month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
delta = timedelta(days=-200)

f= open('C:\Users\Juni\Desktop\list.txt', 'w')
str = 'date\topen\thigh\tlow\tclose\tvolume\n'
f.write(str)
f.close()
f= open('C:\Users\Juni\Desktop\list.txt', 'a')

end = datetime(2015, 8, 30)
start = end + delta
for i in range(55):
	paramEnd 	= month[end.month-1]  +end.strftime(" %d, %Y")
	paramStart 	= month[start.month-1]+start.strftime(" %d, %Y")
	
	reParam = {'q':'KRX:078020', 'startdate':paramStart, 'enddate':paramEnd, 'num':'200'}
	r =requests.get(url, params=reParam)

	soup =BeautifulSoup(r.text)
	table =soup.find('table', {'class':'gf-table historical_price'})
	if(table != None):
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
			str = stock['date']+'\t'+stock['open']+'\t'+stock['high']+'\t'+stock['low']+'\t'+stock['close']+'\t'+stock['volume']+'\n'
			f.write(str)
		end = start
		start = end+delta
	else:
		break;
f.close()

		
		
		
		
		
		
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
