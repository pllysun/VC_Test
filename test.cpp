#include <windows.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HMENU hmenu;
HINSTANCE hins;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int
	iCmdShow) {
	static TCHAR szAppName[] = TEXT("MyWindows");
	HWND hwnd;
	MSG msg;
	hins = hInstance;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(101);
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("这个程序需要在 Windows 才能执行！"), szAppName,
			MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName,
		TEXT("文本编辑器"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//绘图类型
	enum
	{
		TYPE_1SLINE,
		TYPE_2SLINE,
		TYPE_3SLINE,
		TYPE_1XLINE,
		TYPE_2XLINE,
		TYPE_3XLINE,
		TYPE_RECTR,
		TYPE_RECTG,
		TYPE_RECTB,
		TYPE_RE
	};
	//PInfo用来保存窗口中所有图形
	typedef struct tagPaintInfo
	{
		POINT ptBegin;
		POINT ptEnd;
		int nType;
		tagPaintInfo* pNext;
	}PInfo;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static POINT ptBegin;
	static POINT ptEnd;
	static int nTypy = TYPE_1SLINE;
	static bool bFlag = true;
	static PInfo* pHead = NULL;
	static PInfo* pEnd = NULL;
	static PInfo* pNew = NULL;
	PInfo* pTemp = NULL;
	HPEN HP;
	HBRUSH HB;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		ptBegin.x = LOWORD(lParam);
		ptBegin.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		ptEnd.x = LOWORD(lParam);
		ptEnd.y = HIWORD(lParam);
		if (bFlag)
		{
			pNew = (PInfo*)malloc(sizeof(PInfo));
			pNew->ptBegin = ptBegin;
			pNew->ptEnd = ptEnd;
			pNew->nType = nTypy;
			pNew->pNext = NULL;
			pHead = pNew;// 因为是第一个节点，所以新建的节点即是头结点
			pEnd = pHead;// 因为是第一个节点，所以头结点即是尾节点
			bFlag = false;
		}
		else
		{
			// 创建一个新节点
			pNew = (PInfo*)malloc(sizeof(PInfo));
			pNew->ptBegin = ptBegin;
			pNew->ptEnd = ptEnd;
			pNew->nType = nTypy;
			pNew->pNext = NULL;
			pEnd->pNext = pNew;// 将创建的新节点 连接到 链表末尾
			pEnd = pNew;// 链表的尾节点改变（变为新节点）
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case ID_1SL:
			nTypy = TYPE_1SLINE;
			break;
		case ID_2SL:
			nTypy = TYPE_2SLINE;
			break;
		case ID_3SL:
			nTypy = TYPE_3SLINE;
			break;
		case ID_1XL:
			nTypy = TYPE_1XLINE;
			break;
		case ID_2XL:
			nTypy = TYPE_2XLINE;
			break;
		case ID_3XL:
			nTypy = TYPE_3XLINE;
			break;
		case ID_R:
			nTypy = TYPE_RECTR;
			break;
		case ID_G:
			nTypy = TYPE_RECTG;
			break;
		case ID_B:
			nTypy = TYPE_RECTB;
			break;
		case ID_RE:
			nTypy = TYPE_RE;
			break;
		case ID_ABOUT:
				DialogBox(hins, MAKEINTRESOURCE(IDD_DIALOG1), hwnd,AboutDlgProc);
				break;
			//case IDM_EXIT:
			//	DestroyWindow(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		pTemp = pHead;
		while (pTemp)
		{
			ptBegin = pTemp->ptBegin;
			ptEnd = pTemp->ptEnd;
			switch (pTemp->nType)
			{
			case TYPE_1SLINE:
				HP = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_2SLINE:
				HP = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_3SLINE:
				HP = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_1XLINE:
				HP = CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_2XLINE:
				HP = CreatePen(PS_DASHDOT, 2, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_3XLINE:
				HP = CreatePen(PS_DASHDOT, 3, RGB(0, 0, 0));
				SelectObject(hdc, HP);
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_RECTR:
				HB = CreateSolidBrush(RGB(255,0 , 0));
				SelectObject(hdc, HB);
				Rectangle(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			case TYPE_RECTG:
				HB = CreateSolidBrush(RGB(0,255,0));
				SelectObject(hdc, HB);
				Rectangle(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			case TYPE_RECTB:
				HB = CreateSolidBrush(RGB(0, 0,255));
				SelectObject(hdc, HB);
				Rectangle(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			case TYPE_RE:
				HB = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, HB);
				Ellipse(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			default:
				break;
			}
			pTemp = pTemp->pNext;
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		while (pHead)
		{
			pTemp = pHead->pNext;
			free(pHead);
			pHead = pTemp;
		}
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}