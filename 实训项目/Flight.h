#pragma once
#include "Configure.h"
#include <sstream>
#include <vector>
//����ʵ�ǶȺ�ʵ���Ѷȿ��ǣ���Ϊÿ�춼�к���
using uint32 = unsigned int;
class Flight
{
public:
	Flight();
	Flight(uint32 number, const std::string& takeoff_time, const std::string& start, const std::string& end,
		uint32 during_time, uint32 max_load, uint32 booked, std::string take_off, uint32 price);

	void init();				//���޲ι�������ݵ�����ֵ��ȫ
	std::string formatInfo();	//��ʽ������
public:		
	uint32 number;				//�ɻ����
	uint32 hour;
	uint32 minute;				//���ʱ��
	std::string takeoff_time;	//���ʱ��
	std::string start;			//ʼ��վ
	std::string end;			//�յ�վ
	uint32 during_time;			//����ʱ��
	uint32 max_load;			//�����
	uint32 booked;				//�Ѷ�Ʊ����
	std::string take_off;		//�Ƿ����
	bool take_off_bool;			//�Ƿ���ɲ���ֵ
	uint32 price;				//��Ʊ�۸�
};
//��װÿ��ĺ���
struct Day_Flight {
public:
	uint32 year, month, day, n;
	std::vector<Flight> vec_flt;

	std::string formatInfo() {
		std::stringstream ss;
		ss << year << " " << month << " " << day << " " << n << std::endl;
		return ss.str();
	}
};
//���ڲ�ѯ�Ľṹ
struct Result {
public:
	uint32 year, month, day, n;//n<=2�������תһ��
	std::vector<Flight*> flt;//ָ�������������Ʊ����Ʊ
};
//��������ͼ�Ľṹ
struct Map {
public:
	uint32 year, month, day;
	std::vector<Flight*> flt[255];	//ÿ�������ͼ�ڽӱ�
};

