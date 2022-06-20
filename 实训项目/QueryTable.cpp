#include "QueryTable.h"
#include <iostream>
const int Table_Move_X = 0;		//对表格的左右移动
const int Table_Move_Y = 70;	//对表格的上下移动
const int Table_Move_Btn9_X = 0;//上面九个按钮的左右移动
const int Table_Move_Btn9_Y = 0;//上面九个按钮的上下移动
const int Table_Move_Btn4_X = 380;//下面四个按钮的左右移动
const int Table_Move_Btn4_Y = 80;//下面四个按钮的上下移动
const int Page_Move_X = 10;			//页码数左右移动
const int Sort_Move_X = 380;			//排序键的左右移动
const int Sort_Move_Y = 20;			//排序键的上下移动
const int Table_Move_City_X = 0;
const int Table_Move_City_Y = 0;
const int Buy_Move_X = -145;
const int Buy_Space_Size = 15;
QueryTable::QueryTable(int row, int col)
	:BasicWidget(0, 0, 0, 0)	//初始化父类数据
	, m_rows(row), m_cols(col)//初始化行列数
	, m_curPage(0), m_maxPage(0), m_exrtaData(0)//初始化页相关数据
{
	m_prevBtn = new PushButton("上一页");//上一页按钮
	m_nextBtn = new PushButton("下一页");//下一页按钮
	m_firstBtn = new PushButton("首页");//首页按钮
	m_lastBtn = new PushButton("尾页");//尾页按钮
	//下面的三个按钮都要确定日期才可以显示，默认日期为今天
	m_prevWeek = new PushButton("<<");//上周按钮
	m_nextWeek = new PushButton(">>");//下周按钮
	for (int i = 0; i < 7; i++) {		//创建一周的按钮
		m_oneWeek.push_back(new PushButton("无数据"));
	}

	time(&today);
	show_day = today;
	today_tm = *localtime(&today);
	select_sortbtn = -1;

	//设置文字宽度和高度，单位为像素
	m_tw = ::textwidth("乌鲁木齐");
	m_th = ::textheight("乌鲁木齐");

	//设置格子宽度和高度，单位为像素
	m_gridW = 900;
	m_gridH = 100;

	//更新表格宽度和高度
	m_w = m_gridW * m_cols;
	m_h = m_gridH * m_rows;

	//未加载表格，会报错 refreshShowday_OneWeek();//更新现在展示的时间和一周的按钮

	m_sortBtn.push_back(new PushButton("低价优先"));
	m_sortBtn.push_back(new PushButton("高价优先"));
	m_sortBtn.push_back(new PushButton("起飞早-晚"));
	m_sortBtn.push_back(new PushButton("起飞晚-早"));
	m_sortBtn.push_back(new PushButton("时间短-长"));
	m_sortBtn.push_back(new PushButton("时间长-短"));
	m_startBtn = new PushButton("出发地");
	m_endBtn = new PushButton("目的地");
	m_changeBtn = new PushButton("地点");
	m_numberBtn = new PushButton("班次号");
	//m_dateBtn = new PushButton("出发日期");
}

QueryTable::~QueryTable()//防止内存泄漏
{
	delete m_prevBtn;
	delete m_nextBtn;
	delete m_firstBtn;
	delete m_lastBtn;
	delete m_prevWeek;
	delete m_nextWeek;
	for (int i = 0; i < 7; i++) {
		delete m_oneWeek[i];
	}
}

void QueryTable::setRowCount(int row)
{
	m_rows = row;
}

void QueryTable::setColCount(int col)
{
	m_cols = col;
}

