import requests
import json
from BeautifulSoup import BeautifulSoup
from datetime import timedelta, date, datetime
import MySQLdb
import time

# DB 연결
db = MySQLdb.connect("localhost", "root", "rkdqnr123", "stock")
cursor = db.cursor()

# 회사 코드설정
com_fd = open('company_list.txt', 'r')

# range
for com_idx in range(1):
        # 날짜 설정
        end = datetime(2015, 11, 29)
        delta = timedelta(days=-200)
        start = end + delta

        com_line = com_fd.readline()
        com_split = com_line.split(' ')
        company = com_split[0]
        company = '001260'

        if company == '':
                break;
        
        # 크롤링에 필요 요소
        url ='https://www.google.com/finance/historical'
        month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
        month_in = {'Jan':'1', 'Feb':'2', 'Mar':'3', 'Apr':'4', 'May':'5', 'Jun':'6', 'Jul':'7', 'Aug':'8', 'Sep':'9', 'Oct':'10', 'Nov':'11', 'Dec':'12'}

        # DB 초기화
        print 'DB reset'
        sql = '''DELETE FROM stock_price WHERE stock_id = '%s' '''%company
        cursor.execute(sql)
        db.commit()

        # 1300일 정도 받아옴
        print 'company = ' + company
        for i in range(20):
                # 구글에 맞는 파라미터 생성
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

                                if stock['open'] == '-':
                                    break;
                                sql = '''INSERT INTO stock_price VALUES (%s, %s, %s, %s, %s, %s, %s)'''
                                cursor.execute(sql, (company, stock['date'], stock['open'], stock['high'] , stock['low'], stock['close'], stock['volume']))
                        end = start
                        start = end+delta
                else:
                        break;
                db.commit()
                time.sleep(0.3)
                
com_fd.close()
cursor.close()
db.close()
