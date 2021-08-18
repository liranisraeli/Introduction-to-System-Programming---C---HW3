#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"


void	initCompany(Company* pComp)
{
	printf("-----------  Init Airline Company\n");
	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	L_init(&pComp->dateList);
	pComp->sort = eNotSorted;
}

int	addFlight(Company* pComp, const AirportManager* pManager)
{
	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
		return 0;
	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	addDateToList(&pComp->dateList, &pComp->flightArr[pComp->flightCount]->date);
	pComp->flightCount++;
	sortFlights(pComp, pComp->sort + 1);
	return 1;
}

void printCompany(const Company* pComp)
{
	printf("Company %s:\n", pComp->name);
	printf("Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
	printf("flights date list:");
	L_print(&pComp->dateList, L_print_string);
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}



void	printFlightArr(Flight** pFlight, int size)
{
	generalArrayFunction(pFlight, size, sizeof(Flight*), printFlight);
	/*for (int i = 0; i < size; i++)
	printFlight(pFlight[i]);*/
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		freeFlight(arr[i]);
	}
}

void	freeCompany(Company* pComp)
{
	
	//freeFlightArr(pComp->flightArr, pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight*), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->dateList, free);
}

void sortFlights(Company* pComp, int sortType) {

	switch (sortType)
	{
	case (1): //hour
		pComp->sort = eHour;
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightsByTime);
	
		break;
	case (2): //date
		pComp->sort = eFlightDate;
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightsByDate);
		
		break;
	case (3): //departureCode
		pComp->sort = eDepartureCode;
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightsByDeparetureCode);
		
		break;
	case (4): //destinationCode
		pComp->sort = eDestinationCode;
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightsByDestinationCode);
		
		break;

	default:
		break;
	}

}
	int saveCompanyToBinaryFile(Company* pComp, FILE* fp)
	{
		if (!fp || !pComp)
			return 0;
		//write company name
		int len = strlen(pComp->name) + 1;
		if (fwrite(&len, sizeof(int), 1, fp) != 1)
			return 0;
		if (fwrite(pComp->name, sizeof(char), len, fp) != len)
			return 0;
		//write flights count
		if (fwrite(&pComp->flightCount, sizeof(int), 1, fp) != 1)
			return 0;
		//write sorting method
		if (fwrite(&pComp->sort, sizeof(int), 1, fp) != 1)
			return 0;
		//write flights list
		for (int i = 0; i < pComp->flightCount; i++) {
			if (!printFlightToBinaryFile(pComp->flightArr[i], fp))
				return 0;
		}
		return 1;
	}

	int loadCompanyFromBinaryFile(Company* pComp, FILE* fp) {

		if (!fp || !pComp)
			return 0;
		int len;
		//read company name
		if (fread(&len, sizeof(int), 1, fp) != 1)
			return 0;
		pComp->name = (char*)malloc(len * sizeof(char));
		if (!pComp->name)
			return 0;
		if (fread(pComp->name, sizeof(char), len, fp) != len)
			return 0;
		//read flights count
		if (fread(&pComp->flightCount, sizeof(int), 1, fp) != 1)
			return 0;
		//read sorting method
		if (fread(&pComp->sort, sizeof(int), 1, fp) != 1)
			return 0;
		//read flights list
		pComp->flightArr = (Flight**)malloc(sizeof(Flight*)*pComp->flightCount);
		if (!pComp->flightArr)
			return 0;
		if (!L_init(&pComp->dateList))
			return 0;
		for (int i = 0; i < pComp->flightCount; i++) {
			pComp->flightArr[i] = (Flight*)malloc(sizeof(Flight));
			if (!pComp->flightArr[i])
				return 0;
			readFlightFromBinaryFile(pComp->flightArr[i], fp);
			addDateToList(&pComp->dateList, &pComp->flightArr[i]->date);
		}
		return 1;
	}



	Flight* searchFlights(Company* pComp, Flight* pFlight) {
		Flight* found = NULL;
		void* func = NULL;
		switch (pComp->sort)
		{
		case (0): //hour
			func = compareFlightsByTime;
			break;
		case (1): //date
			func = compareFlightsByDate;
			break;
		case (2): //departureCode
			func = compareFlightsByDeparetureCode;
			break;
		case (3): //destinationCode
			func = compareFlightsByDestinationCode;
			break;

		default:
			break;
		}
		if (func)
			found = (Flight*)bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight**), func);
		if (found)
			return found;
		return NULL;
	}