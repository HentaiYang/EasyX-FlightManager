#include "Management.h"
#include <iostream>
#include <conio.h>
#include <fstream>//读写数据

using uint32 = unsigned int;
using namespace std;
Management::Management()
{
	Window::beginDraw();
	::loadimage(&m_bk, "./images/open_bk.png", Window::width(), Window::height());
	Window::clear();
	drawBackground();//写入背景
	Window::flushDraw();//显示开机画面
	readFile("./images/flight.txt");	//主界面初始化时读入文件
	
	::settextstyle(20, 0, "宋体");//	设置默认字体
	::settextcolor(BLACK);
	
	//主界面按钮初始化
	menu_btns.push_back(new PushButton("航班信息管理"));
	menu_btns.push_back(new PushButton("查询线路/订票退票"));
	menu_btns.push_back(new PushButton("退出系统"));

	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->setFixedSize(250, 50); //重设置按键大小

		int bx = (Window::width() - menu_btns[i]->width()) / 2;	//使左右居中
		int vspace = (Window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;	//算出第一个按钮应该距离上面多远
		int by = vspace + i * menu_btns[i]->height();	//分别设置按钮的高度
		menu_btns[i]->move(bx, by);		//按钮居中窗口
	}

	//初始化查看航班的表格
	m_showTable = new Table(16, 0);
	//m_showTable->setRowCount(16);		//设置行数
	//m_showTable->setColCount(8); 用setHeader根据表头定列数，兼容性更好
	m_showTable->setHeader(m_header);	//设置列数和文字大小
	m_showTable->insertData(vec_all);//将vec_all复制一份到表格类
	m_showTable->setToday();//没地方放了，放循环里浪费
	//使表格居中
	m_showTable->move((Window::width() - m_showTable->width()) / 2, 50);
	m_showTable->setButton(0);

	//初始化查询的表格
	m_queryTable = new QueryTable(5, 1);
	m_queryTable->insertData(vec_all);
	m_queryTable->setToday();//没地方放了，放循环里浪费
	m_queryTable->readCity("./images/city.txt");//读入城市文件
	//使表格居中
	m_queryTable->move((Window::width() - m_queryTable->width()) / 2, 50);
	m_queryTable->setButton(1);

	::loadimage(&m_bk, "./images/home_bk.png", Window::width(), Window::height());
}
void Management::run()
{
	int op = Menu;
	//为了做开启画面，写在构造函数 Window::beginDraw();//预缓冲
	while(true)
	{
		Window::clear();
		drawBackground();//写入背景
		if (Window::hasMsg())
		{
			m_msg = Window::getMsg();
			switch (m_msg.message)
			{
			case WM_KEYDOWN:	//按键按下
				if (m_msg.vkcode == VK_ESCAPE)
				{
					::loadimage(&m_bk, "./images/home_bk.png", Window::width(), Window::height());
					op = Menu;
					if(m_showTable->open) 
					{
						m_queryTable->insertData(vec_all);
						m_showTable->refreshTable();
						m_showTable->open = false;
					}
					if (m_queryTable->open)
					{
						m_showTable->insertData(vec_all);
						m_queryTable->refreshTable();
						m_queryTable->open = false;
					}
				}
				break;
			default:  //鼠标操作
				eventLoop();
				break;
			}
		}
		switch (op)
		{
		case Menu:
			op = menu();
			break;
		case Management::Edit:
			edit();
			break;
		case Management::Query_Buy_Return:
			query();
			break;
		default:
			::loadimage(&m_bk, "./images/close_bk.png", Window::width(), Window::height());
			Window::clear();
			drawBackground();
			Window::flushDraw();
			saveFile("./images/flight.txt");//退出系统前保存文件
			exit(666);
			break;
		}

		Window::flushDraw();//显示背景及按钮

	}
	Window::endDraw();
}

int Management::menu()
{
	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->show();	//显示按钮
		menu_btns[i]->eventLoop(m_msg);		//事件循环，这里的m_msg会读取键盘鼠标，那么传到button类中就可以直接使用
		if (menu_btns[i]->isClicked())		//如果按钮被点击
		{
			if(i == Edit) ::loadimage(&m_bk, "./images/bk3.jpg", Window::width(), Window::height());
			if(i == Query_Buy_Return) ::loadimage(&m_bk, "./images/bk3.jpg", Window::width(), Window::height());
			m_msg.message = 0;
			m_msg.lbutton = false;
			m_msg.x = m_msg.y = 0;
			return i;
		}
	}
    return Menu;
}

void Management::edit()
{
	m_showTable->show();
}

void Management::query()
{
	m_queryTable->show();
}

void Management::drawBackground()
{
	::putimage(0, 0, &m_bk);
}

void Management::eventLoop()
{
	if(m_showTable->open) m_showTable->eventLoop(m_msg);
	if(m_queryTable->open) m_queryTable->eventLoop(m_msg);
}

