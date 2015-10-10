import requests
import json
from BeautifulSoup import BeautifulSoup
from datetime import timedelta, date, datetime

import MySQLdb

# DB ����
db = MySQLdb.connect("localhost", "root", "rkdqnr123", "stock")
cursor = db.cursor()

# ��¥ ����
end = datetime(2015, 10, 10)
start = end + delta
delta = timedelta(days=-200)

# ȸ�� �ڵ弳��
company = '009450'

# ũ�Ѹ��� �ʿ� ���
url ='https://www.google.com/finance/historical'
month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
month_in = {'Jan':'1', 'Feb':'2', 'Mar':'3', 'Apr':'4', 'May':'5', 'Jun':'6', 'Jul':'7', 'Aug':'8', 'Sep':'9', 'Oct':'10', 'Nov':'11', 'Dec':'12'}

# DB �ʱ�ȭ
print 'DB reset'
sql = '''DELETE FROM stock_price WHERE stock_id = %s'''%company
cursor.execute(sql)
db.commit()

# 1300�� ���� �޾ƿ�
for i in range(20):
		# ���ۿ� �´� �Ķ���� ����
        paramEnd        = month[end.month-1]  +end.strftime(" %d, %Y")
        paramStart      = month[start.month-1]+start.strftime(" %d, %Y")
        reParam = {'q':'KRX:'+company, 'startdate':paramStart, 'enddate':paramEnd, 'num':'200'}
        r =requests.get(url, params=reParam)

        soup =BeautifulSoup(r.text)
        table =soup.find('table', {'class':'gf-table historical_price'})
        if(table != None):
                trs =table.findAll('tr')
                stock_list =[]
                for tr in trs[1:]:
                        stock ={}
                        cols =tr.findAll('td')
						
                        #��¥ ��ȯ
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
                        
                        sql = '''INSERT INTO stock_price VALUES (%s, %s, %s, %s, %s, %s, %s)'''
                        cursor.execute(sql, (company, stock['date'], stock['open'], stock['high'] , stock['low'], stock['close'], stock['volume']))
                end = start
                start = end+delta
        else:
                break;

db.commit()
cursor.close()
db.close()