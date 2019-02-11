#include "stdafx.h"
#include "Mytxt.h"


//文字风格
struct TxtStyle
{
	COLORREF		m_BkColor;
	COLORREF		m_TextColor;
	int					m_BkMode;
};


TxtStyle style[2] = {
	//未选中
	{
		0xffffff,
		0x000000,
		TRANSPARENT
	},
	//选中
	{
		RGB(51, 135, 255),
		0xffffff,
		OPAQUE
	}
};


void MyTxt::drawWnd(HWND hWnd)
{
	if (!m_isEdit)
	{
		return;
	}
	

	/*
	设置字体
	*/
	HDC hdc = GetDC(hWnd);

	/*
	获得度量单位
	*/
	SIZE psizl;
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	/*
	当前绘制位置
	*/
	UINT x = 0;
	UINT y = 0;
	/*
	开始绘制
	*/
	std::list<Text>::iterator begin = m_TBuff.begin();

	while (begin != m_TBuff.end())
	{
		if (begin == m_CurI)
		{
			m_CaretX = x;
			m_CaretY = y;
		}
		if (begin->text == '\r\n')
		{
			x = 0;
			y += tm.tmHeight;
		}
		else
		{
			
			GetTextExtentPoint32(hdc, &(begin->text), 1, &psizl);
			SetBkColor(hdc, style[begin->i].m_BkColor);
			SetBkMode(hdc, style[begin->i].m_BkMode);
			SetTextColor(hdc, style[begin->i].m_TextColor);
			
			TextOut(hdc, x, y, &(begin->text), 1);
			x += psizl.cx;
		}
		++begin;
	}
	if (m_CurI == begin)
	{
		m_CaretX = x;
		m_CaretY = y;
	}
	SetCaretPos(m_CaretX, m_CaretY);
	ShowCaret(hWnd);
	ReleaseDC(hWnd, hdc);
}

void MyTxt::Init()
{
	setClassName();
	m_CaretX	= 0;
	m_CaretY	= 0;
	m_CurI		= m_TBuff.begin();
	hMenu = CreateMenu();
	m_mouseState = 0;


	//动态菜单
	HMENU menuFile = CreateMenu();
	AppendMenu(menuFile, 0, 667, _T("新建(&N)"));
	AppendMenu(menuFile, 0, 668, _T("打开(&O)"));
	AppendMenu(menuFile, 0, 669, _T("保存(&S)"));
	AppendMenu(menuFile, 0, 670, _T("另存为(&A)"));
	AppendMenu(menuFile, 0, 671, _T("页面设置(&U)"));
	AppendMenu(menuFile, 0, 672, _T("打印(&P)"));
	AppendMenu(menuFile, 0, 673, _T("退出(&X)"));

	AppendMenu(hMenu, MF_POPUP, (UINT)menuFile, _T("文件(&F)"));

	//动态菜单
	HMENU menuEdit = CreateMenu();
	AppendMenu(menuEdit, 0, 674, _T("撤销(&U)"));
	AppendMenu(menuEdit, 0, 675, _T("剪切(&T)"));
	AppendMenu(menuEdit, 0, 676, _T("复制(&C)"));
	AppendMenu(menuEdit, 0, 677, _T("粘贴(&P)"));
	AppendMenu(menuEdit, 0, 678, _T("删除(&L)"));
	AppendMenu(menuEdit, 0, 679, _T("查找(&F)"));
	AppendMenu(menuEdit, 0, 680, _T("查找下一个(&N)"));
	AppendMenu(menuEdit, 0, 681, _T("替换(&R)"));
	AppendMenu(menuEdit, 0, 682, _T("转到(&G)"));
	AppendMenu(menuEdit, 0, 683, _T("全选(&A)"));
	AppendMenu(menuEdit, 0, 684, _T("时间/日期(&D)"));

	AppendMenu(hMenu, MF_POPUP, (UINT)menuEdit, _T("编辑(&E)"));

	//动态菜单
	HMENU menuForm = CreateMenu();
	AppendMenu(menuForm, 0, 685, _T("自动换行(&W)"));
	AppendMenu(menuForm, 0, 686, _T("字体(&F)"));

	AppendMenu(hMenu, MF_POPUP, (UINT)menuForm, _T("格式(&O)"));

	//动态菜单
	HMENU menuSeek = CreateMenu();
	AppendMenu(menuSeek, 0, 687, _T("状态栏(&S)"));

	AppendMenu(hMenu, MF_POPUP, (UINT)menuSeek, _T("查看(&V)"));

	//动态菜单
	HMENU menuHelp = CreateMenu();
	AppendMenu(menuHelp, 0, 688, _T("查看帮助(&H)"));
	AppendMenu(menuHelp, 0, 689, _T("关于记事本(&A)"));

	AppendMenu(hMenu, MF_POPUP, (UINT)menuHelp, _T("帮助(&H)"));
}

