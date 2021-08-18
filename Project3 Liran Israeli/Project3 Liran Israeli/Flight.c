#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"



void	initFlight(Flight* pFlight, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->originCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	getCorrectDate(&pFlight->date);
	pFlight->hour = getFlightHour();
}

int	isFlightInRoute(const Flight* pFlight, const char* codeSource, const char* codeDest)
{
	if ((strcmp(pFlight->originCode, codeSource) == 0) &&
		(strcmp(pFlight->destCode, codeDest) == 0))
		return 1;

	return 0;
}

int	countFlightsInRoute(Flight** arr, int size, const char* codeSource,
	const char* codeDest)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (isFlightInRoute(arr[i], codeSource, codeDest))
			count++;
	}
	return count;
}

void	printFlight(const void* pFlight)
{
	const Flight* pFl = *(const Flight**)pFlight;
	printf("Flight From %s To %s\t", pFl->originCode, pFl->destCode);
	printDate(&pFl->date);
	printf(" Hour: %d\n", pFl->hour);
}


int getFlightHour()
{
	int h;
	do {
		printf("Enter flight hour [0-23]:\t");
		scanf("%d", &h);
	} while (h < 0 || h>23);
	return h;
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		myGets(code, MAX_STR_LEN);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport in this country - try again\n");
	} while (port == NULL);

	return port;

}

void	freeFlight(Flight* pFlight)
{
	free(pFlight);
}

int compareFlightsByTime(const void* flight1, const void* flight2) {
	const Flight* f1 = *(Flight**)flight1;
	const Flight* f2 = *(Flight**)flight2;
	return f1->hour - f2->hour;
}

int compareFlightsByDate(const void* flight1, const void* flight2){
	const Flight* f1 = *(Flight**)flight1;
	const Flight* f2 = *(Flight**)flight2;

	char f1Str[DATE_LENGTH + 1];
	char f2Str[DATE_LENGTH + 1];
 
    convertDateToString(&f1->date, f1Str);
    convertDateToString(&f2->date, f2Str);
    int temp = compareDate(f1Str, f2Str); 
	return temp;
}

int compareFlightsByDeparetureCode(const void* flight1, const void* flight2) {
	const Flight* f1 = *(Flight**)flight1;
	const Flight* f2 = *(Flight**)flight2;
	return strcmp(f1->originCode, f2->originCode);
}

int compareFlightsByDestinationCode(const void* flight1, const void* flight2) {
	const Flight* f1 = *(Flight**)flight1;
	const Flight* f2 = *(Flight**)flight2;
	return strcmp(f1->destCode, f2->destCode);
}


int printFlightToBinaryFile(const Flight* pFl, FILE* fp) {
	if (!fp || !pFl)
		return 0;
	//write originCode
	if (fwrite(&pFl->originCode, sizeof(char), CODE_LENGTH, fp) != CODE_LENGTH)
		return 0;
	//write destinationCode
	if (fwrite(&pFl->destCode, sizeof(char), CODE_LENGTH , fp) != CODE_LENGTH )
		return 0;
	//write hour
	if (fwrite(&pFl->hour, sizeof(int), 1, fp) != 1)
		return 0;
	//write date
	if (!writeDateToBinaryFile(&pFl->date, fp))
		return 0;
	return 1;
}

int readFlightFromBinaryFile(Flight* pFl, FILE* fp) {
	if (!fp || !pFl)
		return 0;
	//read originCode
	if (fread(pFl->originCode, sizeof(char), CODE_LENGTH , fp) != CODE_LENGTH )
		return 0;
	pFl->originCode[CODE_LENGTH] = '\0';
	//read destinationCode
	if (fread(pFl->destCode, sizeof(char), CODE_LENGTH , fp) != CODE_LENGTH )
		return 0;
	pFl->destCode[CODE_LENGTH] = '\0';
	// read hour
	if (fread(&pFl->hour, sizeof(int), 1, fp) != 1)
		return 0;
	// read date
	if (!readDateFromBinaryFile(&pFl->date,fp))
		return 0;
	return 1;
}