#pragma once
#include "Window.h"
#include "PushButton.h"
#include "QueryTable.h"
#include "Flight.h"
#include "Table.h"
#include <vector>
#include <sstream>//�ָ�����
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
	//����������
	void run();

	int menu();

	//��ʾ���к���
	void edit();
	//��ѯ����Ʊ����Ʊ
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

private://���ڱ����ļ��к�����Ϣ
	std::string m_header;	//�����ͷ
	std::vector<Day_Flight> vec_all;//�����еĺ��࣬һ��Ϊһ������
	std::vector<Day_Flight> vec_all_copy;//�������飬���������ָ�����
private:
	Table* m_showTable;//�鿴�ñ��
	QueryTable* m_queryTable;//��ѯ�ñ��
};