void  MyTxt::setClassName(TCHAR* className)
{
	int len = _tcslen(className);
	ZeroMemory(m_className, sizeof(m_className));
	for (int i = 0; i < len && i < 100; ++i)
	{
		m_className[i] = className[i];
	}
}

void MyTxt::clearWnd(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetWindowRect(hWnd, &rect);
	rect.right -= rect.left;
	rect.left = 0;
	rect.bottom -= rect.top;
	rect.top = 0;
	FillRect(hdc, &rect, nullptr);
	ReleaseDC(hWnd, hdc);
}

void MyTxt::leftDown(HWND hWnd, short x, short y)//左键按下
{
	if (!m_isEdit)
	{
		return;
	}
	checkAll(0);
	if (m_mouseState != 1)
	{
		m_mouseState = 1;
	}
	std::list<Text>::iterator temp = m_TBuff.begin();

	int pos_x = 0;
	int pos_y = 0;
	HDC hdc = GetDC(hWnd);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	int y_bottom = 0;
	if (y > tm.tmHeight)
	{
		y_bottom = y - tm.tmHeight;
	}

	SIZE psizl;
	if (y_bottom > 0)//不在第一行
	{
		while (temp != m_TBuff.end())//找哪一行
		{
			if ((temp->text) == '\r\n')
			{
				pos_y += tm.tmHeight;
			}
			++temp;
			if (pos_y >= y_bottom && pos_y <= y)
			{
				break;
			}
		}
	}


	while (temp != m_TBuff.end())//找哪一列
	{
		GetTextExtentPoint32(hdc, &(temp->text), 1, &psizl);
		pos_x += psizl.cx;
		if (pos_x >= x || (temp->text) == '\r\n')
		{
			break;
		}
		++temp;
	}
	m_CurI = temp;
}

void MyTxt::leftUp()//鼠标左键抬起
{
	if (!m_isEdit)
	{
		return;
	}
	
	if (m_mouseState == 1)
	{
		m_mouseState = 2;//
	}
}

void MyTxt::mouseMove(HWND hWnd, short x, short y)//鼠标移动
{
	if (m_isEdit)//进入编辑中
	{
		if (m_mouseState == 1)//鼠标左键
		{
			HDC hdc = GetDC(hWnd);
			SIZE psizl;
			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);

			if (y < m_CaretY)//在上一行
			{
				if (y < m_CaretY + tm.tmHeight)
				{
					if (m_CurI != m_TBuff.begin())
					{
						--m_CurI;
						m_CurI->i = (m_CurI->i + 1) % 2;
						loseEfficacy(hWnd);
					}
				}
			}
			else if (y>m_CaretY+tm.tmHeight)//在下面的行
			{
				if (y > m_CaretY)
				{
					if (m_CurI != m_TBuff.end())
					{
						m_CurI->i = (m_CurI->i + 1) % 2;
						++m_CurI;
						loseEfficacy(hWnd);
					}
				}
			}
			else//同一行
			{
				if (x < m_CaretX)
				{
					if (m_CurI != m_TBuff.begin())
					{
						if (m_CurI != m_TBuff.end())
						{
							if (m_CurI->text != '\r\n')
							{
								--m_CurI;
								m_CurI->i = (m_CurI->i + 1) % 2;
								loseEfficacy(hWnd);
							}
						}
						else
						{
							--m_CurI;
							m_CurI->i = (m_CurI->i + 1) % 2;
							loseEfficacy(hWnd);
						}
					}
				}
				else
				{
					if (m_CurI != m_TBuff.end())
					{
						if (m_CurI->text != '\r\n')
						{
							m_CurI->i = (m_CurI->i + 1) % 2;
							++m_CurI;
							loseEfficacy(hWnd);
						}
					}
				}
			}
			ReleaseDC(hWnd, hdc);
		}
	}
}



