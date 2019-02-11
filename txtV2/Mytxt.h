#pragma once

struct Text
{
	UINT			i;//风格下标
	TCHAR		text;//文字
};

class MyTxt
{
public:
	TCHAR									m_className[100];
	HMENU								hMenu;//当前菜单
private:
	bool										m_isEdit;
	UINT										m_mouseState;//0 正常  1 按下 2 移动并且有选中文字
	UINT										m_CaretX;//当前caret的x坐标
	UINT										m_CaretY;//当前caret的y坐标
	std::list<Text>						m_Text;//txt文件的内容
	std::list<Text>						m_TBuff;//txt未保存的文本内容
	std::list<Text>::iterator			m_CurI;//当前输入位置  //根据此值来保存。通过此值实现左移，右移等操作
	

public:
	void Init();//初始化
	void drawWnd(HWND hWnd);//绘制窗口
	void setClassName(TCHAR* className = _T("*.txt"));//设置类名
	void clearWnd(HWND hWnd);//清屏
	void loseEfficacy(HWND hWnd);//使矩形区域失效 清屏
	void push_back(TCHAR const& c);//插入字符
	void pop_back();//从当前位置删
	void moveDown(HWND hWnd);//下移
	void moveUp(HWND hWnd);//上移
	void moveRight();//右移
	void moveLeft();//左移
	void leftDown(HWND hWnd, short x, short y);//鼠标左键
	void leftUp();//鼠标左键
	void mouseMove(HWND hWnd, short x, short y);//鼠标移动
	void newFile(HWND hWnd);//新建文件
	void openFile(const TCHAR* src, HWND hWnd);//打开文件
	void saveFile(const TCHAR* src);//保存文件
	void checkAll(UINT i = 1);//全选
	void textCopy(HWND hWnd);//复制
	void textPaste(HWND hWnd);//粘贴
};

