#pragma once
#include "Configure.h"
#include <sstream>
#include <vector>
//从现实角度和实现难度考虑，认为每天都有航班
using uint32 = unsigned int;
class Flight
{
public:
	Flight();
	Flight(uint32 number, const std::string& takeoff_time, const std::string& start, const std::string& end,
		uint32 during_time, uint32 max_load, uint32 booked, std::string take_off, uint32 price);

	void init();				//将无参构造的数据的其他值补全
	std::string formatInfo();	//格式化数据
public:		
	uint32 number;				//飞机班次
	uint32 hour;
	uint32 minute;				//起飞时间
	std::string takeoff_time;	//起飞时间
	std::string start;			//始发站
	std::string end;			//终点站
	uint32 during_time;			//飞行时间
	uint32 max_load;			//额定载量
	uint32 booked;				//已订票数量
	std::string take_off;		//是否起飞
	bool take_off_bool;			//是否起飞布尔值
	uint32 price;				//机票价格
};
//封装每天的航班
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
//用于查询的结构
struct Result {
public:
	uint32 year, month, day, n;//n<=2，最多中转一次
	std::vector<Flight*> flt;//指针变量，用于买票和退票
};
//用于有向图的结构
struct Map {
public:
	uint32 year, month, day;
	std::vector<Flight*> flt[255];	//每天的有向图邻接表
};

