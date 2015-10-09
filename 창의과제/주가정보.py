#import pandas.io.data as web
#import datetime

#start = datetime.datetime(2015, 2, 2)
#end = datetime.datetime(2015, 2, 13)
#gs = web.DataReader("KRX:036570", "yahoo", start, end)

import numpy as np
import pandas as pd
import pandas.io.data as web
 
#ncsoft = web.DataReader('KRX:093190', data_source='google', start='01/01/2015', end='09/02/2015')
ncsoft = web.DataReader(cid='914427320792113', data_source='google', start='01/01/2015', end='09/02/2015')
ncsoft

http://www.google.com/finance/historical?q=KRX%3A093190&startdate=Jan+01%2C+2015&enddate=Sep+02%2C+2015&output=csv


http://www.google.com/finance/historical?q=KRX%3A093190&startdate=Aug+01%2C+2015&enddate=Aug+25%2C+2015&output=csv

