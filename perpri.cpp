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

int main()
{
	FILE			*bat;

	bat = fopen("C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\cmd.bat", "w");
	fprintf(bat, "");
	fclose(bat);

	bat = fopen("C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\cmd.bat", "a");
		
	for (int i = 0; i < 83;i++){
		MYSQL			*connection = NULL, conn;
		MYSQL_RES		*sql_result;
		MYSQL_ROW		sql_row;
		int				query_stat;
		
	
		Price price[ROW_SIZE + 1] = {};

		char pre_txt[] = "select * from stock_price where stock_id = '";
		char post_txt[] = "' order by stock_date desc limit ";

		char jongmok_code[10] = {};
		scanf("%s", jongmok_code);
		
		fprintf(bat,"svm-train ./train/%s_train.txt ./model/%s_model.txt\n", jongmok_code, jongmok_code);
		fprintf(bat,"svm-predict ./target/%s_target.txt ./model/%s_model.txt ./result/%s_result.txt\n", jongmok_code, jongmok_code, jongmok_code);

		char li_sz[] = "1000";
		char qry_str[20000] = "";

		strcat(qry_str, pre_txt);
		strcat(qry_str, jongmok_code);
		strcat(qry_str, post_txt);
		strcat(qry_str, li_sz);

		printf("query = %s", qry_str);

		mysql_init(&conn);
		connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);

		if (connection == NULL)
		{
			printf("%s\n", mysql_error(&conn));
			return -1;
		}

		printf(" DB Connection OK\n");

		//query_stat = mysql_query(connection, "select * from stock_price where stock_id = '003550' order by stock_date desc limit 1000");
		query_stat = mysql_query(connection, qry_str);

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
			price[line_num].stock_open = atoi(sql_row[2]);
			price[line_num].stock_high = atoi(sql_row[3]);
			price[line_num].stock_low = atoi(sql_row[4]);
			price[line_num].stock_close = atoi(sql_row[5]);
			price[line_num].stock_volume = atoi(sql_row[6]);
			line_num--;
		}

		FILE			*o_train, *o_target;
		int				train_size = 600;
		int				target_size = 40;
		int				label_dt = 20;
		int				to_idx = ROW_SIZE;

		char tr_name[1000] = "C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\train\\";
		strcat(tr_name, jongmok_code);
		strcat(tr_name, "_train.txt");

		char tar_name[1000] = "C:\\Users\\Juni\\Desktop\\libsvm-3.20\\libsvm-3.20\\windows\\target\\";
		strcat(tar_name, jongmok_code);
		strcat(tar_name, "_target.txt");

		o_train = fopen(tr_name, "w");
		o_target = fopen(tar_name, "w");

		for (int i = 0; i < ROW_SIZE; i++)
		{
			// 학습
			char label[10] = "5";
			int f_price = 0;

			if (to_idx + label_dt < ROW_SIZE)
			{
				f_price = getMovingAvg(price, to_idx + label_dt, label_dt);
				if ((double)f_price / (double)price[to_idx].stock_close * 100 > 110)
				{
					// 매수
					strcpy(label, "2222");
				}
				else if ((double)f_price / (double)price[to_idx].stock_close * 100 < 90)
				{
					// 매도
					strcpy(label, "1111");
				}
				else if ((double)f_price / (double)price[to_idx].stock_close * 100 > 90 && (double)f_price / (double)price[to_idx].stock_close * 100 < 100)
				{
					// 보류
					strcpy(label, "1100");
				}
				else if ((double)f_price / (double)price[to_idx].stock_close * 100 > 100 && (double)f_price / (double)price[to_idx].stock_close * 100 < 110)
				{
					// 보류
					strcpy(label, "2200");
				}
			}

			if (target_size > i) {
				fprintf(o_target, "%s 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d 14:%d 15:%d 16:%d 17:%d\n",
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
			}
			if (target_size < i && label[0] != '5' && label[0] != '3') {
				fprintf(o_train, "%s 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d 14:%d 15:%d 16:%d 17:%d\n",
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
			}
			to_idx--;
		}

		fclose(o_train);
		fclose(o_target);
		
		mysql_free_result(sql_result);
		mysql_close(&conn);

	}
	fclose(bat);
	return 0;
}


