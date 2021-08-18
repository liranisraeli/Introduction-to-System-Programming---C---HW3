#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"


const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };


void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		puts("Enter Flight Date dd/mm/yyyy\t");
		myGets(date, MAX_STR_LEN);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 10)
		return 0;
	if ((date[2] != '/') || (date[5] != '/'))
		return 0;
	sscanf(date, "%d/%d/%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d", pDate->day, pDate->month, pDate->year);
}


int compareDate(const void* pDate1, const void* pDate2) {
	int day1, day2, month1, month2, year1, year2;
	const char* pD1 = (const char*)pDate1;
	const char* pD2 = (const char*)pDate2;
	sscanf(pD1, "%d/%d/%d", &day1, &month1, &year1);
	sscanf(pD2, "%d/%d/%d", &day2, &month2, &year2);
	if (year1 != year2)
		return year1 - year2;
	if (month1 != month2)
		return month1 - month2;
	return day1 - day2;
}

void convertDateToString(const Date* date, char* dateChar) {

	sprintf(dateChar, "%d/%d/%d", date->day, date->month, date->year);

}

int writeDateToBinaryFile(const Date* pDate, FILE* fp) {
	if (!pDate || !fp)
		return 0;
	if (fwrite(&pDate->day, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&pDate->month, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&pDate->year, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int readDateFromBinaryFile(Date* pDate, FILE* fp) {
	if (!pDate || !fp)
		return 0;
	if (fread(&pDate->day, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&pDate->month, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&pDate->year, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

NODE* addDateToList(LIST* list, Date* pDate) {
	char* dateStr = (char*)malloc(sizeof(char)*(DATE_LENGTH + 1));
	if (!dateStr)
		return NULL;
	convertDateToString(pDate, dateStr);
	if (!L_find(list->head.next, dateStr, compareDate))
		return	L_insert_sorted(list, dateStr, compareDate);
	free(dateStr);
	return NULL;
}


