#include "Table.h"
#include <iostream>
const int Table_Move_X = -30;	//对表格的左右移动
const int Table_Move_Y = 80;//对表格的上下移动
const int Table_Move_Btn9_X = 0;//上面九个按钮的左右移动
const int Table_Move_Btn9_Y = 0;//上面九个按钮的上下移动
const int Table_Move_Btn4_X = 380;//下面四个按钮的左右移动
const int Table_Move_Btn4_Y = 100;//下面四个按钮的上下移动
const int Page_Move_X = 10;			//页码数左右移动
const int Edit_Move_X = 10;			//编辑、删除键左右移动
const int Edit_Space_Size = 10;		//编辑键和删除键的间隔距离
const int Sort_Move_X = 350;			//排序键的左右移动
const int Sort_Move_Y = -66;			//排序键的上下移动
Table::Table(int row, int col)
	:BasicWidget(0,0,0,0)	//初始化父类数据
	,m_rows(row),m_cols(col)//初始化行列数
	,m_curPage(0),m_maxPage(0),m_exrtaData(0)//初始化页相关数据
{
	readCity("./images/city.txt");			//读入城市文件
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

	//未加载表格，会报错 refreshShowday_OneWeek();//更新现在展示的时间和一周的按钮

	m_sortBtn.push_back(new PushButton("低价优先"));
	m_sortBtn.push_back(new PushButton("高价优先"));
	m_sortBtn.push_back(new PushButton("起飞早-晚"));
	m_sortBtn.push_back(new PushButton("起飞晚-早"));
	m_sortBtn.push_back(new PushButton("时间短-长"));
	m_sortBtn.push_back(new PushButton("时间长-短"));
	m_numberBtn = new PushButton("按班次号查询");
}

Table::~Table()//防止内存泄漏
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

void Table::setRowCount(int row)
{
	m_rows = row;
}

void Table::setColCount(int col)
{
	m_cols = col;
}

void Table::setHeader(const std::string& header)
{
	m_header = split(header, '\t');
	vec_header = split(header, '\t');
	m_cols = std::count(header.begin(), header.end(), '\t') + 1;

	//设置文字宽度和高度，单位为像素
	m_tw = ::textwidth("乌鲁木齐");
	m_th = ::textheight(header.c_str());

	//设置格子宽度和高度，单位为像素
	m_gridW = m_tw + 10;//有飞机的城市最多有四个字（国内）
	m_gridH = m_th + 10;//用表头规定高度

	//更新表格宽度和高度
	m_w = m_gridW * m_cols;
	m_h = m_gridH * m_rows;
}

