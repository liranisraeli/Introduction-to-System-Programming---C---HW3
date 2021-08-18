#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AirportManager.h"

int	initManager(AirportManager* pManager)
{
	printf("-----------  Init airport Manager\n");
	L_init(&pManager->list);
	pManager->count = 0;
	int count = 0;
	do {
		printf("How many airport?\t");
		scanf("%d", &count);
	} while (count < 0);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	if (count == 0)
		return 1;



	for (int i = 0; i < count; i++)
	{
		if (!addAirport(pManager))
			return 0;
	}
	return 1;


}


int	addAirport(AirportManager* pManager)
{
	Airport* airport = (Airport*)malloc(sizeof(Airport));
	if (!airport) // allocation didn't work
		return 0;
	setAirport(airport, pManager);
	if (!L_insert_sorted(&pManager->list, airport, compareAirportByCode)) {
		return 0;
	}
	pManager->count++;
	return 1;


}

void  setAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}

	initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	Airport airport;
	strcpy(airport.code, code);
	NODE* temp = L_find(pManager->list.head.next, &airport, compareAirportByCode);
	if (!temp)
		return NULL;
	return temp->key;
}

int checkUniqeCode(const char* code, const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}

void	printAirports(const AirportManager* pManager)
{
	printf("there are %d airports\n", pManager->count);
	L_print(&pManager->list, printAirport);

}

void	freeManager(AirportManager* pManager)
{
	L_free(&pManager->list, freeAirport);
}

int saveAirportManagerToFile(AirportManager* pManager, FILE* fp) {
	if (!fp || !pManager)
		return 0;
	fprintf(fp, "%d\n", pManager->count);
	return L_printToFile(&pManager->list, fp, printAirportToFile);	
}

int loadAirportManagerFromFile(AirportManager* pManager, FILE* fp) {
	if (!fp || !pManager)
		return 0;
	L_init(&pManager->list);
	fscanf(fp, "%d",&(pManager->count));
	char temp[CHAR_MAX];
	myGetsFromFile(temp, CHAR_MAX, fp);
	for (int i = 0; i < pManager->count; i++) {
		Airport* pAirport = (Airport*)malloc(sizeof(Airport));
		if (!pAirport)
			return 0;
		readAirportFromFile(pAirport, fp);
		if (!L_insert_sorted(&pManager->list, pAirport, compareAirportByCode))
			return 0;
	}
	return 1;

}