void QueryTable::setButton(int flag)
{
	for (size_t i = 0; i < m_rows; i++)
	{
		m_buyBtn.push_back(new PushButton("购票"));
		m_returnBtn.push_back(new PushButton("退票"));
	}
	//初始化按钮位置
	m_prevBtn->move(m_x + Table_Move_Btn4_X, m_y + m_h + Table_Move_Btn4_Y);
	m_nextBtn->move(m_prevBtn->x() + m_prevBtn->width(), m_prevBtn->y());
	m_firstBtn->setFixedSize(80, 30);
	m_lastBtn->setFixedSize(80, 30);
	m_firstBtn->move(m_nextBtn->x() + m_nextBtn->width(), m_nextBtn->y());
	m_lastBtn->move(m_firstBtn->x() + m_firstBtn->width(), m_firstBtn->y());
	int bw = 110;
	int bh = 40;	//上面的七个按钮规格一样
	m_prevWeek->setFixedSize(bh, bh);
	m_nextWeek->setFixedSize(bh, bh);
	m_prevWeek->move(Table_Move_Btn9_X + (Window::width() - 100 - bw * 7) / 2, Table_Move_Btn9_Y);//以第一个按钮为准
	m_nextWeek->move(m_prevWeek->x() + m_prevWeek->width() + bw * 7, m_prevWeek->y());
	for (size_t i = 0; i < 7; i++) {
		m_oneWeek[i]->setFixedSize(bw, bh);
		m_oneWeek[i]->move(m_prevWeek->x() + m_prevWeek->width() + i * bw, m_prevWeek->y());
	}

	for (size_t i = 0; i < 6; i++)//排序按钮设置
	{
		int tw = 90;	//排序按钮大小
		int th = 26;
		m_sortBtn[i]->setFixedSize(tw, th);
		m_sortBtn[i]->move(m_x + Sort_Move_X + tw * i, m_y + Sort_Move_Y);
	}

	bw = 150;
	bh = 60;
	m_startBtn->setFixedSize(bw, bh);
	m_endBtn->setFixedSize(bw, bh);
	//m_dateBtn->setFixedSize(bw + 30, bh);
	m_startBtn->setBackgroundColor(WHITE);
	m_endBtn->setBackgroundColor(WHITE);
	//m_dateBtn->setBackgroundColor(WHITE);
	m_startBtn->setHoverColor(RGB(204, 213, 240));
	m_endBtn->setHoverColor(RGB(204, 213, 240)); 
	//m_dateBtn->setHoverColor(RGB(204, 213, 240));
	m_startBtn->moveText(-40, -20);
	m_endBtn->moveText(-40, -20);
	//m_dateBtn->moveText(-50, -20);

	m_startBtn->move(m_x + Table_Move_City_X, m_y + Table_Move_City_Y);
	m_endBtn->move(m_startBtn->x() + m_startBtn->width() + 50, m_startBtn->y());
	//m_dateBtn->move(m_endBtn->x() + m_endBtn->width() + 90, m_startBtn->y());
	bw = 60;
	bh = 42;
	for (size_t i = 0; i < m_rows; i++)
	{
		m_buyBtn[i]->setFixedSize(bw, bh);
		m_returnBtn[i]->setFixedSize(bw, bh);
		int my = (m_gridH - bh) / 2;
		m_buyBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + Buy_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
		m_returnBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + bw + Buy_Space_Size + Buy_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
	}
	
	m_changeBtn->setFixedSize(45, 28);
	m_changeBtn->move(m_x + Table_Move_City_X - 50, m_y + Table_Move_City_Y);
	m_numberBtn->setFixedSize(150, 60);
	m_numberBtn->move(m_x + Table_Move_City_X, m_y + Table_Move_City_Y);
	m_numberBtn->setBackgroundColor(WHITE);
	m_numberBtn->setHoverColor(RGB(204, 213, 240));
	m_numberBtn->moveText(-40, -20);
}

void QueryTable::insertData(std::vector<Day_Flight>& data)
{
	m_datas = &data;//复制一遍vec_all，即所有数据
}

void QueryTable::readCity(const std::string& fileName)
{
	std::fstream read(fileName, ios::in);//读入文件
	if (!read.is_open())			//如果没有打开文件就返回
	{
		return;
	}
	std::string name;
	//读取城市文件
	for (size_t i = 0; i < 26; i++) {
		char data2[1024] = { 0 };
		read.getline(data2, 1024);
		stringstream ss(data2);
		ss >> citySum[i];
		if (citySum[i] == 0) continue;
		for (size_t j = 0; j < citySum[i]; j++)
		{
			ss >> name;
			city2num[name] = city2num.size();
			num2city.push_back(name);
		}
	}
}

void QueryTable::show()
{
	if (!open) open = true;
	drawTableGrid();//绘制表格
	drawTableData();//绘制数据
	if (m_maxPage >= 0)
	{
		drawButton();
	}
}

void QueryTable::drawTableGrid()//按钮和表格一起绘制
{
	//表格有八列，最少数据为： 为美观可适当调整
	//班次5字节，起飞时间5字节，城市8字节，飞行时间2字节
	//额定载量3字节，已订票3字节，是否起飞2字节

	setlinecolor(LIGHTGRAY);		//设置线颜色
	setlinestyle(PS_SOLID, 1);	//设置线

	//画横线
	for (size_t i = 0; i < m_rows + 1; i++)
	{
		line(m_x + Table_Move_X, m_y + m_gridH * i + Table_Move_Y, m_x + m_cols * m_gridW + Table_Move_X, m_y + m_gridH * i + Table_Move_Y);
	}
	//画竖线
	for (size_t i = 0; i < m_cols + 1; i++)
	{
		line(m_x + m_gridW * i + Table_Move_X, m_y + Table_Move_Y, m_x + m_gridW * i + Table_Move_X, m_y + m_gridH * m_rows + Table_Move_Y);
	}
	setlinecolor(BLACK);
}

