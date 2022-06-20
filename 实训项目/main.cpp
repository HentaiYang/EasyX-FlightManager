#include <conio.h>
#include <iostream>
#include "Management.h"
#include "Window.h"
#include "Table.h"

int main() 
{
	Window w(1000, 680, EW_SHOWCONSOLE | EW_NOCLOSE);
	w.setWindowTitle("��Ʊ����ϵͳ");
	
	Management m;
	m.run();
	return w.exec();
}

using namespace std;
const int YEAR = 2021;//��һ����ꡢ�¡���
const int MONTH = 12;
const int DAY = 20;
const int DAY_COUNT = 2000;//��������
const int DAY_FLIGHT_LOW = 200;//ÿ�캽������
const int DAY_FLIGHT_UP = 400;//ÿ�캽������
int main11()
{
	ifstream f("./images/diming.txt", ios::in);// ��ȡ�����ļ�
	if (!f)//�ж��ܷ�ɹ���
	{
		cout << "��ʧ��" << endl;
		return 0;
	}
	string ss[1024];//���������
	int k = 0;//�����±�
	string s;//��ȡ�ļ��еĵ���
	while (f >> s)//������ȡ�ļ�ֱ������
		ss[k++] = s;//�������±�һһ��Ӧ
	f.close();//�ر��ļ�
	ofstream ff("./images/flight.txt", ios::out);//�����ļ���
	if (!ff)//�ж��Ƿ�򿪳ɹ�
	{
		cout << "���ļ�ʧ��" << endl;
		return 0;
	}
	k = 1;//�����
	time_t now;
	time_t today;
	tm tdtm;
	tm t;//��ǰʱ��
	time(&now);
	today = now;
	t = *localtime(&now);
	tdtm = t;
	t.tm_year = YEAR - 1900;
	t.tm_mon = MONTH - 1;
	t.tm_mday = DAY;
	srand(time(NULL));//�Ե�ǰʱ��Ϊ�������������
	ff << "���\t���ʱ��\t���վ\t�յ�վ\t����ʱ��\t�����\t�Ѷ�Ʊ\t�Ƿ����\t�۸�" << endl;//д���ͷ
	for (int i = 0; i < DAY_COUNT; i++, now += 86400)//DAY_COUNT Ϊ�������ݵ�����
	{

		t.tm_sec = 0;

		ff << t.tm_year + 1900 << " " << t.tm_mon + 1 << " " << t.tm_mday << " ";//д������
		int max_day = 0;//ÿ�����󺽰�����
		while (max_day < DAY_FLIGHT_LOW)
			max_day = rand() % DAY_FLIGHT_UP;
		ff << max_day << endl;//����ÿ��ĺ�������
		for (int j = 1; j <= max_day; j++)//����ÿһ�����������ȡ����
		{
			ff << k << '\t';//д�뺽���
			k++;
			int hour = rand() % 24;//�����ȡСʱ
			if (hour >= 0 && hour <= 9)
				ff << "0" << hour;
			else
				ff << hour;//ǰ��0��д��Сʱ
			int minute = rand() % 6;
			ff << ":" << minute << "0" << '\t';//�����ȡ���ӣ�д��
			t.tm_hour = hour;
			t.tm_min = minute * 10;
			now = mktime(&t);
			int idx = rand() % 251;
			int idx1 = rand() % 251;//��ȡ��������±�
			if (idx1 == idx)//�����±���ͬ
			{
				if (idx1 <= 250)
					idx1++;
				else
					idx1--;
			}
			ff << ss[idx] << '\t' << ss[idx1] << '\t';//д�뺽�����ʼվ���յ�վ
			int ptime = rand() % 8;
			if (ptime == 0)
				ptime++;
			ff << ptime << '\t';//�����ȡ����ʱ�䣬д��
			int max_ticket = rand() % 251;
			if (max_ticket <= 50) max_ticket += 50;
			int now_ticket = rand() % max_ticket;
			ff << max_ticket << '\t' << now_ticket << '\t';//��ȡ�Ʊ��������Ʊ����д��
			if (now > today) ff << "��" << '\t';
			else ff << "��" << '\t';//�ж��Ƿ���ɣ�д��
			int ticket = rand() % 4001;
			if (ticket <= 100)
				ticket += 100;
			ff << ticket << endl;//����Ʊ�ۣ�д��
		}
		now = mktime(&t);
		now += 86400;
		t = *localtime(&now);
	}
	ff.close();//�ر��ļ�
	return 0;
}