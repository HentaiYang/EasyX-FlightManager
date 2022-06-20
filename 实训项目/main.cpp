#include <conio.h>
#include <iostream>
#include "Management.h"
#include "Window.h"
#include "Table.h"

int main() 
{
	Window w(1000, 680, EW_SHOWCONSOLE | EW_NOCLOSE);
	w.setWindowTitle("机票管理系统");
	
	Management m;
	m.run();
	return w.exec();
}

using namespace std;
const int YEAR = 2021;//第一天的年、月、日
const int MONTH = 12;
const int DAY = 20;
const int DAY_COUNT = 2000;//生成天数
const int DAY_FLIGHT_LOW = 200;//每天航班下限
const int DAY_FLIGHT_UP = 400;//每天航班上限
int main11()
{
	ifstream f("./images/diming.txt", ios::in);// 读取地名文件
	if (!f)//判断能否成功打开
	{
		cout << "打开失败" << endl;
		return 0;
	}
	string ss[1024];//用来存地名
	int k = 0;//地名下标
	string s;//读取文件中的地名
	while (f >> s)//持续读取文件直至结束
		ss[k++] = s;//地名与下标一一对应
	f.close();//关闭文件
	ofstream ff("./images/flight.txt", ios::out);//输入文件打开
	if (!ff)//判断是否打开成功
	{
		cout << "打开文件失败" << endl;
		return 0;
	}
	k = 1;//航班号
	time_t now;
	time_t today;
	tm tdtm;
	tm t;//当前时间
	time(&now);
	today = now;
	t = *localtime(&now);
	tdtm = t;
	t.tm_year = YEAR - 1900;
	t.tm_mon = MONTH - 1;
	t.tm_mday = DAY;
	srand(time(NULL));//以当前时间为种子生成随机数
	ff << "班次\t起飞时间\t起点站\t终点站\t飞行时间\t额定载量\t已订票\t是否起飞\t价格" << endl;//写入表头
	for (int i = 0; i < DAY_COUNT; i++, now += 86400)//DAY_COUNT 为生产数据的天数
	{

		t.tm_sec = 0;

		ff << t.tm_year + 1900 << " " << t.tm_mon + 1 << " " << t.tm_mday << " ";//写入日期
		int max_day = 0;//每天的最大航班数量
		while (max_day < DAY_FLIGHT_LOW)
			max_day = rand() % DAY_FLIGHT_UP;
		ff << max_day << endl;//读入每天的航班数量
		for (int j = 1; j <= max_day; j++)//对于每一个航班随机获取数据
		{
			ff << k << '\t';//写入航班号
			k++;
			int hour = rand() % 24;//随机获取小时
			if (hour >= 0 && hour <= 9)
				ff << "0" << hour;
			else
				ff << hour;//前补0，写入小时
			int minute = rand() % 6;
			ff << ":" << minute << "0" << '\t';//随机获取分钟，写入
			t.tm_hour = hour;
			t.tm_min = minute * 10;
			now = mktime(&t);
			int idx = rand() % 251;
			int idx1 = rand() % 251;//获取两个随机下标
			if (idx1 == idx)//避免下标相同
			{
				if (idx1 <= 250)
					idx1++;
				else
					idx1--;
			}
			ff << ss[idx] << '\t' << ss[idx1] << '\t';//写入航班的起始站和终点站
			int ptime = rand() % 8;
			if (ptime == 0)
				ptime++;
			ff << ptime << '\t';//随机获取飞行时间，写入
			int max_ticket = rand() % 251;
			if (max_ticket <= 50) max_ticket += 50;
			int now_ticket = rand() % max_ticket;
			ff << max_ticket << '\t' << now_ticket << '\t';//获取额定票量和已售票量，写入
			if (now > today) ff << "否" << '\t';
			else ff << "是" << '\t';//判断是否起飞，写入
			int ticket = rand() % 4001;
			if (ticket <= 100)
				ticket += 100;
			ff << ticket << endl;//生成票价，写入
		}
		now = mktime(&t);
		now += 86400;
		t = *localtime(&now);
	}
	ff.close();//关闭文件
	return 0;
}