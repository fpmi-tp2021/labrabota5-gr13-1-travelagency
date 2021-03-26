#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "bus.h"
#include "tour.h"
#include "completed_trip.h"
#include "crew_member.h"


void openDb();
int imageLoadCallback(void *outFile, int argc, char **argv, char **azColName);
void saveImageById(int id, FILE *outFile);
void updateImageById(int id, FILE *inFile);
void registerUser(char* login, char * password);
bool login(char * login, char * password);
void printBusData(int busId, char * startDate, char * endDate);
void printTeamEarnings();
void printMostExpensiveTripData();
void printMaximalTotalDistanceBusData();
void addBus(bus bus);
void addMember(struct crew_member member);
void addTrip(struct completed_trip trip);
void removeBus(int id);
void removeMember(int id);
void removeTrip(int id);
void updateBus(struct bus bus,int id);
void updateMember(struct crew_member, int id);
void updateTrip(struct completed_trip,  int id);
void checkInfo(struct tour);
void printCrewsEarrings(int days);
void printCrewsEarringByDate(char * dateString);