void QueryTable::drawButton()
{
	//画选择城市按钮、选择日期按钮
	setlinecolor(LIGHTGRAY);
	::settextstyle(12, 0, "宋体");
	::settextcolor(DARKGRAY);
	if (select_swap)
	{
		m_numberBtn->show();
	}
	else
	{
		m_startBtn->show();
		m_endBtn->show();
	}
	
	//m_dateBtn->show();
	::settextstyle(22, 0, "宋体");
	setlinecolor(BLACK);
	::settextcolor(BLACK);
	int tx = 20;
	int ty = 30;
	if (select_swap)
	{
		outtextxy(m_numberBtn->x() + tx, m_numberBtn->y() + ty, number);
	}
	else
	{
		outtextxy(m_startBtn->x() + tx, m_startBtn->y() + ty, start);
		outtextxy(m_endBtn->x() + tx, m_endBtn->y() + ty, end);
	}
	
	//outtextxy(m_dateBtn->x() + tx, m_dateBtn->y() + ty, start);
	::settextstyle(20, 0, "宋体");
	m_prevBtn->show();
	m_nextBtn->show();
	m_firstBtn->show();
	m_lastBtn->show();
	::settextstyle(14, 0, "宋体");
	m_changeBtn->show();
	::settextstyle(17, 0, "宋体");
	m_prevWeek->show();
	m_nextWeek->show();
	for (size_t i = 0; i < 7; i++) {
		m_oneWeek[i]->show();
	}
	
	for (size_t i = 0; i < 6; i++)
	{
		m_sortBtn[i]->show();
	}
	::settextstyle(23, 0, "宋体");
	size_t btncnt = m_curPage == m_maxPage ? m_exrtaData : m_rows;
	for (size_t i = 0; i < btncnt; i++)
	{
		m_buyBtn[i]->show();
		m_returnBtn[i]->show();
	}
	::settextstyle(20, 0, "宋体");
	char str[30] = { 0 };
	sprintf_s(str, "第%d页 / 共%d页", m_curPage + 1, m_maxPage + 1);
	outtextxy(m_lastBtn->x() + m_lastBtn->width() + Page_Move_X, m_lastBtn->y() + (m_lastBtn->height() - m_th) / 2, str);
}

