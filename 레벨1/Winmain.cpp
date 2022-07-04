// 2009/09/08
// nonezero@kumoh.ac.kr
// Win32 API 기반 윈도우 프로그래밍 시작
// 윈도우 프로그래밍 = 이벤트기반 프로그래밍
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

long total_days(int Year, int Month, int Day); // 가입년월 까지의 경과일 계산
long total_days_current(tm* d); // 현재까지의 경과일 계산
int Get_Priority(int A, int B, int C); // 우선순위 계산
void File_Open(); // 파일 읽어오기, 클래스에 값 저장
void Swap(int index1, int index2); 
void Sort(); // 우선순위에 따른 정렬
void File_Save(); // 정렬된 파일 저장 및 생성
int File_Append(char* name1, char* point1, char* Y, char* M, char* D); // 고객 추가
vector<int> File_Find(char* str);// 이름을 통해 값 찾기
int Get_ListSize(); // 벡터 크기 반환
Customer Get_Customer(int t); // customer클래스 반환
void File_Delete(char* str); // 고객 삭제

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS WndClass;
	char szAppName[] = "레벨1_항공사우선순위계산";
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
		hBtn1 = CreateWindow("BUTTON", "정렬파일실행", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 20, 350, 100, 60, hWnd,
			(HMENU)222, _hInstance, NULL);
		hBtn2 = CreateWindow("BUTTON", "원본파일실행", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 160, 350, 100, 60, hWnd,
			(HMENU)333, _hInstance, NULL);

		hbtn1_btn = CreateWindow("BUTTON", "실행", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 35, 50, 30, hWnd,
			(HMENU)44444, _hInstance, NULL);
		hbtn1_edt1 = CreateWindow("EDIT", "홍길동", WS_VISIBLE | WS_CHILD | WS_BORDER
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


		hbtn2_btn = CreateWindow("BUTTON", "실행", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 140, 50, 30, hWnd,
			(HMENU)55555, _hInstance, NULL);
		hbtn2_edt1 = CreateWindow("EDIT", "홍길동", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, Button_x,180, 250, 30, hWnd,
			(HMENU)5555, _hInstance, NULL);

		hbtn3_btn = CreateWindow("BUTTON", "실행", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Button_x + 100, 260, 50, 30, hWnd,
			(HMENU)77, _hInstance, NULL);
		hbtn3_edt = CreateWindow("EDIT", "홍길동", WS_VISIBLE | WS_CHILD | WS_BORDER
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
			SetWindowText(hEdt1, "정렬결과출력 !");
		}
		if (id == 333) {
			ShellExecute(NULL, ("open"), ("레벨1.3 대기자명단.txt"), NULL, NULL, SW_SHOW);
			SetWindowText(hEdt1, "원본파일출력 !");
		}
		if (id == 44444) {
			SetWindowText(hEdt1, "!!!  고객 추가 성공 !!!"); //김동하 12345 2020-1-22
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
				SetWindowText(hEdt1, "!!! 검색성공 !!!\r\n");
				for (int i = 0; i < t.size(); i++) {
					Customer customerList = Get_Customer(t[i]);
					sprintf(szMsg, "\r\n--- 검색결과 %d --- \r\n 예약순번 : %d	이름 : %s \r\n 포인트 : %d	가입일 : %s\r\n",
						i+1,customerList.num, customerList.name,
						customerList.point, customerList.YMD);

					int nLen = GetWindowTextLength(hEdt1);
					SendMessage(hEdt1, EM_SETSEL, nLen, nLen);
					SendMessage(hEdt1, EM_REPLACESEL, FALSE, (LPARAM)szMsg);
				}
			}
			else {
				SetWindowText(hEdt1, "!!! 검색실패 !!!\r\n 고객이 없습니다");
			}			
		}	

		if (id == 77) {
			char szMsg[1024];
			char tmp[1024];
			GetWindowText(hbtn3_edt, szMsg, 1024);
			GetWindowText(hbtn3_edt, tmp, 1024);
			vector<int> t = File_Find(szMsg);

			if (t.size() > 0) {
				SetWindowText(hEdt1, "삭제 성공 !\r\n");
				Customer customerList = Get_Customer(t[0]);
				sprintf(szMsg, "\r\n!!! 마지막 접수된 분 부터 삭제됩니다 !!!\r\n--- 삭제된 고객 --- \r\n 예약순번 : %d	이름 : %s \r\n 포인트 : %d	가입일 : %s\r\n",
						customerList.num, customerList.name,
						customerList.point, customerList.YMD);
					File_Delete(tmp);
			int nLen = GetWindowTextLength(hEdt1);
			SendMessage(hEdt1, EM_SETSEL, nLen, nLen);
			SendMessage(hEdt1, EM_REPLACESEL, FALSE, (LPARAM)szMsg);		
				
			}
			else {
				SetWindowText(hEdt1, "!!!! 삭제 실패 !!!\r\n 다시 입력해주세요~");
			}
			
			
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); 
		SetTextColor(hdc, 0x646EFF);
		TextOut(hdc, Button_x, 45, TEXT("고객  추가"),sizeof("고객 추가"));
		TextOut(hdc, Button_x,145, TEXT("고객  검색"), sizeof("고객 추가"));
		TextOut(hdc, Button_x, 260, TEXT("고객  삭제"), sizeof("고객 추가"));
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}
