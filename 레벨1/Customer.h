#pragma once

#include <string.h>
#include <stdlib.h>
#include <sstream> 
#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>

class Customer {
public:
	int num;
	char name[6];
	int point;
	char YMD[10];
	int Year;
	int Month;
	int Day;
	int day_gap;
	float Priority;
	int B_Max;
	int C_Max;
	Customer() {

	}

	Customer(int a,
		char* b,
		int c,
		char* d,
		int e,
		int f,
		int g,
		int h,
		float i,
		int j,
		int k) {
		num = a;
		strcpy(name, b);
		point = c;
		strcpy(YMD, d);
		Year = e;
		Month = f;
		Day = g;
		day_gap = h;
		Priority = i;
		B_Max = j;
		C_Max = k;
	}
	bool operator==(const Customer& c) {
		return (!(strcmp(this->name, c.name)));
	}
};
