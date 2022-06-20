#pragma once
#include "Window.h"
#include "PushButton.h"
#include "QueryTable.h"
#include "Flight.h"
#include "Table.h"
#include <vector>
#include <sstream>//分割数据
#include <map>
using uint32 = unsigned int;
class Management
{
	enum Operator
	{
		Edit,
		Query_Buy_Return,
		Menu = 666
	};
public:
	Management();
	//启动管理类
	void run();

	int menu();

	//显示所有航班
	void edit();
	//查询、售票和退票
	void query();

	void drawBackground();
	void eventLoop();
	void resetButton();

	void readFile(const std::string& fileName);
	void saveFile(const std::string& fileName);

private:
	IMAGE m_bk;
	ExMessage m_msg;

	std::vector<PushButton*> menu_btns;

private://用于保存文件中航班信息
	std::string m_header;	//保存表头
	std::vector<Day_Flight> vec_all;//存所有的航班，一天为一个数据
	std::vector<Day_Flight> vec_all_copy;//备用数组，用于排序或恢复数据
private:
	Table* m_showTable;//查看用表格
	QueryTable* m_queryTable;//查询用表格
};