//使矩形区域失效
void MyTxt::loseEfficacy(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	rect.right -= rect.left;
	rect.left = 0;
	rect.bottom -= rect.top;
	rect.top = 0;
	InvalidateRect(hWnd, &rect, true);
	HDC hdc = GetDC(hWnd);
	HBRUSH hbr = CreateSolidBrush(0xffffff);
	SelectObject(hdc,hbr);
	FillRect(hdc, &rect, hbr);
	
	DeleteObject(hbr);
	ReleaseDC(hWnd, hdc);
}

void MyTxt::push_back(TCHAR const& c)//根据m_CurI位置插入
{
	if (m_isEdit)
	{
		if (m_mouseState == 2)
		{
			std::list<Text>::iterator begin = m_TBuff.begin();
			std::list<Text>::iterator temp;
			while (begin != m_TBuff.end())
			{

				temp = begin;
				++begin;
				if (temp->i == 1)
				{
					m_TBuff.erase(temp);
					m_CurI = begin;
				}
			}
			if (m_TBuff.size() == 0)
			{
				m_CurI == m_TBuff.begin();
			}
			m_mouseState = 0;
		}
		Text temp = {
			0,
			(c)
		};
		m_TBuff.insert(m_CurI, temp);
	}
}

void MyTxt::pop_back()
{
	if (m_isEdit)
	{
		bool flag = true;
		if (m_mouseState == 2)
		{
			std::list<Text>::iterator begin = m_TBuff.begin();
			std::list<Text>::iterator temp;
			while (begin != m_TBuff.end())
			{
				
				temp = begin;
				++begin;
				if (temp->i == 1)
				{
					flag = false;
					m_TBuff.erase(temp);
					m_CurI = begin;
				}
			}
			if (m_TBuff.size() == 0)
			{
				m_CurI == m_TBuff.begin();
			}
		}
		if (flag)
		{
			std::list<Text>::iterator temp = m_CurI;
			if (m_CurI == m_TBuff.begin())
			{
				return;
			}
			--temp;
			m_TBuff.erase(temp);
		}
	}
}

void MyTxt::moveLeft()
{
	if (m_isEdit)
	{
		checkAll(0);
		if (m_mouseState != 0)
		{
			m_mouseState = 0;
		}
		if (m_CurI != m_TBuff.begin())
		{
			--m_CurI;
		}
	}

}

void MyTxt::moveRight()
{
	if (m_isEdit)
	{
		if (m_CurI != m_TBuff.end())
		{
			checkAll(0);
			if (m_mouseState != 0)
			{
				m_mouseState = 0;
			}
			++m_CurI;
		}
	}
}

void MyTxt::moveUp(HWND hWnd)
{
	if (!m_isEdit)
	{
		return;
	}
	if (m_CaretY == 0)//当前位置在第一行
	{
		return;
	}
	checkAll(0);
	if (m_mouseState != 0)
	{
		m_mouseState = 0;
	}
	while (m_CurI != m_TBuff.begin())//找上一行的结束位置
	{
		--m_CurI;
		if ((m_CurI->text) == '\r\n')
		{
			break;
		}
	}
	std::list<Text>::iterator temp = m_CurI;//记录这个结束位置
	while (m_CurI != m_TBuff.begin())//找上一行的起始位置
	{
		--m_CurI;
		if ((m_CurI->text) == '\r\n')
		{
			break;
		}
	}
	if (m_CurI != m_TBuff.begin())
	{
		m_CurI++;
	}
	HDC hdc = GetDC(hWnd);
	UINT Len = 0;
	SIZE psizl;
	while (m_CurI != temp)//从行中找位置
	{
		if (Len >= m_CaretX)//找到退出
		{
			break;
		}
		GetTextExtentPoint32(hdc, &(m_CurI->text), 1, &psizl);
		Len += psizl.cx;
		++m_CurI;
	}
	ReleaseDC(hWnd, hdc);
}

void MyTxt::moveDown(HWND hWnd)
{
	if (!m_isEdit)
	{
		return;
	}
	std::list<Text>::iterator temp = m_CurI;//用temp找下一行
	checkAll(0);
	if (m_mouseState != 0)
	{
		m_mouseState = 0;
	}
	while (temp != m_TBuff.end())//找下一行的起始位置
	{
		if ((temp->text) == '\r\n')
		{
			break;
		}
		++temp;
	}
	if (temp == m_TBuff.end())//已经在最后一行
	{
		return;
	}
	++temp;
	m_CurI = temp;//有下一行  记录起始位置
	while (temp != m_TBuff.end())//找下一行结束位置
	{
		if ((temp->text) == '\r\n')
		{
			break;
		}
		++temp;
	}

	HDC hdc = GetDC(hWnd);
	UINT Len = 0;
	SIZE psizl;
	while (m_CurI != temp)//从行中找位置
	{
		if (Len >= m_CaretX)//找到退出
		{
			break;
		}
		GetTextExtentPoint32(hdc, &(m_CurI->text), 1, &psizl);
		Len += psizl.cx;
		++m_CurI;
	}
	ReleaseDC(hWnd, hdc);
}

