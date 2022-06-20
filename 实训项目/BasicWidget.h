#pragma once
//同一个窗口的类都可以继承这个类
class BasicWidget
{
public:
	BasicWidget(int x, int y, int w, int h);
	virtual ~BasicWidget(){}//虚析构函数
	int width();
	int height();
	void setFixedSize(int w, int h);

	int x();
	int y();
	void move(int x, int y);

	virtual void show() = 0;//虚函数 不允许实例化

protected:
	int m_x;
	int m_y;
	int m_w;
	int m_h;
};

