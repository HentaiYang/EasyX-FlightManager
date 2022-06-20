#include "PushButton.h"

PushButton::PushButton(const std::string& text, int x, int y, int w, int h)
	:BasicWidget(x,y,w,h),m_text(text)	//��������б�
	,selected(false), cantslt(false)//Ĭ��δ��ѡ�У���ѡ��
{

}

void PushButton::show()
{
	setfillcolor(cur_c);	//���ð�����ɫ
	::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 10, 10);//��ť��С

	int tx = m_x + t_x + (m_w - textwidth(m_text.c_str())) / 2;	//ƫ�ƾ���Ϊm_x+���Ե����
	int ty = m_y + t_y + (m_h - textheight(m_text.c_str())) / 2;
	
	::outtextxy(tx, ty, m_text.c_str());//��������
}

bool PushButton::isin()
{
	if (m_msg.x >= m_x && m_msg.x <= m_x + m_w && m_msg.y >= m_y && m_msg.y <= m_y + m_h)
	{
		return true;
	}
	return false;
}

bool PushButton::isClicked()
{
	if (isCant())
	{
		return false;
	}
	if (isin())
	{
		if (m_msg.message == WM_LBUTTONDOWN)
		{
			cur_c = normal_c;
			return true;
		}
	}
	return false;
}

bool PushButton::isSelected()
{
	return selected;
}

bool PushButton::isCant()
{
	return cantslt;
}

void PushButton::eventLoop(const ExMessage& msg)
{
	m_msg = msg;
	if (isCant())//�����ť����ѡ��
	{
		cur_c = cantslt_c;
	}
	else if (isSelected())//��������ť����ѡ��״̬
	{
		cur_c = select_c;
	}
	else if (!isin())//�����겻�ڰ�ť��
	{
		cur_c = normal_c;
	}
	else//����ڰ�ť��
	{
		cur_c = hover_c;
	}
}

void PushButton::setBackgroundColor(COLORREF c)
{
	normal_c = c;
}

void PushButton::setHoverColor(COLORREF c)
{
	hover_c = c;
}

void PushButton::setName(const std::string& text)
{
	m_text = text;
}

void PushButton::setSelected(bool slt)
{
	selected = slt;
}

void PushButton::setCant(bool slt)
{
	cantslt = slt;
}

void PushButton::moveText(int x, int y)
{
	t_x = x;
	t_y = y;
}

std::string PushButton::getName()
{
	return m_text;
}
