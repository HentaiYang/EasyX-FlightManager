#pragma once
#include "BasicWidget.h"
#include "Configure.h"
class PushButton :
    public BasicWidget      //继承基类
{
public:
    PushButton(const std::string& text = "Button", int x = 0, int y = 0, int w = 100, int h = 30);  //默认赋初值
    void show() override;       //重写虚函数
    bool isin();    //光标移到按钮上
    bool isClicked(); //点击按钮
    bool isSelected();//选中按钮
    bool isCant();

    void eventLoop(const ExMessage& msg);
    void setBackgroundColor(COLORREF c);//设置背景颜色
    void setHoverColor(COLORREF c);//设置选中颜色
    void setName(const std::string& text);//修改显示文字
    void setSelected(bool slt);//修改选中状态
    void setCant(bool slt);
    void moveText(int x, int y);

    std::string getName();

private:
    std::string m_text;//按钮显示字符串
    ExMessage m_msg;
    bool selected;
    bool cantslt;
    int t_x = 0;
    int t_y = 0;
private://不同状态的按钮颜色
    COLORREF cur_c = RGB(232, 232, 236); //现在的颜色，默认和正常一样
    COLORREF normal_c = RGB(232, 232, 236); //正常，默认为灰
    COLORREF hover_c = RED; //光标落在按钮上颜色，默认为红
    COLORREF select_c = WHITE;//按钮被选中的颜色
    COLORREF cantslt_c = RGB(204, 204, 204);//不能被选中的颜色
};

