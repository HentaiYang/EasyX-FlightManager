#include "Management.h"
#include <iostream>
#include <conio.h>
#include <fstream>//��д����

using uint32 = unsigned int;
using namespace std;
Management::Management()
{
	Window::beginDraw();
	::loadimage(&m_bk, "./images/open_bk.png", Window::width(), Window::height());
	Window::clear();
	drawBackground();//д�뱳��
	Window::flushDraw();//��ʾ��������
	readFile("./images/flight.txt");	//�������ʼ��ʱ�����ļ�
	
	::settextstyle(20, 0, "����");//	����Ĭ������
	::settextcolor(BLACK);
	
	//�����水ť��ʼ��
	menu_btns.push_back(new PushButton("������Ϣ����"));
	menu_btns.push_back(new PushButton("��ѯ��·/��Ʊ��Ʊ"));
	menu_btns.push_back(new PushButton("�˳�ϵͳ"));

	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->setFixedSize(250, 50); //�����ð�����С

		int bx = (Window::width() - menu_btns[i]->width()) / 2;	//ʹ���Ҿ���
		int vspace = (Window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;	//�����һ����ťӦ�þ��������Զ
		int by = vspace + i * menu_btns[i]->height();	//�ֱ����ð�ť�ĸ߶�
		menu_btns[i]->move(bx, by);		//��ť���д���
	}

	//��ʼ���鿴����ı��
	m_showTable = new Table(16, 0);
	//m_showTable->setRowCount(16);		//��������
	//m_showTable->setColCount(8); ��setHeader���ݱ�ͷ�������������Ը���
	m_showTable->setHeader(m_header);	//�������������ִ�С
	m_showTable->insertData(vec_all);//��vec_all����һ�ݵ������
	m_showTable->setToday();//û�ط����ˣ���ѭ�����˷�
	//ʹ������
	m_showTable->move((Window::width() - m_showTable->width()) / 2, 50);
	m_showTable->setButton(0);

	//��ʼ����ѯ�ı��
	m_queryTable = new QueryTable(5, 1);
	m_queryTable->insertData(vec_all);
	m_queryTable->setToday();//û�ط����ˣ���ѭ�����˷�
	m_queryTable->readCity("./images/city.txt");//��������ļ�
	//ʹ������
	m_queryTable->move((Window::width() - m_queryTable->width()) / 2, 50);
	m_queryTable->setButton(1);

	::loadimage(&m_bk, "./images/home_bk.png", Window::width(), Window::height());
}
void Management::run()
{
	int op = Menu;
	//Ϊ�����������棬д�ڹ��캯�� Window::beginDraw();//Ԥ����
	while(true)
	{
		Window::clear();
		drawBackground();//д�뱳��
		if (Window::hasMsg())
		{
			m_msg = Window::getMsg();
			switch (m_msg.message)
			{
			case WM_KEYDOWN:	//��������
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
			default:  //������
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
			saveFile("./images/flight.txt");//�˳�ϵͳǰ�����ļ�
			exit(666);
			break;
		}

		Window::flushDraw();//��ʾ��������ť

	}
	Window::endDraw();
}

int Management::menu()
{
	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->show();	//��ʾ��ť
		menu_btns[i]->eventLoop(m_msg);		//�¼�ѭ���������m_msg���ȡ������꣬��ô����button���оͿ���ֱ��ʹ��
		if (menu_btns[i]->isClicked())		//�����ť�����
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
	//�����水ť��ʼ��
	menu_btns.push_back(new PushButton("��ʾ������Ϣ"));
	menu_btns.push_back(new PushButton("�༭������Ϣ"));
	menu_btns.push_back(new PushButton("��ѯ������Ϣ"));
	menu_btns.push_back(new PushButton("��Ʊ"));
	menu_btns.push_back(new PushButton("��Ʊ"));
	menu_btns.push_back(new PushButton("�˳�ϵͳ"));

	for (int i = 0; i < menu_btns.size(); i++)
	{
		menu_btns[i]->setFixedSize(250, 50); //�����ð�����С

		int bx = (Window::width() - menu_btns[i]->width()) / 2;	//ʹ���Ҿ���
		int vspace = (Window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;	//�����һ����ťӦ�þ��������Զ
		int by = vspace + i * menu_btns[i]->height();	//�ֱ����ð�ť�ĸ߶�
		menu_btns[i]->move(bx, by);		//��ť���д���
	}
}

void Management::readFile(const std::string& fileName)
{
	fstream read(fileName, ios::in);//�����ļ�
	if (!read.is_open())			//���û�д��ļ��ͷ���
	{
		return;
	}
	char buf[1024] = { 0 };
	read.getline(buf, 1024);
	m_header = buf;				//��ȡ��ͷ���ļ����Ա����ʽ�洢��

	while (!read.eof())			//��ȡ����
	{
		char data1[1024] = { 0 };
		read.getline(data1, 1024);//��һ�д����� �� �� ������
		if (strlen(data1) == 0)
			break;				//��һ�ļ�β�п���������
		stringstream ss1(data1);
		Day_Flight dflt;		//����һ��ĺ����ݴ��
		ss1 >> dflt.year >> dflt.month >> dflt.day >> dflt.n;
		for (int i = 0; i < dflt.n; i++) {
			char data2[1024] = { 0 };
			read.getline(data2, 1024);//��ȡÿ�еĺ�����Ϣ
			Flight flt;
			stringstream ss(data2);

			ss >> flt.number >> flt.takeoff_time >> flt.start >> flt.end >> flt.during_time >> flt.max_load >> flt.booked >> flt.take_off >> flt.price;
			//������� cout << flt.number << flt.hour << flt.minute << flt.start << flt.end << flt.during_time << flt.max_load << flt.booked << flt.take_off << endl;
			flt.init();		//��ȫhour��minute��take_off_bool��ֵ
			dflt.vec_flt.push_back(flt);//���뵱�պ����
		}
		vec_all.push_back(dflt);		//���պ��������ܱ�vec_all
	}
	//���º���������
	time_t today;
	tm today_tm;
	time(&today);
	today_tm = *localtime(&today);//���½���ʱ��
	for (size_t i = 0; i < vec_all.size(); i++)
	{
		if (vec_all[i].year == today_tm.tm_year + 1900 && vec_all[i].month == today_tm.tm_mon + 1 && vec_all[i].day == today_tm.tm_mday)
		{
			for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
			{
				//����ú������ʱ���������ʱ�䣬˵��û���
				if (vec_all[i].vec_flt[j].hour * 60 + vec_all[i].vec_flt[j].minute > today_tm.tm_hour * 60 + today_tm.tm_min)
				{
					vec_all[i].vec_flt[j].take_off = "��";
					vec_all[i].vec_flt[j].take_off_bool = false;
				}
				else//���������
				{
					vec_all[i].vec_flt[j].take_off = "��";
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
			if (d < today)//�Ƚ����磬˵���������
			{
				for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
				{
					vec_all[i].vec_flt[j].take_off = "��";
					vec_all[i].vec_flt[j].take_off_bool = true;
				}
			}
			else//�Ƚ�������û���
			{
				for (size_t j = 0; j < vec_all[i].vec_flt.size(); j++)
				{
					vec_all[i].vec_flt[j].take_off = "��";
					vec_all[i].vec_flt[j].take_off_bool = false;
				}
			}
		}
	}
	//for (int i = 0; i < vec_all.size(); i++)
	//{
	//	vec_all_copy.push_back(vec_all[i]);	//�������ݵ���������vec_all_copy
	//}
	read.close();
}

void Management::saveFile(const std::string& fileName)
{
	fstream write(fileName, ios::out | ios::trunc); //���ļ���������ھ�д�뷽���������ھʹ�������
	if (!write.is_open())
	{
		cerr << fileName << "�ļ�δ�ܴ򿪣�" << endl; //�ô�����������������Ϣ
		return;
	}

	m_header += "\n";				//��ͷ��β�ӻ��У������ٻ���
	write.write(m_header.c_str(), m_header.size());//д���ͷ

	for (auto& val_day : vec_all)			//����ÿ��
	{
		//������� cout << val_day.year << "��" << val_day.month << "��" << val_day.day << "�� ���պ�������" << val_day.n << endl;
		std::string day_title = val_day.formatInfo();
		write.write(day_title.c_str(), day_title.size());

		for (auto& val : val_day.vec_flt)	//������������к���
		{
			//������� cout << val.formatInfo();
			std::string info = val.formatInfo();	//���ص���string����
			write.write(info.c_str(), info.size());	//writeҪ������Ϊc����str
		}
	}

	write.close();
}
