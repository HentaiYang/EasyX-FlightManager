#include "Table.h"
#include <iostream>
const int Table_Move_X = -30;	//�Ա��������ƶ�
const int Table_Move_Y = 80;//�Ա��������ƶ�
const int Table_Move_Btn9_X = 0;//����Ÿ���ť�������ƶ�
const int Table_Move_Btn9_Y = 0;//����Ÿ���ť�������ƶ�
const int Table_Move_Btn4_X = 380;//�����ĸ���ť�������ƶ�
const int Table_Move_Btn4_Y = 100;//�����ĸ���ť�������ƶ�
const int Page_Move_X = 10;			//ҳ���������ƶ�
const int Edit_Move_X = 10;			//�༭��ɾ���������ƶ�
const int Edit_Space_Size = 10;		//�༭����ɾ�����ļ������
const int Sort_Move_X = 350;			//������������ƶ�
const int Sort_Move_Y = -66;			//������������ƶ�
Table::Table(int row, int col)
	:BasicWidget(0,0,0,0)	//��ʼ����������
	,m_rows(row),m_cols(col)//��ʼ��������
	,m_curPage(0),m_maxPage(0),m_exrtaData(0)//��ʼ��ҳ�������
{
	readCity("./images/city.txt");			//��������ļ�
	m_prevBtn = new PushButton("��һҳ");//��һҳ��ť
	m_nextBtn = new PushButton("��һҳ");//��һҳ��ť
	m_firstBtn = new PushButton("��ҳ");//��ҳ��ť
	m_lastBtn = new PushButton("βҳ");//βҳ��ť
	//�����������ť��Ҫȷ�����ڲſ�����ʾ��Ĭ������Ϊ����
	m_prevWeek = new PushButton("<<");//���ܰ�ť
	m_nextWeek = new PushButton(">>");//���ܰ�ť
	for (int i = 0; i < 7; i++) {		//����һ�ܵİ�ť
		m_oneWeek.push_back(new PushButton("������"));
	}

	time(&today);
	show_day = today;
	today_tm = *localtime(&today);
	select_sortbtn = -1;

	//δ���ر�񣬻ᱨ�� refreshShowday_OneWeek();//��������չʾ��ʱ���һ�ܵİ�ť

	m_sortBtn.push_back(new PushButton("�ͼ�����"));
	m_sortBtn.push_back(new PushButton("�߼�����"));
	m_sortBtn.push_back(new PushButton("�����-��"));
	m_sortBtn.push_back(new PushButton("�����-��"));
	m_sortBtn.push_back(new PushButton("ʱ���-��"));
	m_sortBtn.push_back(new PushButton("ʱ�䳤-��"));
	m_numberBtn = new PushButton("����κŲ�ѯ");
}

Table::~Table()//��ֹ�ڴ�й©
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

	//�������ֿ�Ⱥ͸߶ȣ���λΪ����
	m_tw = ::textwidth("��³ľ��");
	m_th = ::textheight(header.c_str());

	//���ø��ӿ�Ⱥ͸߶ȣ���λΪ����
	m_gridW = m_tw + 10;//�зɻ��ĳ���������ĸ��֣����ڣ�
	m_gridH = m_th + 10;//�ñ�ͷ�涨�߶�

	//���±���Ⱥ͸߶�
	m_w = m_gridW * m_cols;
	m_h = m_gridH * m_rows;
}