void MyTxt::newFile(HWND hWnd)
{
	m_TBuff.clear();
	m_Text.clear();
	m_CurI = m_TBuff.begin();
	m_CaretX = 0;
	m_CaretY = 0;
	m_isEdit = true;
	HDC hdc = GetDC(hWnd);
	ShowCaret(hWnd);
	ReleaseDC(hWnd, hdc);
}

void MyTxt::openFile(const TCHAR* src,HWND hWnd)
{
	m_isEdit = true;
	m_Text.clear();
	m_TBuff.clear();
	m_CaretX = 0;
	m_CaretY = 0;
	m_CurI = m_TBuff.begin();
	ShowCaret(hWnd);
	std::fstream newFile(src, std::ios::in | std::ios::binary);
	char txt[2];
	char ch;
	TCHAR c;
	while (ch = newFile.get(), !newFile.eof())
	{
		if (ch == '\r')
		{
			c = '\r\n';
			push_back(c);
			ch = newFile.get();
			continue;
		}
		if (ch > 0)
		{
			push_back(ch);
		}
		else
		{
			txt[0] = ch;
			ch = newFile.get();
			txt[1] = ch;
			MultiByteToWideChar(CP_OEMCP, 0, txt, 2, &c, 1);
			push_back(c);
		}
	}
	newFile.close();
}

void MyTxt::saveFile(const TCHAR* src)
{
	if (!m_isEdit)
	{
		return;
	}
	HANDLE hFile = CreateFile(src,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		nullptr,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	DWORD len = 0;
	char text[2];
	int size = (m_TBuff.size() + 1)*2;
	int i = 0;
	char* txt = nullptr;
	txt = new char[size];
	ZeroMemory(txt, size);
	for (std::list<Text>::iterator it = m_TBuff.begin(); it != m_TBuff.end(); ++it)
	{
		ZeroMemory(text, sizeof(text));
		if (it->text == '\r\n')
		{
			strcat(txt, "\r\n");
			i += 2;
			continue;
		}
		WideCharToMultiByte(CP_OEMCP, 0, &(it->text), -1, text, 2, nullptr, FALSE);
		char ch = text[0];
		if (ch > 0)
		{
			txt[i++] = ch;
		}
		if (ch < 0)
		{
			txt[i++] = text[0];
			txt[i++] = text[1];
		}
		
	}
	WriteFile(hFile, txt, i, &len, nullptr);
	CloseHandle(hFile);
}


void MyTxt::checkAll(UINT i)//全选
{
	std::list<Text>::iterator temp = m_TBuff.begin();
	while (temp != m_TBuff.end())
	{
		temp->i = i;
		++temp;
	}
}

void MyTxt::textCopy(HWND hWnd)//复制
{
	std::list<Text>::iterator temp = m_TBuff.begin();
	TCHAR text[1024];
	ZeroMemory(text, sizeof(text));
	UINT textI = 0;
	while (temp != m_TBuff.end())
	{
		if (temp->i == 1)
		{
			text[textI++] = temp->text;
		}
		++temp;
	}
	UINT len = sizeof(text);
	if (textI == 0)
	{
		return;
	}
	HANDLE hdata = GlobalAlloc(GMEM_MOVEABLE, len);
	LPVOID lptstrCopy = GlobalLock(hdata);
	memcpy(lptstrCopy, text, sizeof(text));
	GlobalUnlock(hdata);
	OpenClipboard(hWnd);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hdata);
	CloseClipboard();
}

void MyTxt::textPaste(HWND hWnd)//粘贴
{
	
	
	OpenClipboard(hWnd);
	HANDLE hdata = nullptr;
	hdata = GetClipboardData(CF_UNICODETEXT);
	TCHAR* pData = (TCHAR*)GlobalLock(hdata);
	int len = _tcslen(pData);
	for (size_t i = 0; i < len; i++)
	{
		push_back(pData[i]);
	}
	GlobalUnlock(hdata);
	CloseClipboard();
}