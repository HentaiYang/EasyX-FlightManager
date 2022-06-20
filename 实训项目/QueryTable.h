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
	QueryTable(int row = 0, int col = 1);	//表的大小
	~QueryTable();
	void setRowCount(int row);			//设置行数量
	void setColCount(int col);			//设置列数量
	void setButton(int flag);
	void insertData(std::vector<Day_Flight>& data);
	void readCity(const std::string& fileName);

	void show() override;				//重写虚函数
	void drawTableGrid();				//绘制表格
	void drawButton();					//绘制按钮
	void drawTableData();				//绘制数据

	void updatePage(std::vector<Result> datas);		//更新页数据

	void eventLoop(const ExMessage& msg);

	bool canbuy_return(Result data);

	void setToday();
	int find_showday();		//返回m_datas中和show_day相同日期的下标
	std::vector<Result> find_result();		//根据地名返回结果
	std::vector<Result> find_result2();		//根据班次号返回结果，
	int dich(int l, int r, int x);				//二分法，用于班次号查询
	void refreshOneWeek();
	void refreshTable();	//用于重置数组的排序、排序按钮的选择状态、上面七个按钮的选择状态

public://优化系统
	bool open = false;	//表格是否被开启过
	bool week_change = true;

public:
	std::map<std::string, std::size_t> city2num;//城市->城市编号 map结构
	std::vector<std::string> num2city;			//城市编号->城市 vector数组

private:
	int m_rows;		//行数
	int m_cols;		//列数

	int m_gridW;	//格子宽度
	int m_gridH;	//格子高度

	int m_tw;		//文字宽度
	int m_th;		//文字高度

	char start[1024] = { 0 };
	char end[1024] = { 0 };
	char number[1024] = { 0 };

	std::vector<Day_Flight>* m_datas;//复制一份vec_all
	std::vector<Result> m_result;	//查询的结果
	std::vector<Result> m_result_copy;
	/*struct Result {
		int year, month, day, n;//n<=2，最多中转两次
		std::vector<Flight> flt;
	};*/
	std::stack<string> mainStk;	//用于查询的主栈和辅栈，存的是城市名
	std::stack<std::vector<Flight*> > supStk;
	size_t citySum[26];							//该首拼音的城市有多少个

	time_t today;		//现在的时间
	tm today_tm;		//现在的时间的tm格式
	int m_today;		//标记今天的时间对应下标

	time_t show_day;	//查询结果是哪天的信息
	int m_showday;		//用于标记现在展示结果是哪天的（m_datas的下标）

	time_t sun_day;		//周日下标对应的time_t格式，备选
	int m_sunday;		//用于标记当前上方七个按钮展示的周日的下标

	int select_btn;		//目前上方七个按钮显示被选中的下标
	int select_sortbtn = -1;	//目前选中的排序按钮，默认值-1

	bool select_start = false;//标记起始地是否输入
	bool select_end = false;//标记目的地是否输入
	bool select_change = false;//标记查询的地名/班次号有没有发生变化
	bool select_swap = false;//标记按班次号查询是否打开
	bool select_number = false;//标记班次号是否输入
	//bool select_date = false;

private://分页器
	int m_curPage;	//当前页
	int m_maxPage;	//最大页数
	int m_exrtaData;//非整数页（最后一页）剩余多少数据

	PushButton* m_prevBtn;//上一页按钮
	PushButton* m_nextBtn;//下一页按钮
	PushButton* m_firstBtn;//首页按钮
	PushButton* m_lastBtn;//尾页按钮
	//下面的三个按钮都要确定日期才可以显示，默认日期为今天
	PushButton* m_prevWeek;//上周按钮
	PushButton* m_nextWeek;//下周按钮
	std::vector<PushButton*> m_oneWeek;	//当前周的周日到周六
	std::vector<PushButton*> m_sortBtn;	//排序按钮
	std::vector<PushButton*> m_screenBtn;//筛选按钮
	PushButton* m_startBtn;//出发地按钮
	PushButton* m_endBtn;//目的地按钮
	//PushButton* m_dateBtn;//日期按钮
	std::vector<PushButton*> m_buyBtn;
	std::vector<PushButton*> m_returnBtn;
	PushButton* m_changeBtn;//切换查询方式按钮
	PushButton* m_numberBtn;//班次号按钮
};