void Table::setButton(int flag)
{
	if (flag == 0) 
	{
		for (size_t i = 0; i < m_rows; i++)
		{
			m_editBtn.push_back(new PushButton("�༭"));
			m_deleteBtn.push_back(new PushButton("ɾ��"));
		}
	}
	if (flag == 1)
	{
		for (size_t i = 0; i < m_rows; i++)
		{
			m_editBtn.push_back(new PushButton("��Ʊ"));
			m_deleteBtn.push_back(new PushButton("��Ʊ"));
		}
	}
	for (size_t i = 0; i < m_rows; i++)	//ÿ�����Ӷ���һ���༭��ť
	{
		std::vector<PushButton*> btns;
		for (size_t j = 0; j < m_cols; j++)
		{
			btns.push_back(new PushButton("edit"));
		}
		m_editAllBtn.push_back(btns);
	}
	m_takeoffBtn = new PushButton("ֻ�鿴δ���");
	m_insertBtn = new PushButton("¼�뺽��");
	for (size_t i = 0; i < m_rows + 1; i++)
	{
		m_insertRowBtn.push_back(new PushButton("¼��->"));
	}
	
	//��ʼ����ťλ��
	m_prevBtn->move(m_x + Table_Move_Btn4_X, m_y + m_h + Table_Move_Btn4_Y);
	m_nextBtn->move(m_prevBtn->x() + m_prevBtn->width(), m_prevBtn->y());
	m_firstBtn->setFixedSize(80, 30);
	m_lastBtn->setFixedSize(80, 30);
	m_firstBtn->move(m_nextBtn->x() + m_nextBtn->width(), m_nextBtn->y());
	m_lastBtn->move(m_firstBtn->x() + m_firstBtn->width(), m_firstBtn->y());
	int bw = 110;
	int bh = 40;	//������߸���ť���һ��
	m_prevWeek->setFixedSize(bh, bh);
	m_nextWeek->setFixedSize(bh, bh);
	m_prevWeek->move(Table_Move_Btn9_X + (Window::width() - 100 - bw * 7) / 2, Table_Move_Btn9_Y);//�Ե�һ����ťΪ׼
	m_nextWeek->move(m_prevWeek->x() + m_prevWeek->width() + bw * 7, m_prevWeek->y());
	for (size_t i = 0; i < 7; i++) {
		m_oneWeek[i]->setFixedSize(bw, bh);
		m_oneWeek[i]->move(m_prevWeek->x() + m_prevWeek->width() + i * bw, m_prevWeek->y());
	}

	for (size_t i = 0; i < m_rows; i++)//�༭��ɾ����ť����
	{
		int tw = 38;	//�༭��ɾ����ť��С
		int th = 22;
		m_editBtn[i]->setFixedSize(tw, th);
		m_deleteBtn[i]->setFixedSize(tw, th);
		int my = (m_gridH - th) / 2;//���а�ť
		m_editBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + Edit_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
		m_deleteBtn[i]->move(m_x + Table_Move_X + m_cols * m_gridW + tw + Edit_Space_Size + Edit_Move_X, m_y + m_gridH * i + Table_Move_Y + my);
	}

	for (size_t i = 0; i < 6; i++)//����ť����
	{
		int tw = 90;	//����ť��С
		int th = 26;
		m_sortBtn[i]->setFixedSize(tw, th);
		m_sortBtn[i]->move(m_x + Table_Move_X + Sort_Move_X + tw * i, m_y + Table_Move_Y + Sort_Move_Y);
	}
	for (size_t i = 0; i < m_rows; i++)//ÿ���༭��ť����
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
	m_datas = &data;//����һ��vec_all������������
}

void Table::readCity(const std::string& fileName)
{
	std::fstream read(fileName, ios::in);//�����ļ�
	if (!read.is_open())			//���û�д��ļ��ͷ���
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
	drawHeader();	//���Ʊ�ͷ
	drawTableGrid();//���Ʊ��
	drawTableData();//��������
	if (m_maxPage >= 0)
	{
		drawButton();
	}
}

