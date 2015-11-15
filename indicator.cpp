#include "Indicator.h"
#include <math.h>

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

int getMomentum(Price price[ROW_SIZE], int today_index, int degree)
{
	double result = (double)price[today_index].stock_close / (double)price[today_index - degree].stock_close * 100;
	return result;
}

int getMacd(Price price[ROW_SIZE], int today_index)
{
	int short_per = 12;
	int long_per = 26;
	// 단기지수 이동평균 - 장기지수 이동평균
	// -에서 /로 정규화
	return (double)getMovingAvg(price, today_index, short_per) / (double)getMovingAvg(price, today_index, long_per) * 100;
}

int getStocastic(Price price[ROW_SIZE], int today_index)
{
	int max_price = price[today_index].stock_high;
	int min_price = price[today_index].stock_low;
	int period = 5;

	for (int i = 1; i < period; i++)
	{
		if (max_price < price[today_index - i].stock_high)
			max_price = price[today_index - i].stock_high;
		if (min_price > price[today_index - i].stock_low)
			min_price = price[today_index - i].stock_low;
	}
	return (double)(price[today_index].stock_close - min_price) / (double)(max_price - min_price) * 100;
}

int getRoc(Price price[ROW_SIZE], int today_index)
{
	return (double)price[today_index].stock_close / (double)price[today_index - 4].stock_close * 100;
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

	return (double)(max_price - price[today_index].stock_close) / (double)(max_price - min_price) * 100;
}

int getADOscilator(Price price[ROW_SIZE], int today_index)
{
	return (double)(price[today_index].stock_high - price[today_index - 1].stock_close) / (double)(price[today_index].stock_high - price[today_index].stock_low) * 100;
}

int getDisparity(Price price[ROW_SIZE], int today_index, int degree)
{
	return (double)price[today_index].stock_close / (double)getMovingAvg(price, today_index, degree) * 100;
}

int getOSCP(Price price[ROW_SIZE], int today_index)
{
	return (double)(getMovingAvg(price, today_index, 5) - getMovingAvg(price, today_index, 10)) / (double)getMovingAvg(price, today_index, 5) * 100;
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
	return (double)up_price / (double)(up_price + down_price) * 100;
}

// default period = 20
int getBolingerWidth(Price price[ROW_SIZE], int today_index, int period)
{
	int mid = getMovingAvg(price, today_index, period);
	int d = 2;

	// sum (종가 - 중심선)^2
	int result = 0;
	for (int i = 0; i < period; i++)
	{
		result += (price[today_index - i].stock_close - mid)*(price[today_index - i].stock_close - mid);
	}

	result = (double)sqrt((double)result / (double)period) * d;
	
	// 정규화
	return (double)(result+mid) / (double)mid * 100;
}
