#pragma once

struct Text
{
	UINT			i;//����±�
	TCHAR		text;//����
};

class MyTxt
{
public:
	TCHAR									m_className[100];
	HMENU								hMenu;//��ǰ�˵�
private:
	bool										m_isEdit;
	UINT										m_mouseState;//0 ����  1 ���� 2 �ƶ�������ѡ������
	UINT										m_CaretX;//��ǰcaret��x����
	UINT										m_CaretY;//��ǰcaret��y����
	std::list<Text>						m_Text;//txt�ļ�������
	std::list<Text>						m_TBuff;//txtδ������ı�����
	std::list<Text>::iterator			m_CurI;//��ǰ����λ��  //���ݴ�ֵ�����档ͨ����ֵʵ�����ƣ����ƵȲ���
	

public:
	void Init();//��ʼ��
	void drawWnd(HWND hWnd);//���ƴ���
	void setClassName(TCHAR* className = _T("*.txt"));//��������
	void clearWnd(HWND hWnd);//����
	void loseEfficacy(HWND hWnd);//ʹ��������ʧЧ ����
	void push_back(TCHAR const& c);//�����ַ�
	void pop_back();//�ӵ�ǰλ��ɾ
	void moveDown(HWND hWnd);//����
	void moveUp(HWND hWnd);//����
	void moveRight();//����
	void moveLeft();//����
	void leftDown(HWND hWnd, short x, short y);//������
	void leftUp();//������
	void mouseMove(HWND hWnd, short x, short y);//����ƶ�
	void newFile(HWND hWnd);//�½��ļ�
	void openFile(const TCHAR* src, HWND hWnd);//���ļ�
	void saveFile(const TCHAR* src);//�����ļ�
	void checkAll(UINT i = 1);//ȫѡ
	void textCopy(HWND hWnd);//����
	void textPaste(HWND hWnd);//ճ��
};

