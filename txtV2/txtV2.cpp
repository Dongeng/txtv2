// txtV2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "txtV2.h"

// 全局变量: 
HINSTANCE		hInst;								// 当前实例
MyTxt				_txtV2;
HWND             g_hWnd;

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	_txtV2.Init();

	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	/*HDC hdc = GetDC(g_hWnd);

	HDC buffDc = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);*/
	

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TXTV2));

	// 主消息循环: 
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
		}
		
	}
	

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TXTV2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TXTV2);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _txtV2.m_className;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(_txtV2.m_className, _txtV2.m_className, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, _txtV2.hMenu, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			hdc = GetDC(hWnd);
			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			CreateCaret(hWnd, nullptr, 1, tm.tmHeight);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			_txtV2.leftDown(hWnd,x,y);
			_txtV2.loseEfficacy(hWnd);
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			_txtV2.mouseMove(hWnd, x, y);
		}
		break;
	case WM_LBUTTONUP:
		_txtV2.leftUp();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			//左移
		case VK_LEFT:
			_txtV2.moveLeft();
			_txtV2.loseEfficacy(hWnd);
			break;
			//右移
		case VK_RIGHT:
			_txtV2.moveRight();
			_txtV2.loseEfficacy(hWnd);
			break;
			//上移
		case VK_UP:
			_txtV2.moveUp(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
			//下移
		case VK_DOWN:
			_txtV2.moveDown(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		{
			switch (wParam)
			{
			case '\b':
				{
					_txtV2.pop_back();
					_txtV2.loseEfficacy(hWnd);
					break;
				}
			case '\r':
				{
					TCHAR ch = '\r\n';
					_txtV2.push_back(ch);
					_txtV2.loseEfficacy(hWnd);
					break;
				}
			default:
				{
					TCHAR ch = (TCHAR)(wParam);
					_txtV2.push_back(ch);
					_txtV2.loseEfficacy(hWnd);
				}
				break;
			}
		}
		break;
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_NEWFILE://新建文件
			
			_txtV2.newFile(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		case ID_OPENFILE://打开文件
			_txtV2.openFile(_T("1.txt"),hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		case ID_SAVEFILE://保存文件
			_txtV2.saveFile(_T("1.txt"));
			break;
		case ID_COPY:
			//复制
			_txtV2.textCopy(hWnd);
			break;
		case ID_PASTY:
			//粘贴
			_txtV2.textPaste(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		case ID_EXIT:
			DestroyCaret();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		_txtV2.drawWnd(hWnd);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