void Management::resetButton()
{
	//主界面按钮初始化
	menu_btns.push_back(new PushButton("显示航班信息"));
	menu_btns.push_back(new PushButton("编辑航班信息"));
	menu_btns.push_back(new PushButton("查询航班信息"));
	menu_btns.push_back(new PushButton("售票"));
	menu_btns.push_back(new PushButton("订票"));
	menu_btns.push_back(new PushButton("退出系统"));

	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->setFixedSize(250, 50); //重设置按键大小

		int bx = (Window::width() - menu_btns[i]->width()) / 2;	//使左右居中
		int vspace = (Window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;	//算出第一个按钮应该距离上面多远
		int by = vspace + i * menu_btns[i]->height();	//分别设置按钮的高度
		menu_btns[i]->move(bx, by);		//按钮居中窗口
	}
}

void Management::readFile(const std::string& fileName)
{
	fstream read(fileName, ios::in);//读入文件
	if (!read.is_open())			//如果没有打开文件就返回
	{
		return;
	}
	char buf[1024] = { 0 };
	read.getline(buf, 1024);
	m_header = buf;				//读取表头（文件中以表的形式存储）

	while (!read.eof())			//读取数据
	{
		char data1[1024] = { 0 };
		read.getline(data1, 1024);//第一行存有年 月 日 航班数
		if (strlen(data1) == 0)
			break;				//万一文件尾有空行则跳出
		stringstream ss1(data1);
		Day_Flight dflt;		//建立一天的航班暂存表
		ss1 >> dflt.year >> dflt.month >> dflt.day >> dflt.n;
		for (int i = 0; i < dflt.n; i++) {
			char data2[1024] = { 0 };
			read.getline(data2, 1024);//读取每行的航班信息
			Flight flt;
			stringstream ss(data2);

			ss >> flt.number >> flt.takeoff_time >> flt.start >> flt.end >> flt.during_time >> flt.max_load >> flt.booked >> flt.take_off >> flt.price;
			//测试输出 cout << flt.number << flt.hour << flt.minute << flt.start << flt.end << flt.during_time << flt.max_load << flt.booked << flt.take_off << endl;
			flt.init();		//补全hour、minute、take_off_bool的值
			dflt.vec_flt.push_back(flt);//存入当日航班表
		}
		vec_all.push_back(dflt);		//单日航班表存入总表vec_all
	}
	//更新航班起飞情况
	time_t today;
	tm today_tm;
	time(&today);
	today_tm = *localtime(&today);//更新今天时间
	for (size_t i = 0; i < vec_all.size(); i++)
	{
		if (vec_all[i].year == today_tm.tm_year + 1900 && vec_all[i].month == today_tm.tm_mon + 1 && vec_all[i].day == today_tm.tm_mday)
		{
			for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
			{
				//如果该航班起飞时间大于现在时间，说明没起飞
				if (vec_all[i].vec_flt[j].hour * 60 + vec_all[i].vec_flt[j].minute > today_tm.tm_hour * 60 + today_tm.tm_min)
				{
					vec_all[i].vec_flt[j].take_off = "否";
					vec_all[i].vec_flt[j].take_off_bool = false;
				}
				else//否则已起飞
				{
					vec_all[i].vec_flt[j].take_off = "是";
					vec_all[i].vec_flt[j].take_off_bool = true;
				}
			}
		}
		else
		{
			tm day = today_tm;
			day.tm_year = vec_all[i].year - 1900;
			day.tm_mon = vec_all[i].month - 1;
			day.tm_mday = vec_all[i].day;
			time_t d = mktime(&day);
			if (d < today)//比今天早，说明都起飞了
			{
				for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
				{
					vec_all[i].vec_flt[j].take_off = "是";
					vec_all[i].vec_flt[j].take_off_bool = true;
				}
			}
			else//比今天晚，都没起飞
			{
				for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
				{
					vec_all[i].vec_flt[j].take_off = "否";
					vec_all[i].vec_flt[j].take_off_bool = false;
				}
			}
		}
	}
	//for (int i = 0; i < vec_all.size(); i++)
	//{
	//	vec_all_copy.push_back(vec_all[i]);	//复制数据到备用数组vec_all_copy
	//}
	read.close();
}

void Management::saveFile(const std::string& fileName)
{
	fstream write(fileName, ios::out | ios::trunc); //打开文件，如果存在就写入方法，不存在就创建方法
	if (!write.is_open())
	{
		cerr << fileName << "文件未能打开！" << endl; //用错误输出流输出错误信息
		return;
	}

	m_header += "\n";				//表头行尾加换行，否则少换行
	write.write(m_header.c_str(), m_header.size());//写入表头

	for (auto& val_day : vec_all)			//遍历每天
	{
		//测试输出 cout << val_day.year << "年" << val_day.month << "月" << val_day.day << "日 当日航班数：" << val_day.n << endl;
		std::string day_title = val_day.formatInfo();
		write.write(day_title.c_str(), day_title.size());

		for (auto& val : val_day.vec_flt)	//遍历当天的所有航班
		{
			//测试输出 cout << val.formatInfo();
			std::string info = val.formatInfo();	//返回的是string类型
			write.write(info.c_str(), info.size());	//write要求类型为c风格的str
		}
	}

	write.close();
}
