#pragma once
#include "BasicWidget.h"
#include "Configure.h"
class PushButton :
    public BasicWidget      //�̳л���
{
public:
    PushButton(const std::string& text = "Button", int x = 0, int y = 0, int w = 100, int h = 30);  //Ĭ�ϸ���ֵ
    void show() override;       //��д�麯��
    bool isin();    //����Ƶ���ť��
    bool isClicked(); //�����ť
    bool isSelected();//ѡ�а�ť
    bool isCant();

    void eventLoop(const ExMessage& msg);
    void setBackgroundColor(COLORREF c);//���ñ�����ɫ
    void setHoverColor(COLORREF c);//����ѡ����ɫ
    void setName(const std::string& text);//�޸���ʾ����
    void setSelected(bool slt);//�޸�ѡ��״̬
    void setCant(bool slt);
    void moveText(int x, int y);

    std::string getName();

private:
    std::string m_text;//��ť��ʾ�ַ���
    ExMessage m_msg;
    bool selected;
    bool cantslt;
    int t_x = 0;
    int t_y = 0;
private://��ͬ״̬�İ�ť��ɫ
    COLORREF cur_c = RGB(232, 232, 236); //���ڵ���ɫ��Ĭ�Ϻ�����һ��
    COLORREF normal_c = RGB(232, 232, 236); //������Ĭ��Ϊ��
    COLORREF hover_c = RED; //������ڰ�ť����ɫ��Ĭ��Ϊ��
    COLORREF select_c = WHITE;//��ť��ѡ�е���ɫ
    COLORREF cantslt_c = RGB(204, 204, 204);//���ܱ�ѡ�е���ɫ
};

