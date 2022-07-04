#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <sstream> 
#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>
#include "Customer.h"

using namespace std;

vector<Customer> customerList;

long total_days(int Year, int Month, int Day) {
	int mon[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	int day = 0;
	int i;

	for (i = 1; i < Year; i++) {
		day += (i % 4 == 0 && i % 100 != 0 || i % 400 == 0) ? 366 : 365;
	}
	if (Year % 4 == 0 && Year % 100 != 0
		|| Year % 400 == 0) mon[2]++;
	for (i = 1; i < Month; i++) {
		day += mon[i];
	}
	return day + Day;
}

long total_days_current(tm* d) {
	int mon[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	int day = 0;
	int i;

	for (i = 1; i < d->tm_year + 1900; i++) {
		day += (i % 4 == 0 && i % 100 != 0 || i % 400 == 0) ? 366 : 365;
	}
	if ((d->tm_year + 1900) % 4 == 0 && (d->tm_year + 1900) % 100 != 0
		|| (d->tm_year + 1900) % 400 == 0) mon[2]++;
	for (i = 1; i < (d->tm_mon + 1); i++) {
		day += mon[i];
	}
	return day + (d->tm_mday);
}

float Get_Priority(int A, int B, int C, float B_Max, float C_Max) {
	float result;
	float tmp1 = B / B_Max;
	float tmp2 = C / C_Max;
	result = A - (tmp1) * 10 - (tmp2) * 5;
	return result;
}

void File_Open() {
	FILE* fp = NULL;
	char buff[100];
	time_t curr;
	struct tm* d;
	curr = time(NULL);
	d = localtime(&curr);
	static int count = 0;
	fp = fopen("레벨1.3 대기자명단.txt", "r");

	if (fp == NULL) {
		printf("입력파일을 열수 없습니다.");
	}

	int B_MAX = 199148;
	int C_MAX = 9436;
	if (fp != NULL) {
		while (!feof(fp)) {

			fgets(buff, 100, fp);

			int num;
			char name[32];
			int point;
			char YMD[32];
			int year;
			int month;
			int day;
			int day_gap;
			float priority;
			char tmp[32];


			char* ptr = strtok(buff, "\t");
			num = atoi(ptr);
			ptr = strtok(NULL, "\t");
			strcpy(name, ptr);
			ptr = strtok(NULL, "\t");
			point = atoi(ptr);
			ptr = strtok(NULL, "\t");
			strcpy(YMD, ptr);

			strcpy(tmp, YMD);
			ptr = strtok(tmp, "-");
			year = atoi(ptr);
			ptr = strtok(NULL, "-");
			month = atoi(ptr);
			ptr = strtok(NULL, "-");
			day = atoi(ptr);

			for (int i = 0; YMD[i] != 0; i++) {
				if (YMD[i] == '\n') {
					YMD[i] = 0;
					break;
				}
			}

			day_gap = total_days_current(d) - total_days(year, month, day);

			if (B_MAX < point) {
				B_MAX = point;
			}
			if (C_MAX < day_gap) {
				C_MAX = day_gap;
			}
			priority = Get_Priority(num, point, day_gap, B_MAX, C_MAX);

			Customer customer = Customer(num, name, point, YMD,
				year, month, day, day_gap, priority, B_MAX, C_MAX);


			customerList.push_back(customer);

			count++;
			if (count == 600) {
				break;
			}
		}
		fclose(fp);
	}
}

void Swap(int index1, int index2) {
	Customer tmp;
	tmp = customerList[index1];
	customerList[index1] = customerList[index2];
	customerList[index2] = tmp;
}

void Sort() {
	for (int i = customerList.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (customerList[j].Priority < customerList[j + 1].Priority) {
				Swap(j, j + 1);
			}
		}
	}
}

void File_Save() {
	FILE* fp = fopen("Result.txt", "w");
	char result[1000];
	sprintf(result, "우선순위\t접수번호\t이름\t마일리지\t가입년월\t\t가입경과일\n");
	fputs(result, fp);
	for (int i = customerList.size()-1; i >-1 ; i--) {
		sprintf(result, "%d\t%d\t%s\t%d\t%s\t%d \n", 
			customerList.size()-i,
			customerList[i].num,
			customerList[i].name,
			customerList[i].point,
			customerList[i].YMD,
			customerList[i].day_gap);
		fputs(result, fp);   // 파일에 문자열 저장
	}
	fclose(fp);    // 파일 포인터 닫기
}

int File_Append(char* name1, char* point1, char* Y, char* M, char* D) {
	time_t curr;
	struct tm* d;
	curr = time(NULL);
	d = localtime(&curr);

	int num;
	char name[32];
	int point;
	char YMD[32];
	int year;
	int month;
	int day;
	int day_gap;
	float priority;
	int B_MAX = 0;
	int C_MAX = 0;

	num = customerList.size() + 1;
	strcpy(name, name1);
	point = atoi(point1);

	year = atoi(Y);
	month = atoi(M);
	day = atoi(D);

	sprintf(YMD, "%s-%s-%s", Y, M, D);

	day_gap = total_days_current(d) - total_days(year, month, day);

	if (B_MAX < point) {
		B_MAX = point;
	}
	if (C_MAX < point) {
		C_MAX = day_gap;
	}
	priority = Get_Priority(num, point, day_gap, B_MAX, C_MAX);
	Customer customer = Customer(num, name, point, YMD,
		year, month, day, day_gap, priority, B_MAX, C_MAX);

	customerList.push_back(customer);


	return 0;
}

vector<int> File_Find(char* str) {
	vector<int> v;

	for (int i = 0; i < customerList.size(); i++) {
		if (!strcmp(customerList[i].name, str)) {
			v.push_back(i);
		}
	}
	return v;
}

int Get_ListSize() {
	return customerList.size();
}

void File_Delete(char* str) {
	vector<int> v = File_Find(str);
	int tmp = v[0];
	customerList.erase(customerList.begin() + tmp);
}

Customer Get_Customer(int t) {
	Customer customer;
	customer = customerList[t];

	return customer;
}