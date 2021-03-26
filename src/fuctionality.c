#include "../include/fuctionality.h"
#include <stdio.h>
#include "../include/sqlite3.h"

#define DEFAULT_SIZE 255

struct sqlite3 *db;

void executeAndCheck(char *request);

void executeRemove(int id, char *table) {
    char request[DEFAULT_SIZE];
    sprintf(request, "DELETE FROM %s WHERE id = %d;", table, id);
    executeAndCheck(request);
}

static int callback(void *tmp, int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
        printf(" %s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");

    }
    printf("\n");
    return 0;
}


void openDb() {
    int openResult = sqlite3_open("../indentifier.sqlite", &db);
    if (openResult) {
        printf("Cant open db!\n");
    }
}


int imageLoadCallback(void *outFile, int argc, char **argv, char **azColName) {
    fprintf((FILE *) outFile, "%s", argv[0]);
}

//void saveImageById(int id, FILE *outFile);
//void updateImageById(int id, FILE *inFile);
void registerUser(char *login, char *password) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into USERS(login, password) VALUES(%s, %s);", login, password);
    executeAndCheck(request);
}

void printBusFlightsForTime(int busID, char *startDate, char *endDate) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT Excursion_routes_Name FROM Flights_performed"
                     "WHERE Bus_Number = %d"
                     "AND Excursion_routes_Starting_point > %s "
                     "AND Excursion_routes_Final_point < %s", busID, startDate, endDate);
    executeAndCheck(request);
}

void printTeamsEarnings(char *startDate, char *endDate) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT Surname,  0.2 * Price AS Earning"
                     "FROM Crew_members c"
                     "JOIN Flights_performed f"
                     "ON  c.Bus_Number = f.Bus_Number"
                     "WHERE Excursion_routes_Starting_point > %s "
                     "AND Excursion_routes_Final_point < %s", startDate, endDate);
    executeAndCheck(request);
}

//void printMostExpensiveTripData();
//void printMaximalTotalDistanceBusData();
void addBus(bus bus) {

    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Bus(Number, Name, Mileage) VALUES(%s, %s, %lf);", bus.number, bus.name,
            bus.totalDistance);
    executeAndCheck(request);
}

void addMember(struct crew_member member) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Crew_members(Personnel_number, "
                     "Surname, Experience, Category, Address, Birth_year, Bus_Number) "
                     "VALUES(%d, %s, %d, %s, %s, %d, %d);", member.id, member.surname, member.workExperience,
            member.category, member.address, member.birthYear, member.busNumber);
    executeAndCheck(request);
}

void addTrip(struct completed_trip trip) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Flights_performed(ID, "
                     "Bus_Number, Excursion_routes_Name, Excursion_routes_Starting_point"
                     ", Excursion_routes_Final_point, Passengers_number, Price) "
                     "VALUES(%d, %s, %s, %s, %s, %d, %lf);", trip.id, trip.busNumber, trip.tripName,
            trip.arrivalDate, trip.appearDate, trip.passengersCount,
            trip.price);
    executeAndCheck(request);
}


void removeBus(int id) {
    executeRemove(id, "Bus");
}

void removeMember(int id) {
    executeRemove(id, "Crew_members");
}

void removeTrip(int id) {
    executeRemove(id, "Flights_performed");
}

void updateBus(struct bus bus, int id) {
    char request[DEFAULT_SIZE];
    sprintf(request, "UPDATE Table Bus "
                     "SET Name = %s,"
                     "SET Mileage = %lf", bus.name, bus.totalDistance);
    executeAndCheck(request);
}

void updateMember(struct crew_member member, int id) {
    char request[DEFAULT_SIZE];
    sprintf(request, "UPDATE Table Bus "
                     "SET Surname = %s,"
                     "SET Experience = %d"
                     "SET Category = %s"
                     "SET Address = %s"
                     "SET Birth_year = %d"
                     "SET Bus_Number = %d",
            member.surname, member.workExperience,
            member.category, member.address, member.birthYear,
            member.busNumber);
    executeAndCheck(request);
}

//void checkInfo(struct tour);
//void printCrewsEarrings(int days);
//void printCrewsEarringByDate(char * dateString);

void executeAndCheck(char *request) {
    char *error = 0;
    if (sqlite3_exec(db, request, callback, 0, &error) != SQLITE_OK) {
        printf("%s\n", error);
    }
}
