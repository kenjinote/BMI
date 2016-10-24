#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <stdio.h>

TCHAR szClassName[] = TEXT("Window");

double GetDlgItemDouble(HWND hDlg, int nIDDlgItem)
{
	double d = 0.0;
	const HWND hWnd = GetDlgItem(hDlg, nIDDlgItem);
	if (hWnd)
	{
		const int nSize = GetWindowTextLength(hWnd);
		if (nSize)
		{
			LPTSTR lpszText = (LPTSTR)GlobalAlloc(0, (nSize + 1) * sizeof(TCHAR));
			if (lpszText)
			{
				GetWindowText(hWnd, lpszText, nSize + 1);
				d = _wtof(lpszText);
				GlobalFree(lpszText);
			}
		}
	}
	return d;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("STATIC"), TEXT("身長(&T):"), WS_VISIBLE | WS_CHILD, 10, 10, 64, 22, hWnd, 0, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("160"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 74, 10, 64, 22, hWnd, (HMENU)100, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("cm"), WS_VISIBLE | WS_CHILD, 74 + 64, 10, 32, 22, hWnd, 0, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("体重(&W):"), WS_VISIBLE | WS_CHILD, 10, 50, 64, 22, hWnd, 0, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("50"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 74, 50, 64, 22, hWnd, (HMENU)101, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("kg"), WS_VISIBLE | WS_CHILD, 74 + 64, 50, 32, 22, hWnd, 0, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("&BMI ="), WS_VISIBLE | WS_CHILD, 10, 130, 64, 22, hWnd, 0, (LPCREATESTRUCT(lParam))->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_READONLY, 74, 130, 64, 22, hWnd, (HMENU)102, (LPCREATESTRUCT(lParam))->hInstance, 0);
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(100, EN_CHANGE), 0);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && (LOWORD(wParam) == 100 || LOWORD(wParam) == 101))
		{
			const double height = GetDlgItemDouble(hWnd, 100);
			const double weight = GetDlgItemDouble(hWnd, 101);
			if (height == 0.0 || weight == 0.0)
			{
				SetDlgItemText(hWnd, 102, 0);
				return 0;
			}
			double t = height / 100.0;
			double w = weight;
			w /= t*t;
			TCHAR szTemp[16];
			swprintf_s(szTemp, 16, L"%g", w);
			SetDlgItemText(hWnd, 102, szTemp);
			return 1;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("BMI 標準体重計算"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