void QueryTable::drawTableData()
{
	time(&today);
	today_tm = *localtime(&today);//更新今天时间
	//m_nowday = find_today(0, m_datas.size());
	if (week_change)
	{
		refreshOneWeek();//更新一周的按钮
	}
	if (select_swap)
	{
		if (select_number && select_change)//如果用班次号，选二分法
		{
			m_result = find_result2();
			ArraySort::straightSort(m_result);
			m_result_copy = m_result;
		}
	}
	else
	{
		if (select_start && select_end)//如果选择两个地点，则显示结果
		{
			if (select_change)
			{
				m_result = find_result();
				ArraySort::straightSort(m_result);
				m_result_copy = m_result;
			}
		}
	}
	
	//在这行之前确认绘制哪一天的数据

	//防止越界
	updatePage(m_result);			//更新该天对应的页数等数据
	if (m_result.size() == 0)
	{
		if ((select_start && select_end) || select_number)
		{
			::settextstyle(20, 0, "宋体", 0, 0, 0, false, false, false);
			outtextxy(m_x, m_y + m_h + 80, "没有查询到符合条件的航班");
		}
		return;
	}
	if (m_curPage > m_maxPage) m_curPage = m_maxPage;
	if (m_curPage < 0) m_curPage = 0;

	int begPos = m_curPage * m_rows;	//数据开始的位置
	int endPos = begPos + m_rows;	//数据结束的位置
	if (m_curPage == m_maxPage) endPos = begPos + m_exrtaData;

	for (size_t i = begPos; i < endPos; i++)		//代表行
	{
		auto& line_data = m_result[i];
		auto& line_flt = m_result[i].flt;
		if (!canbuy_return(line_data))
		{//如果这个机票不能买和退票
			m_buyBtn[i - begPos]->setCant(true);
			m_returnBtn[i - begPos]->setCant(true);
		}
		else
		{
			if ((*(line_flt.begin()))->max_load <= (*(line_flt.begin()))->booked || (line_data.n == 2 && (*(line_flt.begin() + 1))->max_load <= (*(line_flt.begin() + 1))->booked))
				m_buyBtn[i - begPos]->setCant(true);
			else
				m_buyBtn[i - begPos]->setCant(false);
			if ((*(line_flt.begin()))->booked <= 0 || (line_data.n == 2 && (*(line_flt.begin() + 1))->booked <= 0))
				m_returnBtn[i - begPos]->setCant(true);
			else
				m_returnBtn[i - begPos]->setCant(false);
		}
		int tx = m_x + 40;//右移距离
		int ty = m_y + (i - begPos) * m_gridH + 20;//下移距离
		int jw = 180;
		int p1x = tx + 140, p1y = ty + 90;
		int p2y = p1y + 7;
		int p4y = p1y - 4;
		int p3x = p1x + jw;
		int p5x = p3x + 17;

		//用多边形画箭头
		POINT pts[] = { {p1x,p2y},{p1x,p1y},{p3x,p1y},{p3x,p4y},{p5x,p2y} };
		setfillcolor(RGB(102, 102, 102));
		solidpolygon(pts, 5);

		//画时间
		::settextstyle(60, 0, "微软雅黑", 0, 0, 20, false, false, false);
		outtextxy(tx + Table_Move_X, ty + Table_Move_Y, (*(line_flt.begin()))->takeoff_time.c_str());//起始时间

		//打印到达时间
		string s1, s2;//s1是辅助数组

		s1 = to_string(((*(line_flt.begin() + line_data.n - 1))->hour + (*(line_flt.begin() + line_data.n - 1))->during_time) % 24);
		if (s1.size() == 1) s2 = "0" + s1;
		else s2 = s1;
		s2 += ":";
		s1 = to_string((*(line_flt.begin() + line_data.n - 1))->minute);
		if (s1.size() == 1) s2 += "0" + s1;
		else s2 += s1;
		outtextxy(tx + Table_Move_X + jw + 168, ty + Table_Move_Y, s2.c_str());
		::settextstyle(12, 0, "宋体", 0, 0, 0, false, false, false);
		if (line_data.n == 1 && (*(line_flt.begin()))->hour + (*(line_flt.begin()))->during_time >= 24)
			outtextxy(tx + Table_Move_X + jw + 285, ty + Table_Move_Y - 5, "+1天");
		else if(line_data.n == 2 && (*(line_flt.begin() + 1))->hour + (*(line_flt.begin() + 1))->during_time >= 24)//第二个航班规定起始时间在今天
			outtextxy(tx + Table_Move_X + jw + 285, ty + Table_Move_Y - 5, "+1天");
		//画经过时间
		::settextstyle(12, 0, "宋体", 0, 0, 0, false, false, false);
		::settextcolor(DARKGRAY);
		outtextxy(tx + Table_Move_X + jw + 310, ty + Table_Move_Y + 15, "途径时间");
		::settextcolor(BLACK);
		::settextstyle(18, 0, "宋体", 0, 0, 0, false, false, false);
		uint32 dth = 0;
		uint32 dtm = 0;
		if (line_data.n == 1)
		{
			dth = (*(line_flt.begin()))->during_time;
			dtm = 0;
		}
		else
		{
			int m = ((*(line_flt.begin() + 1))->hour + (*(line_flt.begin() + 1))->during_time) * 60 + (*(line_flt.begin() + 1))->minute - ((*(line_flt.begin()))->hour * 60 + (*(line_flt.begin()))->minute);
			dth = m / 60;
			dtm = m % 60;
		}
		string dt = to_string(dth) + "小时" + to_string(dtm) + "分";
		outtextxy(tx + Table_Move_X + jw + 310, ty + Table_Move_Y + 30, dt.c_str());

		//画价格
		::settextstyle(20, 0, "微软雅黑", 0, 0, 0, false, false, false);
		int x = 680;
		int y = 25;
		outtextxy(tx + Table_Move_X + x, ty + Table_Move_Y + y, "元");
		int h = ::textheight("元");
		::settextstyle(45, 0, "微软雅黑", 0, 0, 0, false, false, false);
		uint32 pri = 0;
		for (size_t i = 0; i < line_data.n; i++)
		{
			pri += (*(line_flt.begin() + i))->price;
		}
		string price = to_string(pri);
		outtextxy(tx + Table_Move_X + x - ::textwidth(price.c_str()), ty + Table_Move_Y + y + 3 + h - ::textheight(price.c_str()), price.c_str());
		
		::settextstyle(20, 0, "微软雅黑", 0, 0, 0, false, false, false);
		if (line_data.n == 1)
		{
			outtextxy(p1x + (jw - ::textwidth("直达")) / 2, p1y - ::textheight("直达"), "直达");
			string s = "班次：";
			s += to_string((*(line_flt.begin()))->number) + " " + (*(line_flt.begin()))->start + "->" + (*(line_flt.begin()))->end;
			outtextxy(p1x + (jw - ::textwidth(s.c_str())) / 2, p1y + 7, s.c_str());
		}
		else
		{
			string s1 = "中转城市：";
			s1 += (*(line_flt.begin()))->end;
			outtextxy(p1x + (jw - ::textwidth(s1.c_str())) / 2, p1y - ::textheight(s1.c_str()), s1.c_str());
			string s2 = "班次：";
			string s3 = "班次：";
			string sd = "停留时间：";
			s2 += to_string((*(line_flt.begin()))->number) + " " + (*(line_flt.begin()))->start + "->" + (*(line_flt.begin()))->end;
			s3 += to_string((*(line_flt.begin() + 1))->number) + " " + (*(line_flt.begin() + 1))->start + "->" + (*(line_flt.begin() + 1))->end;
			int sdm = (*(line_flt.begin() + 1))->hour * 60 + (*(line_flt.begin() + 1))->minute - ((*(line_flt.begin()))->hour + (*(line_flt.begin()))->during_time) * 60 - (*(line_flt.begin()))->minute;
			int h = sdm / 60;
			int m = sdm % 60;
			sd += to_string(h) + "小时" + to_string(m) + "分";
			outtextxy(p1x + (jw - ::textwidth(s2.c_str())) / 2, p1y + 7, s2.c_str());
			::settextcolor(DARKGRAY);
			outtextxy(p1x + (jw - ::textwidth(sd.c_str())) / 2, p1y + ::textheight(sd.c_str()) / 2 + 12, sd.c_str());
			::settextcolor(BLACK);
			outtextxy(p1x + (jw - ::textwidth(s3.c_str())) / 2, p1y + ::textheight(s3.c_str()) + 17, s3.c_str());
		}
		
		::settextstyle(20, 0, "宋体", 0, 0, 0, false, false, false);
	}
	if ((select_start && select_end) || select_number)
	{
		string s = "查询到";
		s += to_string(m_result.size()) + "个符合条件的航班";
		outtextxy(m_x, m_y + m_h + 80, s.c_str());
	}
}

