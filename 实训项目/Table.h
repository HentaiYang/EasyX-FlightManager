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
	"周日",
	"周一",
	"周二",
	"周三",
	"周四",
	"周五",
	"周六"
};

class Table :
	public BasicWidget
{
public:
	Table(int row = 0, int col = 0);	//表的大小
	~Table();
	void setRowCount(int row);			//设置行数量
	void setColCount(int col);			//设置列数量
	void setHeader(const std::string& header);
	void setButton(int flag);
	void insertData(std::vector<Day_Flight>& data);
	void readCity(const std::string& fileName);

	void show() override;				//重写虚函数
	void drawTableGrid();				//绘制表格
	void drawButton();					//绘制按钮
	void drawTableData();				//绘制数据
	void drawHeader();					//绘制表头
	int dich(int l, int r, int x);
	static std::vector<std::string> split(std::string str, char separator = '\t');

	void updatePage();		//更新页数据

	void eventLoop(const ExMessage& msg);

	//void update_data(size_t num);	//编辑按钮实现函数
	void delete_data(size_t num);	//删除按钮实现函数

	void setToday();
	int find_showday();		//返回m_datas中和show_day相同日期的下标
	void refreshOneWeek();
	void refreshTable();	//用于重置数组的排序、排序按钮的选择状态、上面七个按钮的选择状态

public://优化系统
	bool open = false;	//表格是否被开启过
	bool week_change = true;

private://优化系统
	void refreshAfterClicked();

public:
	std::map<std::string, std::size_t> mp_city;
	std::vector<std::string> m_city;
	size_t citySum[26];

private:
	int m_rows;		//行数
	int m_cols;		//列数

	int m_gridW;	//格子宽度
	int m_gridH;	//格子高度

	int m_tw;		//文字宽度
	int m_th;		//文字高度

	std::vector<std::string> m_header;
	std::vector<std::string> vec_header;
	std::vector<Day_Flight>* m_datas;//复制一份vec_all
	std::vector<std::vector<std::string> > output_data;

	time_t today;		//现在的时间
	tm today_tm;		//现在的时间的tm格式
	int m_today;		//标记今天的时间对应下标

	time_t show_day;	//现在展示的日期
	time_t sun_day;		//周日下标对应的time_t格式，备选
	int m_showday;		//用于标记现在展示的数据是哪天的（m_datas的下标）
	int m_sunday;		//用于标记当前上方七个按钮展示的周日的下标

	int select_btn;		//目前上方七个按钮显示被选中的下标
	int select_sortbtn;	//目前选中的排序按钮，无默认值

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
	std::vector<PushButton*> m_editBtn;	//修改按钮
	std::vector<PushButton*> m_deleteBtn;	//删除按钮
	std::vector<PushButton*> m_sortBtn;	//排序按钮
	std::vector<PushButton*> m_screenBtn;//筛选按钮
	std::vector<std::vector<PushButton*> > m_editAllBtn;//每个格子的编辑按钮
	std::vector<bool> select_edit;//哪一排被点击了编辑
	PushButton* m_takeoffBtn;	//只看未起飞的航班
	bool select_takeoff = false;//是否筛选未起飞航班
	PushButton* m_insertBtn;	//录入航班
	bool select_insert = false;
	std::vector<PushButton*> m_insertRowBtn;//每行一个插入航班按钮
	PushButton* m_numberBtn;
	bool select_number = false;
	char number[1024] = { 0 };
};

