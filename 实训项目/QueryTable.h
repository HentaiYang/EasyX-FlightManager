#pragma once
#include "BasicWidget.h"
#include "Configure.h"
#include "Flight.h"
#include "PushButton.h"
#include "ArraySort.h"
#include "Window.h"
#include "Table.h"
#include <vector>
#include <ctime>
#include <stack>
class QueryTable :
	public BasicWidget
{
public:
	QueryTable(int row = 0, int col = 1);	//��Ĵ�С
	~QueryTable();
	void setRowCount(int row);			//����������
	void setColCount(int col);			//����������
	void setButton(int flag);
	void insertData(std::vector<Day_Flight>& data);
	void readCity(const std::string& fileName);

	void show() override;				//��д�麯��
	void drawTableGrid();				//���Ʊ��
	void drawButton();					//���ư�ť
	void drawTableData();				//��������

	void updatePage(std::vector<Result> datas);		//����ҳ����

	void eventLoop(const ExMessage& msg);

	bool canbuy_return(Result data);

	void setToday();
	int find_showday();		//����m_datas�к�show_day��ͬ���ڵ��±�
	std::vector<Result> find_result();		//���ݵ������ؽ��
	std::vector<Result> find_result2();		//���ݰ�κŷ��ؽ����
	int dich(int l, int r, int x);				//���ַ������ڰ�κŲ�ѯ
	void refreshOneWeek();
	void refreshTable();	//���������������������ť��ѡ��״̬�������߸���ť��ѡ��״̬

public://�Ż�ϵͳ
	bool open = false;	//����Ƿ񱻿�����
	bool week_change = true;

public:
	std::map<std::string, std::size_t> city2num;//����->���б�� map�ṹ
	std::vector<std::string> num2city;			//���б��->���� vector����

private:
	int m_rows;		//����
	int m_cols;		//����

	int m_gridW;	//���ӿ��
	int m_gridH;	//���Ӹ߶�

	int m_tw;		//���ֿ��
	int m_th;		//���ָ߶�

	char start[1024] = { 0 };
	char end[1024] = { 0 };
	char number[1024] = { 0 };

	std::vector<Day_Flight>* m_datas;//����һ��vec_all
	std::vector<Result> m_result;	//��ѯ�Ľ��
	std::vector<Result> m_result_copy;
	/*struct Result {
		int year, month, day, n;//n<=2�������ת����
		std::vector<Flight> flt;
	};*/
	std::stack<string> mainStk;	//���ڲ�ѯ����ջ�͸�ջ������ǳ�����
	std::stack<std::vector<Flight*> > supStk;
	size_t citySum[26];							//����ƴ���ĳ����ж��ٸ�

	time_t today;		//���ڵ�ʱ��
	tm today_tm;		//���ڵ�ʱ���tm��ʽ
	int m_today;		//��ǽ����ʱ���Ӧ�±�

	time_t show_day;	//��ѯ������������Ϣ
	int m_showday;		//���ڱ������չʾ���������ģ�m_datas���±꣩

	time_t sun_day;		//�����±��Ӧ��time_t��ʽ����ѡ
	int m_sunday;		//���ڱ�ǵ�ǰ�Ϸ��߸���ťչʾ�����յ��±�

	int select_btn;		//Ŀǰ�Ϸ��߸���ť��ʾ��ѡ�е��±�
	int select_sortbtn = -1;	//Ŀǰѡ�е�����ť��Ĭ��ֵ-1

	bool select_start = false;//�����ʼ���Ƿ�����
	bool select_end = false;//���Ŀ�ĵ��Ƿ�����
	bool select_change = false;//��ǲ�ѯ�ĵ���/��κ���û�з����仯
	bool select_swap = false;//��ǰ���κŲ�ѯ�Ƿ��
	bool select_number = false;//��ǰ�κ��Ƿ�����
	//bool select_date = false;

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
	std::vector<PushButton*> m_sortBtn;	//����ť
	std::vector<PushButton*> m_screenBtn;//ɸѡ��ť
	PushButton* m_startBtn;//�����ذ�ť
	PushButton* m_endBtn;//Ŀ�ĵذ�ť
	//PushButton* m_dateBtn;//���ڰ�ť
	std::vector<PushButton*> m_buyBtn;
	std::vector<PushButton*> m_returnBtn;
	PushButton* m_changeBtn;//�л���ѯ��ʽ��ť
	PushButton* m_numberBtn;//��κŰ�ť
};