void Table::drawTableGrid()//��ť�ͱ��һ�����
{
	//����а��У���������Ϊ�� Ϊ���ۿ��ʵ�����
	//���5�ֽڣ����ʱ��5�ֽڣ�����8�ֽڣ�����ʱ��2�ֽ�
	//�����3�ֽڣ��Ѷ�Ʊ3�ֽڣ��Ƿ����2�ֽ�

	setlinecolor(BLACK);		//��������ɫ
	setlinestyle(PS_SOLID, 1);	//������

	//������
	for (size_t i = 0; i < m_rows + 1; i++)
	{
		line(m_x + Table_Move_X, m_y + m_gridH * i + Table_Move_Y, m_x + m_cols * m_gridW + Table_Move_X, m_y + m_gridH * i + Table_Move_Y);
	}
	//������
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
	::settextstyle(17, 0, "����");
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
	::settextstyle(24, 0, "����");
	if(!select_number) m_insertBtn->show();

	setlinecolor(LIGHTGRAY);
	::settextstyle(12, 0, "����");
	::settextcolor(DARKGRAY);
	m_numberBtn->show();
	::settextstyle(22, 0, "����");
	setlinecolor(BLACK);
	::settextcolor(BLACK);
	int tx = 20;
	int ty = 20;
	outtextxy(m_numberBtn->x() + tx, m_numberBtn->y() + ty, number);

	::settextstyle(15, 0, "����");
	//m_takeoffBtn->show();
	if (!select_number && select_insert)
	{
		for (size_t i = 0; i < btncnt + 1; i++)
		{
			m_insertRowBtn[i]->show();
		}
	}
	
	::settextstyle(20, 0, "����");
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
	sprintf_s(str, "��%dҳ / ��%dҳ", m_curPage + 1, m_maxPage + 1);
	outtextxy(m_lastBtn->x() + m_lastBtn->width() + Page_Move_X, m_lastBtn->y() + (m_lastBtn->height() - m_th) / 2, str);
}

