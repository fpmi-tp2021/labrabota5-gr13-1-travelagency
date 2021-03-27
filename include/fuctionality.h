#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "bus.h"
#include "tour.h"
#include "completed_trip.h"
#include "crew_member.h"


int openDb();

int imageLoadCallback(void *outFile, int argc, char **argv, char **azColName);

void saveImageById(int id, FILE *outFile);

void updateImageById(int id, FILE *inFile);

void registerUser(char *login, char *password);

int login(char *login, char *password);

void printBusTotalStatistic(int busID);

void printBusFlightsForTime(int busId, char *startDate, char *endDate);

void printTeamsEarnings(char *startDate, char *endDate);

void printMostExpensiveTripData();

void printMaximalTotalDistanceBusData();

void addBus(bus bus);

void addMember(struct crew_member member);

void addCompletedTrip(struct completed_trip trip);

void addExcursion(struct tour);

void removeBus(int id);

void removeMember(int id);

void removeCompetedTrip(int id);

void updateBus(struct bus bus, int id);

void updateMember(struct crew_member, int id);

void checkInfo(struct tour);

void printCrewsEarrings(double persentage, char *startDate, char *endDate);

void printCrewsEarringByDate(int crewID, char *dateString);