void QueryTable::updatePage(std::vector<Result> datas)
{
	int sz = datas.size();
	if (m_rows >= sz)
	{
		m_maxPage = 0;
		m_exrtaData = sz;
	}
	else
	{
		m_maxPage = sz / m_rows;
		m_exrtaData = sz % m_rows;
		if (m_exrtaData == 0)
		{
			m_maxPage--;
			m_exrtaData = m_rows;
		}
	}
}

void QueryTable::eventLoop(const ExMessage& msg)
{
	m_prevBtn->eventLoop(msg);
	m_nextBtn->eventLoop(msg);
	m_firstBtn->eventLoop(msg);
	m_lastBtn->eventLoop(msg);
	m_prevWeek->eventLoop(msg);
	m_nextWeek->eventLoop(msg);
	for (int i = 0; i < 7; i++) {
		m_oneWeek[i]->eventLoop(msg);
	}
	size_t btncnt = m_curPage == m_maxPage ? m_exrtaData : m_rows;
	for (size_t i = 0; i < btncnt; i++)
	{
		m_buyBtn[i]->eventLoop(msg);
		m_returnBtn[i]->eventLoop(msg);
	}
	for (size_t i = 0; i < 6; i++)
	{
		m_sortBtn[i]->eventLoop(msg);
	}
	m_changeBtn->eventLoop(msg);
	if (select_swap)
	{
		m_numberBtn->eventLoop(msg);
	}
	else
	{
		m_startBtn->eventLoop(msg);
		m_endBtn->eventLoop(msg);
	}
	
	//m_dateBtn->eventLoop(msg);
	
	if (m_prevBtn->isClicked())
	{
		if (m_curPage != 0)//当前所在页不是第一页才能往前翻
		{
			m_curPage--;
		}
	}
	if (m_nextBtn->isClicked())
	{
		if (m_curPage != m_maxPage)//当前页不是最大页才能往后翻
		{
			m_curPage++;
		}
	}
	if (m_firstBtn->isClicked())
	{
		m_curPage = 0;
	}
	if (m_lastBtn->isClicked())
	{
		m_curPage = m_maxPage;
	}
	if (m_prevWeek->isClicked())
	{
		if (m_sunday > 0)//判断是否有上一周
		{
			m_sunday -= 7;
			time_t t = show_day;
			tm t2 = *localtime(&t);
			bool flag = (m_showday - t2.tm_wday == m_sunday ? true : false);
			if (flag)	//上面日期翻页后要保证未选择的按钮不为选中颜色
			{
				m_oneWeek[select_btn]->setSelected(true);
			}
			else
			{
				m_oneWeek[select_btn]->setSelected(false);
			}
			week_change = true;
		}
	}
	if (m_nextWeek->isClicked())
	{
		if (m_sunday + 7 < (int)m_datas->size())//判断是否有下一周
		{
			m_sunday += 7;
			time_t t = show_day;
			tm t2 = *localtime(&t);
			bool flag = (m_showday - t2.tm_wday == m_sunday ? true : false);
			if (flag)	//上面日期翻页后要保证未选择的按钮不为选中颜色
			{
				m_oneWeek[select_btn]->setSelected(true);
			}
			else
			{
				m_oneWeek[select_btn]->setSelected(false);
			}
			week_change = true;
		}
	}
	for (int i = 0; i < 7; i++) {
		if (m_oneWeek[i]->isClicked())
		{
			//如果点击无数据就不切换
			if (m_sunday + i >= 0 && m_sunday + i < m_datas->size())
			{
				//改变先前选中按钮颜色
				m_oneWeek[select_btn]->setSelected(false);
				if (select_sortbtn != -1)
				{
					m_sortBtn[select_sortbtn]->setSelected(false);
					select_sortbtn = -1;
				}
				ArraySort::defSort((m_datas->begin() + m_showday)->vec_flt);
				show_day -= 86400 * (m_showday - m_sunday - i);//改变正在展示的日期
				m_showday = find_showday();
				m_curPage = 0;				//展示第一页
				select_btn = i;
				//改变选中按钮颜色
				m_oneWeek[i]->setSelected(true);
				select_change = true;
			}

		}
	}
	for (size_t i = 0; i < 6; i++)
	{
		if (m_sortBtn[i]->isClicked())
		{
			if (select_sortbtn != -1)
			{
				m_sortBtn[select_sortbtn]->setSelected(false);	//设置选中颜色
			}
			if (select_sortbtn == i)
			{
				m_sortBtn[select_sortbtn]->setSelected(false);
				m_result = m_result_copy;
				select_sortbtn = -1;
				break;
			}
			m_sortBtn[i]->setSelected(true);
			select_sortbtn = i;
			switch (i)
			{
			case 0:ArraySort::lowPriceSort(m_result);
				break;
			case 1:ArraySort::highPriceSort(m_result);
				break;
			case 2:ArraySort::lowBeginSort(m_result);
				break;
			case 3:ArraySort::highBeginSort(m_result);
				break;
			case 4:ArraySort::shortTimeSort(m_result);
				break;
			case 5:ArraySort::longTimeSort(m_result);
				break;
			default:
				break;
			}
		}
	}
	if (m_changeBtn->isClicked())
	{
		select_number = false;
		select_start = false;
		select_end = false;
		if (select_swap)
		{
			m_changeBtn->setName("地点");
			select_swap = false;
			select_change = false;
			memset(number, NULL, 1024);
			m_result.clear();
			m_result_copy.clear();
		}
		else
		{
			m_changeBtn->setName("班次号");
			select_swap = true;
			select_change = false;
			memset(start, NULL, 1024);
			memset(end, NULL, 1024);
			m_result.clear();
			m_result_copy.clear();
		}
	}
	if (select_swap)
	{
		if (m_numberBtn->isClicked())
		{
			while (InputBox(number, 1024, "请输入班次号", NULL, NULL, 0, 0, false))
			{
				bool flag = true;
				for (size_t i = 0; i < strlen(number); i++)
				{
					if (!isdigit(number[i]))
						flag = false;
				}
				if (!flag)
				{
					MessageBox(NULL, "请输入数字！", "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					continue;
				}
				if (strlen(number) > 8)
				{
					MessageBox(NULL, "数字过大！", "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					continue;
				}
				select_number = true;
				select_change = true;
				break;
			}
		}
	}
	else
	{
		if (m_startBtn->isClicked())
		{
			while (InputBox(start, 1024, "请输入出发地", NULL, NULL, 0, 0, false))
			{
				string city = start;
				if (!city2num.count(city))
				{
					string s = "城市错误或该城市没有机场，请重新输入！";
					MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					continue;
				}
				else
				{
					select_start = true;
					if (select_end == true) select_change = true;
				}
				break;
			}
		}
		if (m_endBtn->isClicked())
		{
			while (InputBox(end, 1024, "请输入目的地", NULL, NULL, 0, 0, false))
			{
				string city = end;
				if (!city2num.count(city))
				{
					string s = "城市错误或该城市没有机场，请重新输入！";
					MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					continue;
				}
				else
				{
					select_end = true;
					if (select_start == true) select_change = true;
				}
				break;
			}
		}
	}
	
	for (size_t i = 0; i < btncnt; i++)
	{
		if (m_buyBtn[i]->isClicked() && MessageBox(NULL, "您确定购票吗？", "购票", MB_OKCANCEL || MB_SYSTEMMODAL))
		{
			string msg = "已购买班次";
			std::vector<Flight*> flt = m_result[m_curPage * m_rows + i].flt;
			msg += to_string((*(flt.begin()))->number);
			if (m_result[m_curPage * m_rows + i].n == 2)
			{
				msg += "和班次";
				msg += to_string((*(flt.begin() + 1))->number);
			}
			msg += "起飞时间为：";
			msg += (*(flt.begin()))->takeoff_time;
			(*(flt.begin()))->booked++;
			if (m_result[m_curPage * m_rows + i].n == 2) (*(flt.begin() + 1))->booked++;
			MessageBox(NULL, msg.c_str(), "购买成功", MB_OK || MB_SYSTEMMODAL);
		}
		if (m_returnBtn[i]->isClicked() && MessageBox(NULL, "您确定退票吗？", "退票", MB_OKCANCEL || MB_SYSTEMMODAL))
		{
			string msg = "班次";
			std::vector<Flight*> flt = m_result[m_curPage * m_rows + i].flt;
			msg += to_string((*(flt.begin()))->number);
			if (m_result[m_curPage * m_rows + i].n == 2)
			{
				msg += "和班次";
				msg += to_string((*(flt.begin() + 1))->number);
			}
			msg += "已退票";
			msg += (*(flt.begin()))->takeoff_time;
			(*(flt.begin()))->booked--;

			if (m_result[m_curPage * m_rows + i].n == 2) (*(flt.begin() + 1))->booked--;
			MessageBox(NULL, msg.c_str(), "退票成功", MB_OK || MB_SYSTEMMODAL);
		}
	}
	/*if (m_dateBtn->isClicked())
	{
		char d[1024] = { 0 };
		while (InputBox(d, 1024, "请输入出发时间", NULL, NULL, 0, 0, false))
		{

		}
	}*/
}

bool QueryTable::canbuy_return(Result data)
{
	tm daytm = today_tm;
	daytm.tm_year = data.year - 1900;
	daytm.tm_mon = data.month - 1;
	daytm.tm_mday = data.day;
	daytm.tm_hour = (*(data.flt.begin()))->hour;
	daytm.tm_min = (*(data.flt.begin()))->minute;
	time_t day = mktime(&daytm);
	//if (day - today < 864000 && day - today > 3600)//起飞十天内，一小时前可以购票和退票
	if (day - today > 3600)//起飞一小时前可订票退票，可无限期预定票（可改）
	{
		return true;
	}
	return false;
}

void QueryTable::setToday()
{
	time_t day = today;
	tm t = *localtime(&day);
	select_btn = t.tm_wday;					//默认今天的按钮被选中
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	m_today = ((int)difftime(today, day)) / 86400;//设置m_today
	m_showday = m_today;				//设置m_showday
	m_sunday = m_showday - select_btn;		//当前七个按钮展示数据对应的周日所对应的下标
	m_oneWeek[select_btn]->setSelected(true);//默认打开今天
}

int QueryTable::find_showday()
{
	time_t day = show_day;
	tm t = *localtime(&day);
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	return ((int)difftime(show_day, day)) / 86400;
}

std::vector<Result> QueryTable::find_result()//认为日期连续（即中间不存在没有航班的日期）
{
	time_t day = show_day;
	tm t = *localtime(&day);
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	size_t date = ((size_t)difftime(show_day, day)) / 86400;//这个是目标日期的下标
	m_showday = date;

	Map mp_day;//建立当天有向图邻接表
	mp_day.year = (m_datas->begin() + m_showday)->year;
	mp_day.month = (m_datas->begin() + m_showday)->month;
	mp_day.day = (m_datas->begin() + m_showday)->day;
	std::vector<Flight>* vflt = &((m_datas->begin() + m_showday)->vec_flt);
	for (size_t j = 0; j < vflt->size(); j++)
	{
		Flight* flt = &(*(vflt->begin() + j));
		mp_day.flt[city2num[flt->start]].push_back(flt);
	}
	
	std::vector<Result> flt;	//返回的结果集
	std::string st = start;		//起始地的名称
	size_t stn = city2num[start];	//起始地的编号
	std::string ed = end;
	size_t edn = city2num[end];

	std::vector<Flight*> supvec;	//用于暂存进辅栈的航班数组
	std::vector<Flight*> route;	//用于暂存路线的航班数组
	//mainStk栈内元素超过3个，即中转两次以上不作考虑
	mainStk.push(start);
	for (auto& val : mp_day.flt[stn])
	{
		supvec.push_back(val);
	}
	supStk.push(supvec);

	//用双栈获取一点到另一点的所有路径（1<=路径数<=3）
	while (!mainStk.empty())
	{
		supvec = supStk.top(); supStk.pop();
		if (mainStk.size() >= 3 || supvec.empty())//倒达这里却有四个结点说明第四个不是终点，直接削栈
		{
			mainStk.pop();
			if(!route.empty()) route.pop_back();
			continue;
		}
		else
		{
			Flight* f = *(supvec.begin());	//当前使用航班
			if (route.size() == 1 && f->end != ed)//只考虑中转一次
			{
				supvec.erase(supvec.begin());
				supStk.push(supvec);
				continue;
			}
			if (route.size() == 0 && f->end == ed)
			{//直达
				route.push_back(supvec[0]);
				Result t;
				t.flt = route;
				t.n = route.size();
				t.year = (m_datas->begin() + m_showday)->year;
				t.month = (m_datas->begin() + m_showday)->month;
				t.day = (m_datas->begin() + m_showday)->day;
				flt.push_back(t);
				supvec.erase(supvec.begin());
				supStk.push(supvec);
				route.pop_back();
				continue;
			}
			if (route.size() == 1 && f->end == ed)//如果成功获取一条路径
			{	
				//已知两个航班的起始时间都是今天
				int mdif = (int)(f->hour * 60 + f->minute) - (int)(((*(route.begin()))->hour + (*(route.begin()))->during_time) * 60 + (*(route.begin()))->minute);
				if (route.size() == 1 && (mdif < 10 || mdif > 12 * 60))//中转时间不能小于10分钟或大于12小时
				{
					supvec.erase(supvec.begin());
					supStk.push(supvec);
					continue;
				}
				route.push_back(supvec[0]);
				Result t;
				t.flt = route;
				t.n = route.size();
				t.year = (m_datas->begin() + m_showday)->year;
				t.month = (m_datas->begin() + m_showday)->month;
				t.day = (m_datas->begin() + m_showday)->day;
				flt.push_back(t);
				supvec.erase(supvec.begin());
				supStk.push(supvec);
				route.pop_back();
				continue;
			}
			if (f->hour + f->during_time < 24)//如果第一条路不到终点并且没超过第一天，不考虑第二天
			{//那么这可以作为第一个结点
				mainStk.push(f->end);
				route.push_back(supvec[0]);
				supvec.erase(supvec.begin());
				supStk.push(supvec);
				supStk.push(mp_day.flt[city2num[f->end]]);//把新节点的路径压入辅栈
				continue;
			}
			//如果剩下的这个点不符合任何要求
			supvec.erase(supvec.begin());
			supStk.push(supvec);
		}
	}
	select_change = false;
	return flt;
}

std::vector<Result> QueryTable::find_result2()
{
	select_change = false;
	select_number = true;
	std::vector<Result> flt;	//返回的结果集
	ArraySort::defSort((m_datas->begin() + m_showday)->vec_flt);//保证元素有序
	uint32 x = atoi(number);
	int num = dich(0, (m_datas->begin() + m_showday)->vec_flt.size() - 1, x);
	if (num == -1) return flt;
	Result r;
	r.year = (m_datas->begin() + m_showday)->year;
	r.month = (m_datas->begin() + m_showday)->month;
	r.day = (m_datas->begin() + m_showday)->day;
	r.n = 1;
	r.flt.push_back(&(m_datas->begin() + m_showday)->vec_flt[num]);
	flt.push_back(r);
	return flt;
}

int QueryTable::dich(int l, int r, int x)
{
	while (l <= r)
	{
		int mid = l + r >> 1;
		if ((m_datas->begin() + m_showday)->vec_flt[mid].number == x)
			return mid;
		else if ((m_datas->begin() + m_showday)->vec_flt[mid].number < x)
			l = mid + 1;
		else
			r = mid - 1;
	}
	return -1;
}

void QueryTable::refreshOneWeek()
{
	//m_oneWeek[wtoday];//这是当前展示的表格对应按钮
	for (int i = m_sunday; i < m_sunday + 7; i++) {
		std::string s = "";
		if (i < 0 || i >= m_datas->size())
		{
			s += "无数据";
		}
		else
		{
			s += std::to_string((m_datas->begin() + i)->month);
			s += "月";
			s += std::to_string((m_datas->begin() + i)->day);
			s += "日 ";
			if (i != m_today) s += Week_Str[i - m_sunday];
			else s += "今天";
		}
		m_oneWeek[i - m_sunday]->setName(s);
	}
	week_change = false;
}

void QueryTable::refreshTable()
{
	m_result = m_result_copy;
	m_oneWeek[select_btn]->setSelected(false);
	if (select_sortbtn != -1)
	{
		m_sortBtn[select_sortbtn]->setSelected(false);
		select_btn = -1;
	}
	show_day = today;
	today_tm = *localtime(&today);
	setToday();
	week_change = true;
	select_start = false;
	select_end = false;
	select_change = false;
	select_swap = false;
	select_number = false;
	m_changeBtn->setName("地点");
	memset(number, NULL, 1024);
	memset(start, NULL, 1024);
	memset(end, NULL, 1024);
	m_result.clear();
	m_result_copy.clear();
}

