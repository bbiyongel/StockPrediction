#include <stdio.h>
#include <math.h>
#include <string.h>
#include <winsock2.h>
#include <mysql.h>
#include <errno.h>
#include "Indicator.h"

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "rkdqnr123"
#define DB_NAME "stock"

int main(void)
{
	MYSQL			*connection = NULL, conn;
	MYSQL_RES		*sql_result;
	MYSQL_ROW		sql_row;
	int				query_stat;
	

	
	Price price[ROW_SIZE+1];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);

	if (connection == NULL)
	{
		printf("%s\n", mysql_error(&conn));
		return -1;
	}

	printf(" DB Connection OK\n");

	query_stat = mysql_query(connection, "select * from stock_price order by stock_date desc limit 1000");
	
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	sql_result = mysql_store_result(connection);

	int line_num = ROW_SIZE;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		strcpy(price[line_num].stock_id, sql_row[0]);
		strcpy(price[line_num].stock_date, sql_row[1]);
		price[line_num].stock_open		= atoi(sql_row[2]);
		price[line_num].stock_high		= atoi(sql_row[3]);
		price[line_num].stock_low		= atoi(sql_row[4]);
		price[line_num].stock_close		= atoi(sql_row[5]);
		price[line_num].stock_volume	= atoi(sql_row[6]);
		line_num--;
	}

	FILE*			o_train;
	int				label_dt = 20;
	int				to_idx = ROW_SIZE;

	o_train = fopen("o_train.txt", "w");
	for (int i = 0; i < 900; i++) 
	{
		// 학습
		char label[10]		= "3";
		int f_price			= 0;
		
		if (to_idx + label_dt < ROW_SIZE)
		{
			f_price = getMovingAvg(price, to_idx + label_dt, label_dt);
			if ( (double)f_price / (double)price[to_idx].stock_close  * 100 > 110)
			{
				// 매수
				strcpy(label, "2");
			}
			else if ((double)f_price / (double)price[to_idx].stock_close * 100 < 95)
			{
				// 매도
				strcpy(label, "0");
			}
			else
			{
				// 보류
				strcpy(label, "1");
			}
		}
		
		fprintf(o_train,"%s 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d 14:%d 15:%d 16:%d 17:%d\n",
			label,
			getMomentum(price, to_idx, 5),
			getMomentum(price, to_idx, 10),
			getMomentum(price, to_idx, 20),
			getMacd(price, to_idx),
			getStocastic(price, to_idx),
			getRoc(price, to_idx),
			getWiliamsR(price, to_idx),
			getADOscilator(price, to_idx),
			getDisparity(price, to_idx, 5),
			getDisparity(price, to_idx, 10),
			getOSCP(price, to_idx),
			getRSI(price, to_idx, 9),
			getRSI(price, to_idx, 14),
			getRSI(price, to_idx, 28),
			getBolingerWidth(price, to_idx, 5),
			getBolingerWidth(price, to_idx, 10),
			getBolingerWidth(price, to_idx, 20)
			);
		to_idx--;
	}
	//printf(" %s 지표\n", price[to_idx].stock_date);
	//printf(" Momentum(5):%d\n Momentum(10):%d\n Momentum(20):%d\n Macd_rate:%d\n Stocastic:%d\n Roc:%d\n WiliamsR:%d\n ADOscilator:%d\n Disparity(5):%d\n Disparity(10):%d\n OSCP:%d\n RSI(9):%d\n RSI(14):%d\n RSI(28):%d\n BolingerWidth(5):%d\n BolingerWidth(10):%d\n BolingerWidth(20):%d\n",
	//	getMomentum(price, to_idx, 5),
	//	getMomentum(price, to_idx, 10),
	//	getMomentum(price, to_idx, 20),
	//	getMacd(price, to_idx),
	//	getStocastic(price, to_idx),
	//	getRoc(price, to_idx),
	//	getWiliamsR(price, to_idx),
	//	getADOscilator(price, to_idx),
	//	getDisparity(price, to_idx, 5),
	//	getDisparity(price, to_idx, 10),
	//	getOSCP(price, to_idx),
	//	getRSI(price, to_idx, 9),
	//	getRSI(price, to_idx, 14),
	//	getRSI(price, to_idx, 28),
	//	getBolingerWidth(price, to_idx, 5),
	//	getBolingerWidth(price, to_idx, 10),
	//	getBolingerWidth(price, to_idx, 20)
	//	);









	mysql_free_result(sql_result);
	mysql_close(&conn);
	return 0;
}