void Table::setButton(int flag)
{
	if (flag == 0) 
	{
		for (size_t i = 0; i < m_rows; i++)
		{
			m_editBtn.push_back(new PushButton("编辑"));
			m_deleteBtn.push_back(new PushButton("删除"));
		}
	}
	if (flag == 1)
	{
		for (size_t i = 0; i < m_rows; i++)
		{
			m_editBtn.push_back(new PushButton("购票"));
			m_deleteBtn.push_back(new PushButton("退票"));
		}
	}
	for (size_t i = 0; i < m_rows; i++)	//每个格子都是一个编辑按钮
	{
		std::vector<PushButton*> btns;
		for (size_t j = 0; j < m_cols; j++)
		{
			btns.push_back(new PushButton("edit"));
		}
		m_editAllBtn.push_back(btns);
	}
	m_takeoffBtn = new PushButton("只查看未起飞");
	m_insertBtn = new PushButton("录入航班");
	for (size_t i = 0; i < m_rows + 1; i++)
	{
		m_insertRowBtn.push_back(new PushButton("录入->"));
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

	for (size_t i = 0; i < m_rows; i++)//编辑和删除按钮设置
	{
		int tw = 38;	//编辑和删除按钮大小
		int th = 22;
		m_editBtn[i]->setFixedSize(tw, th);
		m_deleteBtn[i]->setFixedSize(tw, th);
		int my = (m_gridH - th) / 2;//居中按钮
		m_editBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + Edit_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
		m_deleteBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + tw + Edit_Space_Size + Edit_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
	}

	for (size_t i = 0; i < 6; i++)//排序按钮设置
	{
		int tw = 90;	//排序按钮大小
		int th = 26;
		m_sortBtn[i]->setFixedSize(tw, th);
		m_sortBtn[i]->move(m_x + Table_Move_X + Sort_Move_X + tw * i, m_y + Table_Move_Y + Sort_Move_Y);
	}
	for (size_t i = 0; i < m_rows; i++)//每个编辑按钮设置
	{
		for (size_t j = 0; j < m_cols; j++)
		{
			select_edit.push_back(false);
			m_editAllBtn[i][j]->setFixedSize(m_gridW, m_gridH);
			m_editAllBtn[i][j]->move(m_x + Table_Move_X + m_gridW * j, m_y + Table_Move_Y + m_gridH * i);
		}
	}

	m_takeoffBtn->setFixedSize(100, 26);
	m_takeoffBtn->move(m_x + Table_Move_X + Sort_Move_X - 130, m_y + Table_Move_Y + Sort_Move_Y);
	m_insertBtn->setFixedSize(120, 50);
	m_insertBtn->move(m_x - 82, m_y + m_h + 93);
	for (size_t i = 0; i < m_rows + 1; i++)
	{
		m_insertRowBtn[i]->setFixedSize(50, 20);
		m_insertRowBtn[i]->move(m_x - 82, m_y + m_gridH * i + Table_Move_Y - 10);
	}
	m_numberBtn->setFixedSize(120, 50);
	m_numberBtn->move(m_x + 140, m_y + m_h + 93);
	m_numberBtn->setBackgroundColor(WHITE);
	m_numberBtn->setHoverColor(RGB(204, 213, 240));
	m_numberBtn->moveText(-20, -15);
}

void Table::insertData(std::vector<Day_Flight>& data)
{
	m_datas = &data;//复制一遍vec_all，即所有数据
}

void Table::readCity(const std::string& fileName)
{
	std::fstream read(fileName, ios::in);//读入文件
	if (!read.is_open())			//如果没有打开文件就返回
	{
		return;
	}
	std::string name;
	for (int i = 0; i < 26; i++) {
		char data2[1024] = { 0 };
		read.getline(data2, 1024);
		stringstream ss(data2);
		ss >> citySum[i];
		if (citySum[i] == 0) continue;
		for (size_t j = 0; j < citySum[i]; j++)
		{
			ss >> name;
			mp_city[name] = mp_city.size();
			m_city.push_back(name);
		}
	}
}

void Table::show()
{
	if (!open) open = true;
	drawHeader();	//绘制表头
	drawTableGrid();//绘制表格
	drawTableData();//绘制数据
	if (m_maxPage >= 0)
	{
		drawButton();
	}
}

void Table::drawTableGrid()//按钮和表格一起绘制
{
	//表格有八列，最少数据为： 为美观可适当调整
	//班次5字节，起飞时间5字节，城市8字节，飞行时间2字节
	//额定载量3字节，已订票3字节，是否起飞2字节

	setlinecolor(BLACK);		//设置线颜色
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
}

void Table::drawButton()
{
	m_prevBtn->show();
	m_nextBtn->show();
	m_firstBtn->show();
	m_lastBtn->show();
	::settextstyle(17, 0, "宋体");
	m_prevWeek->show();
	m_nextWeek->show();
	for (size_t i = 0; i < 7; i++) {
		m_oneWeek[i]->show();
	}
	size_t btncnt = m_curPage == m_maxPage ? m_exrtaData : m_rows;
	for (size_t i = 0; i < btncnt; i++)
	{
		m_editBtn[i]->show();
		m_deleteBtn[i]->show();
	}
	for (size_t i = 0; i < 6; i++)
	{
		m_sortBtn[i]->show();
	}
	::settextstyle(24, 0, "宋体");
	if(!select_number) m_insertBtn->show();

	setlinecolor(LIGHTGRAY);
	::settextstyle(12, 0, "宋体");
	::settextcolor(DARKGRAY);
	m_numberBtn->show();
	::settextstyle(22, 0, "宋体");
	setlinecolor(BLACK);
	::settextcolor(BLACK);
	int tx = 20;
	int ty = 20;
	outtextxy(m_numberBtn->x() + tx, m_numberBtn->y() + ty, number);

	::settextstyle(15, 0, "宋体");
	//m_takeoffBtn->show();
	if (!select_number && select_insert)
	{
		for (size_t i = 0; i < btncnt + 1; i++)
		{
			m_insertRowBtn[i]->show();
		}
	}
	
	::settextstyle(20, 0, "宋体");
	for (size_t i = 0; i < m_rows; i++)
	{
		if (select_edit[i])
		{
			for (size_t j = 0; j < m_cols; j++)
			{
				m_editAllBtn[i][j]->show();
			}
		}
	}
	
	char str[30] = { 0 };
	sprintf_s(str, "第%d页 / 共%d页", m_curPage + 1, m_maxPage + 1);
	outtextxy(m_lastBtn->x() + m_lastBtn->width() + Page_Move_X, m_lastBtn->y() + (m_lastBtn->height() - m_th) / 2, str);
}

void Table::drawTableData()
{
	//m_nowday = find_today(0, m_datas.size());
	if (week_change)
	{
		refreshOneWeek();//更新一周的按钮
	}
	//(m_datas->begin() + m_showday)->vec_flt; 是当前展示日期的数组

	//如果没点筛选就按数组更新页数
	//if(!select_takeoff) 
	if (select_number)
	{
		int num = dich(0, (m_datas->begin() + m_showday)->vec_flt.size() - 1, atoi(number));
		if (num == -1)
		{
			outtextxy(m_x + Table_Move_X + 10, m_y + 20, "没有找到该航班");
			m_curPage = 0;
			m_maxPage = 0;
			m_exrtaData = 0;
			return;
		}
		else
		{
			output_data.clear();
			output_data.push_back(split((m_datas->begin() + m_showday)->vec_flt[num].formatInfo(), '\t'));
			for (size_t k = 0; k < m_cols; k++)//代表列
			{
				int tx = m_x + k * m_gridW + (m_gridW - ::textwidth(output_data[0][k].c_str())) / 2;//右移距离
				int ty = m_y + 5;//下移距离
				outtextxy(tx + Table_Move_X, ty + Table_Move_Y, output_data[0][k].c_str());
			}
		}
		m_curPage = 0;
		m_maxPage = 0;
		m_exrtaData = 1;
		return;
	}
	updatePage();			//更新该天对应的页数等数据
	if ((m_datas->begin() + m_showday)->vec_flt.size() == 0) return;
	if (m_curPage > m_maxPage) m_curPage = m_maxPage;
	if (m_curPage < 0) m_curPage = 0;
	int begPos = m_curPage * m_rows;	//数据开始的位置
	int endPos = begPos + m_rows;	//数据结束的位置
	if (m_curPage == m_maxPage) endPos = begPos + m_exrtaData;
	output_data.clear();
	/*if (select_takeoff)
	{
		time(&today);
		today_tm = *localtime(&today);//更新今天时间
		if ((m_datas->begin() + m_showday)->year == today_tm.tm_year + 1900 && (m_datas->begin() + m_showday)->month == today_tm.tm_mon + 1 && (m_datas->begin() + m_showday)->day == today_tm.tm_mday)
		{
			for (size_t i = begPos, j = 0; i < (m_datas->begin() + m_showday)->vec_flt.size() && j < m_rows; i++)
			{
				if ((m_datas->begin() + m_showday)->vec_flt[i].hour * 60 + (m_datas->begin() + m_showday)->vec_flt[i].minute > today_tm.tm_hour * 60 + today_tm.tm_min)
				{
					j++;
					(m_datas->begin() + m_showday)->vec_flt[i].take_off = "否";
					(m_datas->begin() + m_showday)->vec_flt[i].take_off_bool = false;
					output_data.push_back(split((m_datas->begin() + m_showday)->vec_flt[i].formatInfo(), '\t'));
				}
			}
		}
		//主动更新页数信息
		int sz = output_data[m_showday].size();
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
		if (m_curPage > m_maxPage)
			m_curPage = m_maxPage;
		if (m_maxPage == 0)
			endPos = m_exrtaData;
	}*/
	//else if (!select_takeoff)
	//{
	for (size_t i = begPos; i < endPos; i++)
	{
		output_data.push_back(split((m_datas->begin() + m_showday)->vec_flt[i].formatInfo(), '\t'));
	}
	//}
	
	for (size_t i = begPos; i < endPos; i++)		//代表行
	{
		for (size_t k = 0; k < m_cols; k++)//代表列
		{
			int tx = m_x + k * m_gridW + (m_gridW - ::textwidth(output_data[i - begPos][k].c_str())) / 2;//右移距离
			int ty = m_y + (i - begPos) * m_gridH + 5;//下移距离
			outtextxy(tx + Table_Move_X, ty + Table_Move_Y, output_data[i - begPos][k].c_str());
		}
	}
	string s = "当日航班数：";
	s += to_string((m_datas->begin() + m_showday)->n);
	::settextstyle(22, 0, "宋体");
	outtextxy(m_x + Table_Move_X + 10, m_y + 20, s.c_str());
	::settextstyle(20, 0, "宋体");
}

void Table::drawHeader()
{
	//绘制矩形
	setlinestyle(PS_SOLID, 2);
	int headH = 30;
	::rectangle(m_x + Table_Move_X, m_y + Table_Move_Y - headH, m_x + m_w + Table_Move_X, m_y + Table_Move_Y);
	for (size_t i = 1; i < m_cols; i++)
	{
		line(m_x + i * m_gridW + Table_Move_X, m_y + Table_Move_Y - headH, m_x + i * m_gridW + Table_Move_X, m_y + Table_Move_Y);
	}

	for (size_t i = 0; i < m_header.size(); i++)
	{
		int tx = m_x + i * m_gridW + Table_Move_X + (m_gridW - ::textwidth(m_header[i].c_str())) / 2;
		int ty = m_y + Table_Move_Y - headH + (m_gridH - ::textheight(m_header[i].c_str())) / 2;
		outtextxy(tx, ty, m_header[i].c_str());
	}
}

int Table::dich(int l, int r, int x)
{
	while (l <= r)
	{
		int mid = l + r >> 1;
		if ((m_datas->begin() + m_showday)->vec_flt[mid].number == x) return mid;
		if ((m_datas->begin() + m_showday)->vec_flt[mid].number < x) l = mid + 1;
		else r = mid - 1;
	}
	return -1;
}

std::vector<std::string> Table::split(std::string str, char separator)
{
	std::vector<std::string> res;

	for (size_t pos = 0; pos != std::string::npos;)//npos是size_t能表示的最大值
	{
		pos = str.find(separator);//找到指定分割地址的位置

		res.push_back(str.substr(0, pos));//插入指定字符串
		str = std::string(str.c_str() + pos + 1);//重建字符串，第二个重载，用c风格的字符串地址来定义
	}
	return res;
}

void Table::updatePage()
{
	int sz = (m_datas->begin() + m_showday)->vec_flt.size();
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

void Table::eventLoop(const ExMessage& msg)
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
		m_editBtn[i]->eventLoop(msg);
		m_deleteBtn[i]->eventLoop(msg);
	}
	for (size_t i = 0; i < 6; i++)
	{
		m_sortBtn[i]->eventLoop(msg);
	}

	for (size_t i = 0; i < m_rows; i++)
	{
		if (select_edit[i])
		{
			for (size_t j = 0; j < m_cols; j++)
			{
				m_editAllBtn[i][j]->eventLoop(msg);
			}
		}
	}
	if (!select_number) m_insertBtn->eventLoop(msg);
	m_numberBtn->eventLoop(msg);
	//m_takeoffBtn->eventLoop(msg);
	if (!select_number && select_insert)
	{
		for (size_t i = 0; i < btncnt + 1; i++)
		{
			m_insertRowBtn[i]->eventLoop(msg);
		}
	}

	if (m_prevBtn->isClicked())
	{
		if (m_curPage != 0)//当前所在页不是第一页才能往前翻
		{
			m_curPage--;
			refreshAfterClicked();
		}
	}
	if (m_nextBtn->isClicked())
	{
		if (m_curPage != m_maxPage)//当前页不是最大页才能往后翻
		{
			m_curPage++;
			refreshAfterClicked();
		}
	}
	if (m_firstBtn->isClicked())
	{
		m_curPage = 0;
		refreshAfterClicked();
	}
	if (m_lastBtn->isClicked())
	{
		m_curPage = m_maxPage;
		refreshAfterClicked();
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
				refreshAfterClicked();
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
			}
			
		}
	}
	for (size_t i = 0; i < btncnt; i++) {
		if (m_editBtn[i]->isClicked())
		{
			if (select_edit[i]) 
			{
				select_edit[i] = false;
				m_editBtn[i]->setSelected(false);
				m_editBtn[i]->setName("编辑");
			}
			else
			{
				select_edit[i] = true;
				m_editBtn[i]->setSelected(true);
				m_editBtn[i]->setName("停止");
				Flight flt = (m_datas->begin() + m_showday)->vec_flt[m_curPage * m_rows + i];
				m_editAllBtn[i][0]->setName(to_string(flt.number));
				m_editAllBtn[i][1]->setName(flt.takeoff_time);
				m_editAllBtn[i][2]->setName(flt.start);
				m_editAllBtn[i][3]->setName(flt.end);
				m_editAllBtn[i][4]->setName(to_string(flt.during_time));
				m_editAllBtn[i][5]->setName(to_string(flt.max_load));
				m_editAllBtn[i][6]->setName(to_string(flt.booked));
				m_editAllBtn[i][7]->setName(flt.take_off);
				m_editAllBtn[i][8]->setName(to_string(flt.price));
			}
		}
		if (m_deleteBtn[i]->isClicked())
		{
			delete_data(i);
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
				ArraySort::defSort((m_datas->begin() + m_showday)->vec_flt);
				select_sortbtn = -1;
				break;
			}
			m_sortBtn[i]->setSelected(true);
			select_sortbtn = i;
			switch (i)
			{
			case 0:ArraySort::lowPriceSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			case 1:ArraySort::highPriceSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			case 2:ArraySort::lowBeginSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			case 3:ArraySort::highBeginSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			case 4:ArraySort::shortTimeSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			case 5:ArraySort::longTimeSort((m_datas->begin() + m_showday)->vec_flt);
				break;
			default:
				break;
			}
			refreshAfterClicked();
		}
	}

	if (!select_number && m_insertBtn->isClicked())
	{
		if (select_insert)
		{
			select_insert = false;
			m_insertBtn->setSelected(false);
			m_insertBtn->setName("录入航班");
		}
		else
		{
			select_insert = true;
			m_insertBtn->setSelected(true);
			m_insertBtn->setName("停止录入");
		}
	}
	/*if (m_takeoffBtn->isClicked())
	{
		if (select_takeoff)
		{
			select_takeoff = false;
			m_takeoffBtn->setSelected(false);
		}
		else
		{
			select_takeoff = true;
			m_takeoffBtn->setSelected(true);
		}
	}*/
	if (m_numberBtn->isClicked())
	{
		if (select_number)
		{
			select_number = false;
			memset(number, NULL, 1024);
		}
		else 
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
				break;
			}
		}
		
	}
	if (!select_number && select_insert)
	{
		for (size_t i = 0; i < btncnt + 1; i++)
		{
			if (m_insertRowBtn[i]->isClicked())
			{
				char s[1024] = { 0 };
				while (InputBox(s, 1024, "请按“班次 起飞时间 起点站 终点站 飞行时间 额定载量 已订票 是否起飞 价格”的顺序输入\n输入例：“123 06:30 青岛 济南 2 120 100 是 3164”", "请输入航班信息", NULL, 0, 0, false))
				{
					stringstream ss(s);
					int num, drtm, mx, bk, pri;
					string tktm, st, ed, tk;
					try {
						ss >> num >> tktm >> st >> ed >> drtm >> mx >> bk >> tk >> pri;
						if (num < 0)
						{
							MessageBox(NULL, "班次号不能为负数！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (num > 1e7)
						{
							MessageBox(NULL, "您输入的班次号过大！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (tktm.size() != 5 || !isdigit(tktm[0]) || !isdigit(tktm[1]) || tktm[2] != ':' || !isdigit(tktm[3]) || !isdigit(tktm[4]))
						{
							MessageBox(NULL, "起飞时间格式有误！(有前缀0)", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						string s = tktm;
						int h = atoi(s.substr(0, 2).c_str());
						int m = atoi(s.substr(3).c_str());
						if (h < 0 || h > 23)
						{
							MessageBox(NULL, "小时数范围为0-23！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (m < 0 || m > 59)
						{
							MessageBox(NULL, "分钟数范围为0-59！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (!mp_city.count(st))
						{
							MessageBox(NULL, "起点站不存在或该城市无机场！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (!mp_city.count(ed))
						{
							MessageBox(NULL, "终点站不存在或该城市无机场！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (drtm < 0)
						{
							MessageBox(NULL, "飞行时间不能为负数！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (drtm > 24)
						{
							MessageBox(NULL, "飞行时间并不合理！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (mx < 0)
						{
							MessageBox(NULL, "额定载量不能为负数！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (bk < 0)
						{
							MessageBox(NULL, "已订票数不能为负数！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (bk > mx)
						{
							MessageBox(NULL, "已订票数不能大于额定载量！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (tk != "是" && tk != "否")
						{
							MessageBox(NULL, "是否起飞请输入“是”或“否”", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (pri < 0)
						{
							MessageBox(NULL, "价格不能为负数！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						Flight flt(num, tktm, st, ed, drtm, mx, bk, tk, pri);
						(m_datas->begin() + m_showday)->vec_flt.insert(((m_datas->begin() + m_showday)->vec_flt.begin() + m_curPage * m_rows + i), flt);
						(m_datas->begin() + m_showday)->n++;
						updatePage();
						
						refreshAfterClicked();
						break;
					}
					catch (exception e) {
						MessageBox(NULL, "您输入的格式有误！", "请重新输入", MB_OK | MB_SYSTEMMODAL);
						break;
					}
				}
			}
			if (i == m_rows + 1)
			{
				m_curPage++;
			}
		}
	}
	for (size_t i = 0; i < m_rows; i++)
	{
		if (select_edit[i])
		{
			for (size_t j = 0; j < m_cols; j++)
			{
				if (m_editAllBtn[i][j]->isClicked())
				{
					char title[20] = "修改";
					char remind[20] = "原数值为：";
					strcat(title, vec_header[j].c_str());
					strcat(remind, m_editAllBtn[i][j]->getName().c_str());
					char s[1024] = { 0 };
					Flight& flt = (m_datas->begin() + m_showday)->vec_flt[m_curPage * m_rows + i];
					while (InputBox(s, 1024, remind, title, NULL, 0, 0, false))
					{
						if (j == 0 || j == 4 || j == 5 || j == 6 || j == 8)
						{
							bool flag = true;
							for (size_t k = 0; k < strlen(s); k++)
							{
								if (s[k] < 0 || s[k] > 127)
								{
									flag = false;
									break;
								}
								if (!isdigit(s[k]))
								{
									flag = false;
									break;
								}
							}
							if (!flag)
							{
								string s = vec_header[j];
								s += " 为整数，请重新输入！";
								MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
								continue;
							}
							else if (j == 5 && atoi(s) < flt.booked)
							{
								MessageBox(NULL, "额定载量不能低于订票数！", "输入数据错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
								continue;
							}
							else if (j == 6 && atoi(s) > flt.max_load)
							{
								MessageBox(NULL, "订票数不能高于额定载量！", "输入数据错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
								continue;
							}
							else
							{
								switch (j)
								{
								case 0:flt.number = atoi(s);
									m_editAllBtn[i][0]->setName(to_string(flt.number));
									break;
								case 4:flt.during_time = atoi(s);
									m_editAllBtn[i][4]->setName(to_string(flt.during_time));
									break;
								case 5:flt.max_load = atoi(s);
									m_editAllBtn[i][5]->setName(to_string(flt.max_load));
									break;
								case 6:flt.booked = atoi(s);
									m_editAllBtn[i][6]->setName(to_string(flt.booked));
									break;
								case 8:flt.price = atoi(s);
									m_editAllBtn[i][8]->setName(to_string(flt.price));
									break;
								default:
									break;
								}
								break;
							}					
						}
						else//要输入字符串
						{
							if (j == 1)//输入时间
							{
								size_t k = 0;
								if (strlen(s) == 5 && isdigit(s[0]) && isdigit(s[1]) && s[2] == ':' && isdigit(s[3]) && isdigit(s[4]))
								{
									flt.takeoff_time = s;
									string ss(s);
									int hour = atoi(ss.substr(0, 2).c_str());
									int minute = atoi(ss.substr(3).c_str());
									flt.hour = hour;
									flt.minute = minute;
									m_editAllBtn[i][j]->setName(flt.takeoff_time);
									break;
								}
								else
								{
									string s = vec_header[j];
									s += " 格式为xx:xx(有前缀0)，请重新输入！";
									MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
									continue;
								}
							}
							else if (j == 2 || j == 3)//输入城市
							{
								string city = s;
								if (!mp_city.count(city))
								{
									string s = vec_header[j];
									s += " 城市错误或该城市没有机场，请重新输入！";
									MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
									continue;
								}
								else
								{
									if (j == 2)
									{
										flt.start = city;
										m_editAllBtn[i][j]->setName(flt.start);
									}
									else
									{
										flt.end = city;
										m_editAllBtn[i][j]->setName(flt.end);
									}
									break;
								}
							}
							else
							{
								string flag = s;
								if (flag == "是" || flag == "否")
								{
									flt.take_off = flag;
									flt.take_off_bool =  flag == "是" ? true : false;
									m_editAllBtn[i][j]->setName(flt.take_off);
									break;
								}
								else
								{
									string s = vec_header[j];
									s += " 请输入“是”或“否”，请重新输入！";
									MessageBox(NULL, s.c_str(), "输入格式错误", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
									continue;
								}
							}
						}
					}
					return;
				}
			}
		}
	}
	//用编辑按钮为结尾，再加按钮加在上面
}

//先提示用户是否确定删除，再删除整行
//可以考虑加个设置选项，取消删除提醒
void Table::delete_data(size_t num)
{
	if (MessageBox(NULL, "您确定删除该航班吗？", "删除航班", MB_OKCANCEL | MB_SYSTEMMODAL) != IDOK)//可以考虑把航班信息打印一份
	{
		return;
	}
	Day_Flight& flt = *(m_datas->begin() + m_showday);
	flt.vec_flt.erase(flt.vec_flt.begin() + m_curPage * m_rows + num);//删除对应行
	flt.n--;						//减少当天航班数
	updatePage();		//刷新当前展示表格的m_extraPage等数据
	refreshAfterClicked();
}

void Table::setToday()
{
	time_t day = today;
	tm t = *localtime(&day);
	select_btn = t.tm_wday;					//默认今天的按钮被选中
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	m_today = ((int)difftime(today, day)) / 86400;//设置m_today
	m_showday = find_showday();				//设置m_showday
	m_sunday = m_showday - select_btn;		//当前七个按钮展示数据对应的周日所对应的下标
	m_oneWeek[select_btn]->setSelected(true);//默认打开今天
	updatePage();
	m_curPage = 0;
}

int Table::find_showday()//认为日期连续（即中间不存在没有航班的日期）
{
	time_t day = show_day;
	tm t = *localtime(&day);
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	return ((int)difftime(show_day, day)) / 86400;
}

void Table::refreshOneWeek()
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

void Table::refreshTable()
{
	ArraySort::defSort((m_datas->begin() + m_showday)->vec_flt);
	m_oneWeek[select_btn]->setSelected(false);
	if(select_sortbtn != -1) 
	{
		m_sortBtn[select_sortbtn]->setSelected(false);
		select_btn = -1;
	}
	for (size_t i = 0; i < m_rows; i++)
	{
		if (select_edit[i])
		{
			select_edit[i] = false;
			m_editBtn[i]->setSelected(false);
			m_editBtn[i]->setName("编辑");
		}
	}
	show_day = today;
	today_tm = *localtime(&today);
	setToday();
	week_change = true;
	//select_takeoff = false;
	m_takeoffBtn->setSelected(false);
	select_insert = false;
	select_number = false;
	memset(number, NULL, 1024);
	m_insertBtn->setSelected(false);
	m_insertBtn->setName("录入航班");
}

void Table::refreshAfterClicked()
{
	for (size_t i = 0; i < m_rows; i++)
	{
		select_edit[i] = false;
		m_editBtn[i]->setSelected(false);
		m_editBtn[i]->setName("编辑");
	}
}
