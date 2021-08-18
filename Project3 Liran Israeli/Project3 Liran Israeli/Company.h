#pragma once
#ifndef __COMP__
#define __COMP__

#define EXIT			-1
#include "Flight.h"
#include "AirportManager.h"

typedef enum {eHour, eFlightDate, eDepartureCode, eDestinationCode, eNotSorted} SORT;

typedef struct
{
	LIST dateList;
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	SORT sort;
}Company;

void	initCompany(Company* pComp);
int		addFlight(Company* pComp, const AirportManager* pManager);
void	printCompany(const Company* pComp);
void	printFlightsCount(const Company* pComp);
void	printFlightArr(Flight** pFlight, int size);
void	freeFlightArr(Flight** arr, int size);
void	freeCompany(Company* pComp);
void sortFlights(Company* pComp, int sortType);
int saveCompanyToBinaryFile(Company* pComp, FILE* fp);
int loadCompanyFromBinaryFile(Company* pComp, FILE* fp);
Flight* searchFlights(Company* pComp, Flight* pFlight);
#endif

