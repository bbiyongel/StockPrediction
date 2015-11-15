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

int getMovingAvg(Price price[ROW_SIZE], int today_index, int N);
int getMomentum(Price price[ROW_SIZE], int today_index, int degree);
int getMacd(Price price[ROW_SIZE], int today_index);
int getStocastic(Price price[ROW_SIZE], int today_index);
int getRoc(Price price[ROW_SIZE], int today_index);
int getWiliamsR(Price price[ROW_SIZE], int today_index);
int getADOscilator(Price price[ROW_SIZE], int today_index);
int getDisparity(Price price[ROW_SIZE], int today_index, int degree);
int getOSCP(Price price[ROW_SIZE], int today_index);
int getRSI(Price price[ROW_SIZE], int today_index, int period);
int getBolingerWidth(Price price[ROW_SIZE], int today_index, int period);
