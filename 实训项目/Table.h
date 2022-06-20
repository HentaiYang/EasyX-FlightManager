#pragma once
#include "BasicWidget.h"
#include "Configure.h"
#include "Flight.h"
#include "PushButton.h"
#include "ArraySort.h"
#include "Window.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <map>
enum Week {
	Sunday,
	Monday,
	Tuesday, 
	Wednesday, 
	Thursday,
	Friday,
	Saturday
};
static std::string Week_Str[7] = {
	"����",
	"��һ",
	"�ܶ�",
	"����",
	"����",
	"����",
	"����"
};

class Table :
	public BasicWidget
{
public:
	Table(int row = 0, int col = 0);	//��Ĵ�С
	~Table();
	void setRowCount(int row);			//����������
	void setColCount(int col);			//����������
	void setHeader(const std::string& header);
	void setButton(int flag);
	void insertData(std::vector<Day_Flight>& data);
	void readCity(const std::string& fileName);

	void show() override;				//��д�麯��
	void drawTableGrid();				//���Ʊ��
	void drawButton();					//���ư�ť
	void drawTableData();				//��������
	void drawHeader();					//���Ʊ�ͷ
	int dich(int l, int r, int x);
	static std::vector<std::string> split(std::string str, char separator = '\t');

	void updatePage();		//����ҳ����

	void eventLoop(const ExMessage& msg);

	//void update_data(size_t num);	//�༭��ťʵ�ֺ���
	void delete_data(size_t num);	//ɾ����ťʵ�ֺ���

	void setToday();
	int find_showday();		//����m_datas�к�show_day��ͬ���ڵ��±�
	void refreshOneWeek();
	void refreshTable();	//���������������������ť��ѡ��״̬�������߸���ť��ѡ��״̬

public://�Ż�ϵͳ
	bool open = false;	//����Ƿ񱻿�����
	bool week_change = true;

private://�Ż�ϵͳ
	void refreshAfterClicked();

public:
	std::map<std::string, std::size_t> mp_city;
	std::vector<std::string> m_city;
	size_t citySum[26];

private:
	int m_rows;		//����
	int m_cols;		//����

	int m_gridW;	//���ӿ��
	int m_gridH;	//���Ӹ߶�

	int m_tw;		//���ֿ��
	int m_th;		//���ָ߶�

	std::vector<std::string> m_header;
	std::vector<std::string> vec_header;
	std::vector<Day_Flight>* m_datas;//����һ��vec_all
	std::vector<std::vector<std::string> > output_data;

	time_t today;		//���ڵ�ʱ��
	tm today_tm;		//���ڵ�ʱ���tm��ʽ
	int m_today;		//��ǽ����ʱ���Ӧ�±�

	time_t show_day;	//����չʾ������
	time_t sun_day;		//�����±��Ӧ��time_t��ʽ����ѡ
	int m_showday;		//���ڱ������չʾ������������ģ�m_datas���±꣩
	int m_sunday;		//���ڱ�ǵ�ǰ�Ϸ��߸���ťչʾ�����յ��±�

	int select_btn;		//Ŀǰ�Ϸ��߸���ť��ʾ��ѡ�е��±�
	int select_sortbtn;	//Ŀǰѡ�е�����ť����Ĭ��ֵ

private://��ҳ��
	int m_curPage;	//��ǰҳ
	int m_maxPage;	//���ҳ��
	int m_exrtaData;//������ҳ�����һҳ��ʣ���������

	PushButton* m_prevBtn;//��һҳ��ť
	PushButton* m_nextBtn;//��һҳ��ť
	PushButton* m_firstBtn;//��ҳ��ť
	PushButton* m_lastBtn;//βҳ��ť
	//�����������ť��Ҫȷ�����ڲſ�����ʾ��Ĭ������Ϊ����
	PushButton* m_prevWeek;//���ܰ�ť
	PushButton* m_nextWeek;//���ܰ�ť
	std::vector<PushButton*> m_oneWeek;	//��ǰ�ܵ����յ�����
	std::vector<PushButton*> m_editBtn;	//�޸İ�ť
	std::vector<PushButton*> m_deleteBtn;	//ɾ����ť
	std::vector<PushButton*> m_sortBtn;	//����ť
	std::vector<PushButton*> m_screenBtn;//ɸѡ��ť
	std::vector<std::vector<PushButton*> > m_editAllBtn;//ÿ�����ӵı༭��ť
	std::vector<bool> select_edit;//��һ�ű�����˱༭
	PushButton* m_takeoffBtn;	//ֻ��δ��ɵĺ���
	bool select_takeoff = false;//�Ƿ�ɸѡδ��ɺ���
	PushButton* m_insertBtn;	//¼�뺽��
	bool select_insert = false;
	std::vector<PushButton*> m_insertRowBtn;//ÿ��һ�����뺽�ఴť
	PushButton* m_numberBtn;
	bool select_number = false;
	char number[1024] = { 0 };
};

