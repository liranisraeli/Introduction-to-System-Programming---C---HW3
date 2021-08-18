#pragma once
#ifndef __MYDATE__
#define __MYDATE__
#include "list.h"

#define MIN_YEAR 2020
#define DATE_LENGTH 10

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
int compareDate(const void* pDate1, const void* pDate2);
void convertDateToString(const Date* date, char* dateChar);
int writeDateToBinaryFile(const Date* pDate, FILE* fp);
int readDateFromBinaryFile(Date* pDate, FILE* fp);
NODE* addDateToList(LIST* list, Date* pDate);

#endif
