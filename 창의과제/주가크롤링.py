# -*- coding: cp949 -*-
import MySQLdb
db = MySQLdb.connect("localhost", "root", "rkdqnr123", "stock")
cursor = db.cursor()
#with db:
#   cursor = db.cursor()
#   sql = '''INSERT INTO stock_price VALUES (%s, %s, %s, %s, %s, %s, %s)'''
#    cursor.execute(sql, ('test', 'Aug 28, 2015', 0, 1 ,2, 3, 4))

import requests
import json
from BeautifulSoup import BeautifulSoup
from datetime import timedelta, date, datetime
url ='https://www.google.com/finance/historical'
month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
month_in = {'Jan':'1', 'Feb':'2', 'Mar':'3', 'Apr':'4', 'May':'5', 'Jun':'6', 'Jul':'7', 'Aug':'8', 'Sep':'9', 'Oct':'10', 'Nov':'11', 'Dec':'12'}
delta = timedelta(days=-200)

f= open('C:\Users\Juni\Desktop\list.txt', 'w')
str = 'date\topen\thigh\tlow\tclose\tvolume\n'
f.write(str)
f.close()

# 파일 저장 경로
f= open('C:\Users\Juni\Desktop\list.txt', 'a')

# DB 초기화
sql = '''DELETE FROM stock_price WHERE stock_id = %s'''%'078020'
cursor.execute(sql)
db.commit()

end = datetime(2015, 8, 30)
start = end + delta
for i in range(55):
        paramEnd        = month[end.month-1]  +end.strftime(" %d, %Y")
        paramStart      = month[start.month-1]+start.strftime(" %d, %Y")
        
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
                        #날짜 변환
                        date_temp = cols[0].text
                        date_temp = date_temp.replace(',','')
                        date_temp = date_temp.split(' ')
                        day = date_temp[2]+'.'+month_in[date_temp[0]]+'.'+date_temp[1]
                        stock['date']   =day
                        stock['open']   =(cols[1].text).replace(',','')
                        stock['high']   =(cols[2].text).replace(',','')
                        stock['low']    =(cols[3].text).replace(',','')
                        stock['close']  =(cols[4].text).replace(',','')
                        stock['volume'] =(cols[5].text).replace(',','')
#str = stock['date']+'\t'+stock['open']+'\t'+stock['high']+'\t'+stock['low']+'\t'+stock['close']+'\t'+stock['volume']+'\n'
                        
                        sql = '''INSERT INTO stock_price VALUES (%s, %s, %s, %s, %s, %s, %s)'''
                        cursor.execute(sql, ('078020', stock['date'], stock['open'], stock['high'] , stock['low'], stock['close'], stock['volume']))

                        f.write(str)
                end = start
                start = end+delta
        else:
                break;

db.commit()
cursor.close()
db.close()
f.close()

                
                
                
                
                
                
#fn ='C:\Users\Juni\Desktop\list.json'

#fout = open('C:\Users\Juni\Desktop\list3.txt','w')
#count = 0
#with open(fn, 'r') as f:
#       stock_list =json.load(f)
#       for s in stock_list[:]:
#               count+=1
#               #fprint s['full_code'], s['code'][1:], s['name']        
#               data = s['code'][1:] +" "
#               fout.write(data)
#               data = s['name'].encode('utf-8') +"<br>\n"
#               fout.write(data)
                
#print count

                
                
                
                
                
                
                
#fn ='C:\Users\Juni\Desktop\list.json'

#fout = open('C:\Users\Juni\Desktop\list3.txt','w')
#count = 0
#with open(fn, 'r') as f:
#       stock_list =json.load(f)
#       for s in stock_list[:]:
#               count+=1
#               #fprint s['full_code'], s['code'][1:], s['name']        
#               data = s['code'][1:] +" "
#               fout.write(data)
#               data = s['name'].encode('utf-8') +"<br>\n"
#               fout.write(data)
                
#print count
