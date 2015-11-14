#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <mysql.h>
#include <errno.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "rkdqnr123"
#define DB_NAME "stock"

#define ROW_SIZE 1000

typedef struct _Price
{
	char stock_id[10];
	char stock_date[20];
	int stock_open;
	int stock_high;
	int stock_low;
	int stock_close;
	int stock_volume;
}Price;

typedef struct _Indicator
{
	
	int stock_open;
	int stock_high;
	int stock_low;
	int stock_close;
	int stock_volume;
}Indicator;

////현재 종가, N일전 종가
//int getMomentum(int today_close, int before_close)
//{
//	return today_close / before_close * 100;
//}


int getMovingAvg(Price price[ROW_SIZE], int today_index, int N)
{
	int SUM = 0;
	for (int i = 0; i < N; i++)
	{
		if (today_index - i >= 0)
		{
			SUM += price[today_index - i].stock_close;
		}
		else
		{
			return 0;
		}
	}
	return SUM / N;
}

int getMomentum(Price price[ROW_SIZE], int today_index, int before_index)
{
	return price[today_index].stock_close / price[before_index].stock_close * 100;
}

int getMacd(Price price[ROW_SIZE], int today_index)
{
	int short_per	= 12;
	int long_per	= 26;
	// 단기지수 이동평균 - 장기지수 이동평균
	return getMovingAvg(price, today_index, short_per) - getMovingAvg(price, today_index, long_per);
}

int getStocastic(Price price[ROW_SIZE], int today_index)
{
	int max_price = price[today_index].stock_high;
	int min_price = price[today_index].stock_low;
	int period = 5;

	for (int i = 1; i < period; i++)
	{
		if (max_price < price[today_index-i].stock_high)
			max_price = price[today_index-i].stock_high;
		if (min_price > price[today_index - i].stock_low)
			min_price = price[today_index - i].stock_low;
	}
	return (price[today_index].stock_close - min_price) / (max_price - min_price) * 100;
}

int getRoc(Price price[ROW_SIZE], int today_index)
{
	return price[today_index].stock_close / price[today_index - 4].stock_close * 100;
}

int getWiliamsR(Price price[ROW_SIZE], int today_index)
{
	int max_price = price[today_index].stock_high;
	int min_price = price[today_index].stock_low;
	int period = 10;

	for (int i = 1; i < period; i++)
	{
		if (max_price < price[today_index - i].stock_high)
			max_price = price[today_index - i].stock_high;
		if (min_price > price[today_index - i].stock_low)
			min_price = price[today_index - i].stock_low;
	}

	return (max_price - price[today_index].stock_close) / (max_price - min_price) * 100;
}

int getADOscilator(Price price[ROW_SIZE], int today_index)
{
	return (price[today_index].stock_high - price[today_index - 1].stock_close) / (price[today_index].stock_high - price[today_index].stock_low) * 100;
}

int getDisparity(Price price[ROW_SIZE], int today_index, int degree)
{
	return price[today_index].stock_close / getMovingAvg(price, today_index, degree) * 100;
}

int getOSCP(Price price[ROW_SIZE], int today_index)
{
	return (getMovingAvg(price, today_index, 5) - getMovingAvg(price, today_index, 10)) / getMovingAvg(price, today_index, 5) * 100;
}

int getRSI(Price price[ROW_SIZE], int today_index, int period)
{
	int up_price = 0;
	int down_price = 0;

	for (int i = 0; i < period; i++) 
	{
		if (price[today_index - period + i + 1].stock_close - price[today_index - period + i].stock_close > 0)
		{
			up_price += price[today_index - period + i + 1].stock_close - price[today_index - period + i].stock_close;
		}
		else if (price[today_index - period + i + 1].stock_close - price[today_index - period + i].stock_close < 0)
		{
			down_price += abs(price[today_index - period + i + 1].stock_close - price[today_index - period + i].stock_close);
		}
	}
	return up_price / (up_price + down_price) * 100;
}

//int getBolingerMid(Price price[ROW_SIZE], int today_index, int period)
//{
//	int sum = 0;
//	for (int i = 0; i < period; i++)
//	{
//		sum += price[today_index - i].stock_close;
//	}
//	return sum / period;
//}
//
//int getBolingerHigh(Price price[ROW_SIZE], int today_index, int period, int mid)
//{
//
//}
//
//int getBolingerLow(Price price[ROW_SIZE], int today_index, int period, int mid)
//{
//
//}

int main(void)
{
	MYSQL			*connection = NULL, conn;
	MYSQL_RES		*sql_result;
	MYSQL_ROW		sql_row;
	int				query_stat;

	Price price[ROW_SIZE];

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);

	if (connection == NULL)
	{
		printf("%s\n", mysql_error(&conn));
		return -1;
	}

	printf("Connection OK\n");

	query_stat = mysql_query(connection, "select * from stock_price order by stock_date limit 1000");
	
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	sql_result = mysql_store_result(connection);

	int line_num = 0;
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		strcpy(price[line_num].stock_id, sql_row[0]);
		strcpy(price[line_num].stock_date, sql_row[1]);
		price[line_num].stock_open		= atoi(sql_row[2]);
		price[line_num].stock_high		= atoi(sql_row[3]);
		price[line_num].stock_low		= atoi(sql_row[4]);
		price[line_num].stock_close		= atoi(sql_row[5]);
		price[line_num].stock_volume	= atoi(sql_row[6]);
		line_num++;
	}

	mysql_free_result(sql_result);

	mysql_close(&conn);

	return 0;

}


