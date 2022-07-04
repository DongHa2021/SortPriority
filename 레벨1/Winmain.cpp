// 2009/09/08
// nonezero@kumoh.ac.kr
// Win32 API ��� ������ ���α׷��� ����
// ������ ���α׷��� = �̺�Ʈ��� ���α׷���
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <sstream> 
#include <iostream>
#include <vector>
#include <windows.h> 
#include"Customer.h"
#define Button_x 350
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE _hInstance;

using namespace std;

long total_days(int Year, int Month, int Day); // ���Գ�� ������ ����� ���
long total_days_current(tm* d); // ��������� ����� ���
int Get_Priority(int A, int B, int C); // �켱���� ���
void File_Open(); // ���� �о����, Ŭ������ �� ����
void Swap(int index1, int index2); 
void Sort(); // �켱������ ���� ����
void File_Save(); // ���ĵ� ���� ���� �� ����
int File_Append(char* name1, char* point1, char* Y, char* M, char* D); // �� �߰�
vector<int> File_Find(char* str);// �̸��� ���� �� ã��
int Get_ListSize(); // ���� ũ�� ��ȯ
Customer Get_Customer(int t); // customerŬ���� ��ȯ
void File_Delete(char* str); // �� ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS WndClass;
	char szAppName[] = "����1_�װ���켱�������";
	WndClass.style = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = szAppName;

	if (!RegisterClass(&WndClass)) return NULL;

	hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdt1,hBtn1, hBtn2, 
		hbtn1, hbtn2, 
		hbtn1_edt1,hbtn1_edt2, hbtn1_edt3, hbtn1_edt4, hbtn1_edt5,
		hbtn2_edt1,
		hbtn1_btn,hbtn1_btn2,
		hbtn2_btn, hbtn2_btn2,
		hbtn3_btn, hbtn3_edt;

	HDC hdc;
	HANDLE  hFile;
	
	DWORD  dwRead;
	PAINTSTRUCT ps;
	RECT rt;


	switch (mesg)
	{
	case WM_CREATE:
		File_Open();
		Sort();
		File_Save();
		hEdt1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD |WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, 330, 300, hWnd,
			(HMENU)111, _hInstance, NULL);
		hBtn1 = CreateWindow("BUTTON", "�������Ͻ���", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 20, 350, 100, 60, hWnd,
			(HMENU)222, _hInstance, NULL);
		hBtn2 = CreateWindow("BUTTON", "�������Ͻ���", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 160, 350, 100, 60, hWnd,
			(HMENU)333, _hInstance, NULL);

		hbtn1_btn = CreateWindow("BUTTON", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 35, 50, 30, hWnd,
			(HMENU)44444, _hInstance, NULL);
		hbtn1_edt1 = CreateWindow("EDIT", "ȫ�浿", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x, 75, 75, 30, hWnd,
			(HMENU)4444, _hInstance, NULL);
		hbtn1_edt2 = CreateWindow("EDIT", "11111", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x+80, 75, 75, 30, hWnd,
			(HMENU)4444, _hInstance, NULL);
		hbtn1_edt3 = CreateWindow("EDIT", "1900", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x+160, 75, 75, 30, hWnd,
			(HMENU)4444, _hInstance, NULL);
		hbtn1_edt4 = CreateWindow("EDIT", "01", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x+240, 75, 75, 30, hWnd,
			(HMENU)4444, _hInstance, NULL);
		hbtn1_edt5 = CreateWindow("EDIT", "01", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x+320, 75, 75, 30, hWnd,
			(HMENU)4444, _hInstance, NULL);


		hbtn2_btn = CreateWindow("BUTTON", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 140, 50, 30, hWnd,
			(HMENU)55555, _hInstance, NULL);
		hbtn2_edt1 = CreateWindow("EDIT", "ȫ�浿", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x,180, 250, 30, hWnd,
			(HMENU)5555, _hInstance, NULL);

		hbtn3_btn = CreateWindow("BUTTON", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 260, 50, 30, hWnd,
			(HMENU)77, _hInstance, NULL);
		hbtn3_edt = CreateWindow("EDIT", "ȫ�浿", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x, 300, 250, 30, hWnd,
			(HMENU)7777, _hInstance, NULL);		
		break;

	case WM_COMMAND:
		int id;
		id = LOWORD(wParam);
		Sort();
		File_Save();

		if (id == 222) {
			ShellExecute(NULL, ("open"), ("Result.txt"), NULL, NULL, SW_SHOW);
			SetWindowText(hEdt1, "���İ����� !");
		}
		if (id == 333) {
			ShellExecute(NULL, ("open"), ("����1.3 ����ڸ��.txt"), NULL, NULL, SW_SHOW);
			SetWindowText(hEdt1, "����������� !");
		}
		if (id == 44444) {
			SetWindowText(hEdt1, "!!!  �� �߰� ���� !!!"); //�赿�� 12345 2020-1-22
			char szMsg1[10];
			char szMsg2[10];
			char szMsg3[10];
			char szMsg4[10];
			char szMsg5[10];
				GetWindowText(hbtn1_edt1, szMsg1, 10);
				GetWindowText(hbtn1_edt2, szMsg2, 10);
				GetWindowText(hbtn1_edt3, szMsg3, 10);
				GetWindowText(hbtn1_edt4, szMsg4, 10);
				GetWindowText(hbtn1_edt5, szMsg5, 10);

				File_Append(szMsg1, szMsg2, szMsg3, szMsg4, szMsg5);		
		}
				

		if (id == 55555) {
			char szMsg[1024];
			GetWindowText(hbtn2_edt1, szMsg, 1024);
			vector<int> t = File_Find(szMsg);
			
			if (t.size() > 0) {
				SetWindowText(hEdt1, "!!! �˻����� !!!\r\n");
				for (int i = 0; i < t.size(); i++) {
					Customer customerList = Get_Customer(t[i]);
					sprintf(szMsg, "\r\n--- �˻���� %d --- \r\n ������� : %d	�̸� : %s \r\n ����Ʈ : %d	������ : %s\r\n",
						i+1,customerList.num, customerList.name,
						customerList.point, customerList.YMD);

					int nLen = GetWindowTextLength(hEdt1);
					SendMessage(hEdt1, EM_SETSEL, nLen, nLen);
					SendMessage(hEdt1, EM_REPLACESEL, FALSE, (LPARAM)szMsg);
				}
			}
			else {
				SetWindowText(hEdt1, "!!! �˻����� !!!\r\n ���� �����ϴ�");
			}			
		}	

		if (id == 77) {
			char szMsg[1024];
			char tmp[1024];
			GetWindowText(hbtn3_edt, szMsg, 1024);
			GetWindowText(hbtn3_edt, tmp, 1024);
			vector<int> t = File_Find(szMsg);

			if (t.size() > 0) {
				SetWindowText(hEdt1, "���� ���� !\r\n");
				Customer customerList = Get_Customer(t[0]);
				sprintf(szMsg, "\r\n!!! ������ ������ �� ���� �����˴ϴ� !!!\r\n--- ������ �� --- \r\n ������� : %d	�̸� : %s \r\n ����Ʈ : %d	������ : %s\r\n",
						customerList.num, customerList.name,
						customerList.point, customerList.YMD);
					File_Delete(tmp);
			int nLen = GetWindowTextLength(hEdt1);
			SendMessage(hEdt1, EM_SETSEL, nLen, nLen);
			SendMessage(hEdt1, EM_REPLACESEL, FALSE, (LPARAM)szMsg);		
				
			}
			else {
				SetWindowText(hEdt1, "!!!! ���� ���� !!!\r\n �ٽ� �Է����ּ���~");
			}
			
			
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); 
		SetTextColor(hdc, 0x646EFF);
		TextOut(hdc, Button_x, 45, TEXT("��  �߰�"),sizeof("�� �߰�"));
		TextOut(hdc, Button_x,145, TEXT("��  �˻�"), sizeof("�� �߰�"));
		TextOut(hdc, Button_x, 260, TEXT("��  ����"), sizeof("�� �߰�"));
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}
