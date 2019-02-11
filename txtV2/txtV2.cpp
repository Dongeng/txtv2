// txtV2.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "txtV2.h"

// ȫ�ֱ���: 
HINSTANCE		hInst;								// ��ǰʵ��
MyTxt				_txtV2;
HWND             g_hWnd;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	_txtV2.Init();

	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	/*HDC hdc = GetDC(g_hWnd);

	HDC buffDc = CreateCompatibleDC(hdc);

	ReleaseDC(g_hWnd, hdc);*/
	

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TXTV2));

	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
			//����
		case VK_LEFT:
			_txtV2.moveLeft();
			_txtV2.loseEfficacy(hWnd);
			break;
			//����
		case VK_RIGHT:
			_txtV2.moveRight();
			_txtV2.loseEfficacy(hWnd);
			break;
			//����
		case VK_UP:
			_txtV2.moveUp(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
			//����
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
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_NEWFILE://�½��ļ�
			
			_txtV2.newFile(hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		case ID_OPENFILE://���ļ�
			_txtV2.openFile(_T("1.txt"),hWnd);
			_txtV2.loseEfficacy(hWnd);
			break;
		case ID_SAVEFILE://�����ļ�
			_txtV2.saveFile(_T("1.txt"));
			break;
		case ID_COPY:
			//����
			_txtV2.textCopy(hWnd);
			break;
		case ID_PASTY:
			//ճ��
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
		// TODO:  �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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