void Table::drawTableData()
{
	//m_nowday = find_today(0, m_datas.size());
	if (week_change)
	{
		refreshOneWeek();//����һ�ܵİ�ť
	}
	//(m_datas->begin() + m_showday)->vec_flt; �ǵ�ǰչʾ���ڵ�����

	//���û��ɸѡ�Ͱ��������ҳ��
	//if(!select_takeoff) 
	if (select_number)
	{
		int num = dich(0, (m_datas->begin() + m_showday)->vec_flt.size() - 1, atoi(number));
		if (num == -1)
		{
			outtextxy(m_x + Table_Move_X + 10, m_y + 20, "û���ҵ��ú���");
			m_curPage = 0;
			m_maxPage = 0;
			m_exrtaData = 0;
			return;
		}
		else
		{
			output_data.clear();
			output_data.push_back(split((m_datas->begin() + m_showday)->vec_flt[num].formatInfo(), '\t'));
			for (size_t k = 0; k < m_cols; k++)//������
			{
				int tx = m_x + k * m_gridW + (m_gridW - ::textwidth(output_data[0][k].c_str())) / 2;//���ƾ���
				int ty = m_y + 5;//���ƾ���
				outtextxy(tx + Table_Move_X, ty + Table_Move_Y, output_data[0][k].c_str());
			}
		}
		m_curPage = 0;
		m_maxPage = 0;
		m_exrtaData = 1;
		return;
	}
	updatePage();			//���¸����Ӧ��ҳ��������
	if ((m_datas->begin() + m_showday)->vec_flt.size() == 0) return;
	if (m_curPage > m_maxPage) m_curPage = m_maxPage;
	if (m_curPage < 0) m_curPage = 0;
	int begPos = m_curPage * m_rows;	//���ݿ�ʼ��λ��
	int endPos = begPos + m_rows;	//���ݽ�����λ��
	if (m_curPage == m_maxPage) endPos = begPos + m_exrtaData;
	output_data.clear();
	/*if (select_takeoff)
	{
		time(&today);
		today_tm = *localtime(&today);//���½���ʱ��
		if ((m_datas->begin() + m_showday)->year == today_tm.tm_year + 1900 && (m_datas->begin() + m_showday)->month == today_tm.tm_mon + 1 && (m_datas->begin() + m_showday)->day == today_tm.tm_mday)
		{
			for (size_t i = begPos, j = 0; i < (m_datas->begin() + m_showday)->vec_flt.size() && j < m_rows; i++)
			{
				if ((m_datas->begin() + m_showday)->vec_flt[i].hour * 60 + (m_datas->begin() + m_showday)->vec_flt[i].minute > today_tm.tm_hour * 60 + today_tm.tm_min)
				{
					j++;
					(m_datas->begin() + m_showday)->vec_flt[i].take_off = "��";
					(m_datas->begin() + m_showday)->vec_flt[i].take_off_bool = false;
					output_data.push_back(split((m_datas->begin() + m_showday)->vec_flt[i].formatInfo(), '\t'));
				}
			}
		}
		//��������ҳ����Ϣ
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
	
	for (size_t i = begPos; i < endPos; i++)		//������
	{
		for (size_t k = 0; k < m_cols; k++)//������
		{
			int tx = m_x + k * m_gridW + (m_gridW - ::textwidth(output_data[i - begPos][k].c_str())) / 2;//���ƾ���
			int ty = m_y + (i - begPos) * m_gridH + 5;//���ƾ���
			outtextxy(tx + Table_Move_X, ty + Table_Move_Y, output_data[i - begPos][k].c_str());
		}
	}
	string s = "���պ�������";
	s += to_string((m_datas->begin() + m_showday)->n);
	::settextstyle(22, 0, "����");
	outtextxy(m_x + Table_Move_X + 10, m_y + 20, s.c_str());
	::settextstyle(20, 0, "����");
}

void Table::drawHeader()
{
	//���ƾ���
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

	for (size_t pos = 0; pos != std::string::npos;)//npos��size_t�ܱ�ʾ�����ֵ
	{
		pos = str.find(separator);//�ҵ�ָ���ָ��ַ��λ��

		res.push_back(str.substr(0, pos));//����ָ���ַ���
		str = std::string(str.c_str() + pos + 1);//�ؽ��ַ������ڶ������أ���c�����ַ�����ַ������
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
		if (m_curPage != 0)//��ǰ����ҳ���ǵ�һҳ������ǰ��
		{
			m_curPage--;
			refreshAfterClicked();
		}
	}
	if (m_nextBtn->isClicked())
	{
		if (m_curPage != m_maxPage)//��ǰҳ�������ҳ��������
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
		if (m_sunday > 0)//�ж��Ƿ�����һ��
		{
			m_sunday -= 7;
			time_t t = show_day;
			tm t2 = *localtime(&t);
			bool flag = (m_showday - t2.tm_wday == m_sunday ? true : false);
			if (flag)	//�������ڷ�ҳ��Ҫ��֤δѡ��İ�ť��Ϊѡ����ɫ
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
		if (m_sunday + 7 < (int)m_datas->size())//�ж��Ƿ�����һ��
		{
			m_sunday += 7;
			time_t t = show_day;
			tm t2 = *localtime(&t);
			bool flag = (m_showday - t2.tm_wday == m_sunday ? true : false);
			if (flag)	//�������ڷ�ҳ��Ҫ��֤δѡ��İ�ť��Ϊѡ����ɫ
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
			//�����������ݾͲ��л�
			if (m_sunday + i >= 0 && m_sunday + i < m_datas->size())
			{
				refreshAfterClicked();
				//�ı���ǰѡ�а�ť��ɫ
				m_oneWeek[select_btn]->setSelected(false);
				if (select_sortbtn != -1)
				{
					m_sortBtn[select_sortbtn]->setSelected(false);
					select_sortbtn = -1;
				}
				ArraySort::defSort((m_datas->begin() + m_showday)->vec_flt);
				show_day -= 86400 * (m_showday - m_sunday - i);//�ı�����չʾ������
				m_showday = find_showday();
				m_curPage = 0;				//չʾ��һҳ
				select_btn = i;
				//�ı�ѡ�а�ť��ɫ
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
				m_editBtn[i]->setName("�༭");
			}
			else
			{
				select_edit[i] = true;
				m_editBtn[i]->setSelected(true);
				m_editBtn[i]->setName("ֹͣ");
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
				m_sortBtn[select_sortbtn]->setSelected(false);	//����ѡ����ɫ
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
			m_insertBtn->setName("¼�뺽��");
		}
		else
		{
			select_insert = true;
			m_insertBtn->setSelected(true);
			m_insertBtn->setName("ֹͣ¼��");
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
			while (InputBox(number, 1024, "�������κ�", NULL, NULL, 0, 0, false))
			{
				bool flag = true;
				for (size_t i = 0; i < strlen(number); i++)
				{
					if (!isdigit(number[i]))
						flag = false;
				}
				if (!flag)
				{
					MessageBox(NULL, "���������֣�", "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					continue;
				}
				if (strlen(number) > 8)
				{
					MessageBox(NULL, "���ֹ���", "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
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
				while (InputBox(s, 1024, "�밴����� ���ʱ�� ���վ �յ�վ ����ʱ�� ����� �Ѷ�Ʊ �Ƿ���� �۸񡱵�˳������\n����������123 06:30 �ൺ ���� 2 120 100 �� 3164��", "�����뺽����Ϣ", NULL, 0, 0, false))
				{
					stringstream ss(s);
					int num, drtm, mx, bk, pri;
					string tktm, st, ed, tk;
					try {
						ss >> num >> tktm >> st >> ed >> drtm >> mx >> bk >> tk >> pri;
						if (num < 0)
						{
							MessageBox(NULL, "��κŲ���Ϊ������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (num > 1e7)
						{
							MessageBox(NULL, "������İ�κŹ���", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (tktm.size() != 5 || !isdigit(tktm[0]) || !isdigit(tktm[1]) || tktm[2] != ':' || !isdigit(tktm[3]) || !isdigit(tktm[4]))
						{
							MessageBox(NULL, "���ʱ���ʽ����(��ǰ׺0)", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						string s = tktm;
						int h = atoi(s.substr(0, 2).c_str());
						int m = atoi(s.substr(3).c_str());
						if (h < 0 || h > 23)
						{
							MessageBox(NULL, "Сʱ����ΧΪ0-23��", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (m < 0 || m > 59)
						{
							MessageBox(NULL, "��������ΧΪ0-59��", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (!mp_city.count(st))
						{
							MessageBox(NULL, "���վ�����ڻ�ó����޻�����", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (!mp_city.count(ed))
						{
							MessageBox(NULL, "�յ�վ�����ڻ�ó����޻�����", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (drtm < 0)
						{
							MessageBox(NULL, "����ʱ�䲻��Ϊ������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (drtm > 24)
						{
							MessageBox(NULL, "����ʱ�䲢������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (mx < 0)
						{
							MessageBox(NULL, "���������Ϊ������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (bk < 0)
						{
							MessageBox(NULL, "�Ѷ�Ʊ������Ϊ������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (bk > mx)
						{
							MessageBox(NULL, "�Ѷ�Ʊ�����ܴ��ڶ������", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (tk != "��" && tk != "��")
						{
							MessageBox(NULL, "�Ƿ���������롰�ǡ��򡰷�", "����������", MB_OK | MB_SYSTEMMODAL);
							continue;
						}
						if (pri < 0)
						{
							MessageBox(NULL, "�۸���Ϊ������", "����������", MB_OK | MB_SYSTEMMODAL);
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
						MessageBox(NULL, "������ĸ�ʽ����", "����������", MB_OK | MB_SYSTEMMODAL);
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
					char title[20] = "�޸�";
					char remind[20] = "ԭ��ֵΪ��";
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
								s += " Ϊ���������������룡";
								MessageBox(NULL, s.c_str(), "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
								continue;
							}
							else if (j == 5 && atoi(s) < flt.booked)
							{
								MessageBox(NULL, "��������ܵ��ڶ�Ʊ����", "�������ݴ���", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
								continue;
							}
							else if (j == 6 && atoi(s) > flt.max_load)
							{
								MessageBox(NULL, "��Ʊ�����ܸ��ڶ������", "�������ݴ���", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
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
						else//Ҫ�����ַ���
						{
							if (j == 1)//����ʱ��
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
									s += " ��ʽΪxx:xx(��ǰ׺0)�����������룡";
									MessageBox(NULL, s.c_str(), "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
									continue;
								}
							}
							else if (j == 2 || j == 3)//�������
							{
								string city = s;
								if (!mp_city.count(city))
								{
									string s = vec_header[j];
									s += " ���д����ó���û�л��������������룡";
									MessageBox(NULL, s.c_str(), "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
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
								if (flag == "��" || flag == "��")
								{
									flt.take_off = flag;
									flt.take_off_bool =  flag == "��" ? true : false;
									m_editAllBtn[i][j]->setName(flt.take_off);
									break;
								}
								else
								{
									string s = vec_header[j];
									s += " �����롰�ǡ��򡰷񡱣����������룡";
									MessageBox(NULL, s.c_str(), "�����ʽ����", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
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
	//�ñ༭��ťΪ��β���ټӰ�ť��������
}

//����ʾ�û��Ƿ�ȷ��ɾ������ɾ������
//���Կ��ǼӸ�����ѡ�ȡ��ɾ������
void Table::delete_data(size_t num)
{
	if (MessageBox(NULL, "��ȷ��ɾ���ú�����", "ɾ������", MB_OKCANCEL | MB_SYSTEMMODAL) != IDOK)//���Կ��ǰѺ�����Ϣ��ӡһ��
	{
		return;
	}
	Day_Flight& flt = *(m_datas->begin() + m_showday);
	flt.vec_flt.erase(flt.vec_flt.begin() + m_curPage * m_rows + num);//ɾ����Ӧ��
	flt.n--;						//���ٵ��캽����
	updatePage();		//ˢ�µ�ǰչʾ����m_extraPage������
	refreshAfterClicked();
}

void Table::setToday()
{
	time_t day = today;
	tm t = *localtime(&day);
	select_btn = t.tm_wday;					//Ĭ�Ͻ���İ�ť��ѡ��
	t.tm_year = m_datas->begin()->year - 1900;
	t.tm_mon = m_datas->begin()->month - 1;
	t.tm_mday = m_datas->begin()->day;
	day = mktime(&t);
	m_today = ((int)difftime(today, day)) / 86400;//����m_today
	m_showday = find_showday();				//����m_showday
	m_sunday = m_showday - select_btn;		//��ǰ�߸���ťչʾ���ݶ�Ӧ����������Ӧ���±�
	m_oneWeek[select_btn]->setSelected(true);//Ĭ�ϴ򿪽���
	updatePage();
	m_curPage = 0;
}

int Table::find_showday()//��Ϊ�������������м䲻����û�к�������ڣ�
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
	//m_oneWeek[wtoday];//���ǵ�ǰչʾ�ı���Ӧ��ť
	for (int i = m_sunday; i < m_sunday + 7; i++) {
		std::string s = "";
		if (i < 0 || i >= m_datas->size())
		{
			s += "������";
		}
		else
		{
			s += std::to_string((m_datas->begin() + i)->month);
			s += "��";
			s += std::to_string((m_datas->begin() + i)->day);
			s += "�� ";
			if (i != m_today) s += Week_Str[i - m_sunday];
			else s += "����";
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
			m_editBtn[i]->setName("�༭");
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
	m_insertBtn->setName("¼�뺽��");
}

void Table::refreshAfterClicked()
{
	for (size_t i = 0; i < m_rows; i++)
	{
		select_edit[i] = false;
		m_editBtn[i]->setSelected(false);
		m_editBtn[i]->setName("�༭");
	}
}
