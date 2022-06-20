#pragma once
#include <algorithm>
#include "Flight.h"
using namespace std;
class ArraySort
{
public:
	static int cmp_lps(const void* a, const void* b);
	static int cmp_hps(const void* a, const void* b);//�߼�����
	static int cmp_lbs(const void* a, const void* b);//�������ʱ������
	static int cmp_hbs(const void* a, const void* b);//�������ʱ������
	static int cmp_sts(const void* a, const void* b);//��̷���ʱ������
	static int cmp_lts(const void* a, const void* b);//�����ʱ������
	static void lowPriceSort(std::vector<Flight>& data); //�ͼ�����
	static void highPriceSort(std::vector<Flight>& data);//�߼�����
	static void lowBeginSort(std::vector<Flight>& data); //�������ʱ������
	static void highBeginSort(std::vector<Flight>& data);//�������ʱ������
	static void shortTimeSort(std::vector<Flight>& data);//��̷���ʱ������
	static void longTimeSort(std::vector<Flight>& data); //�����ʱ������
	static int cmp_def(const void* a, const void* b);
	static void defSort(std::vector<Flight>& data);//Ĭ�ϰ��������

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

