
#pragma once
#ifndef __FLIGHT__
#define __FLIGHT__

#include "AirportManager.h"
#include "Date.h"
#include "General.h"

typedef struct
{
	char		originCode[CODE_LENGTH + 1];
	char		destCode[CODE_LENGTH + 1];
	int			hour;
	Date		date;
}Flight;

void	initFlight(Flight* pFlight, const AirportManager* pManager);
int		isFlightInRoute(const Flight* pFlight, const char* codeSource, const char* codeDest);
int		countFlightsInRoute(Flight** arr, int size, const char* codeSource, const char* codeDest);
void	printFlight(const void* pFlight);
int		getFlightHour();
Airport*	setAiportToFlight(const AirportManager* pManager, const char* msg);
void	freeFlight(Flight* pFlight);
int compareFlightsByTime(const void* flight1, const void* flight2);
int compareFlightsByDate(const void* flight1, const void* flight2);
int compareFlightsByDeparetureCode(const void* flight1, const void* flight2);
int compareFlightsByDestinationCode(const void* flight1, const void* flight2);
int printFlightToBinaryFile(const Flight* pFlight, FILE* fp);
int readFlightFromBinaryFile(Flight* pFlight, FILE* fp);

#endif
