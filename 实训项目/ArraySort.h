#pragma once
#include <algorithm>
#include "Flight.h"
using namespace std;
class ArraySort
{
public:
	static int cmp_lps(const void* a, const void* b);
	static int cmp_hps(const void* a, const void* b);//高价优先
	static int cmp_lbs(const void* a, const void* b);//最早起飞时间优先
	static int cmp_hbs(const void* a, const void* b);//最晚起飞时间优先
	static int cmp_sts(const void* a, const void* b);//最短飞行时间优先
	static int cmp_lts(const void* a, const void* b);//最长飞行时间优先
	static void lowPriceSort(std::vector<Flight>& data); //低价优先
	static void highPriceSort(std::vector<Flight>& data);//高价优先
	static void lowBeginSort(std::vector<Flight>& data); //最早起飞时间优先
	static void highBeginSort(std::vector<Flight>& data);//最晚起飞时间优先
	static void shortTimeSort(std::vector<Flight>& data);//最短飞行时间优先
	static void longTimeSort(std::vector<Flight>& data); //最长飞行时间优先
	static int cmp_def(const void* a, const void* b);
	static void defSort(std::vector<Flight>& data);//默认按班次排序

	static bool cmp_rlps(Result a, Result b);
	static bool cmp_rhps(Result a, Result b);
	static bool cmp_rlbs(Result a, Result b);
	static bool cmp_rhbs(Result a, Result b);
	static bool cmp_rsts(Result a, Result b);
	static bool cmp_rlts(Result a, Result b);
	static bool cmp_rstt(Result a, Result b);
	static void lowPriceSort(std::vector<Result>& data);
	static void highPriceSort(std::vector<Result>& data);
	static void lowBeginSort(std::vector<Result>& data);
	static void highBeginSort(std::vector<Result>& data);
	static void shortTimeSort(std::vector<Result>& data);
	static void longTimeSort(std::vector<Result>& data);
	static void straightSort(std::vector<Result>& data);
private:
	
};

