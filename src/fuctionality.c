#include "../include/fuctionality.h"
#include <stdio.h>
#include "../include/sqlite3.h"

#define DEFAULT_SIZE 255

int currentExecutionResult = -1;
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

static int loginCallback(void *tmp, int argc, char **argv, char **colName) {
    currentExecutionResult = argv != NULL ? 0 : -1;
    return 0;
}

static int validateCallback(void *tmp, int argc, char **argv, char **colName) {
    return !argc;
}

int openDb() {
    int openResult = sqlite3_open("../TravelAgency.sqlite", &db);
    if (openResult) {
        printf("Cant open db!\n");
        return -1;
    }
    return 0;
}

int login(char *login, char * password){
    currentExecutionResult = -1;
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT * FROM Users WHERE Login='%s' AND Password='%s';", login, password);
    char *error = 0;
    if (sqlite3_exec(db, request, loginCallback, 0, &error) != SQLITE_OK) {
        printf("%s\n", error);
        return -1;
    }
    return currentExecutionResult;
}

int imageLoadCallback(void *outFile, int argc, char **argv, char **azColName) {
    fprintf((FILE *) outFile, "%s", argv[0]);
}

void saveImageById(int id, FILE *outFile) {
    char request[DEFAULT_SIZE];
    char *error;
    sprintf(request, "SELECT Image FROM Bus where Number=%d;", id);

    sqlite3_stmt *pStmt;
    sqlite3_prepare_v2(db, request, -1, &pStmt, 0);

    int rc = sqlite3_step(pStmt);
    int bytes = 0;

    if (rc == SQLITE_ROW) {
        bytes = sqlite3_column_bytes(pStmt, 0);
    }
    fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, outFile);
    fclose(outFile);
    sqlite3_finalize(pStmt);
}

void updateImageById(int id, FILE *inFile) {
    fseek(inFile, 0, SEEK_END);
    int length = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);
    char bytes[length + 1];
    int size = fread(bytes, 1, length, inFile);
    fclose(inFile);

    sqlite3_stmt *pStmt;
    char request[DEFAULT_SIZE * 1000];
    sprintf(request, "UPDATE Bus SET  Image = (?) WHERE id = %d", id);

    if (sqlite3_prepare(db, request, -1, &pStmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_bind_blob(pStmt, 1, bytes, size, SQLITE_STATIC);

    if (sqlite3_step(pStmt) != SQLITE_DONE) {
        printf("execution failed: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(pStmt);
}

void registerUser(char *login, char *password) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into USERS(login, password) VALUES(%s, %s);", login, password);
    executeAndCheck(request);
}

void printBusFlightsForTime(int busID, char *startDate, char *endDate) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT Excursion_routes_Name FROM Flights_performed"
                     "WHERE Bus_Number = %d"
                     "AND StartDate > %s "
                     "AND Arrival_Date < %s", busID, startDate, endDate);
    executeAndCheck(request);
}

void printBusTotalStatistic(int busID) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT b.Name, COUNT(f.ID) AS totalTrips,"
                     "SUM(f.Passengers_number) AS totalPassengers,"
                     " SUM(f.Price) AS totalPrice"
                     "FROM Bus b"
                     "JOIN Flights_performed f"
                     "ON  b.Number = f.Bus_Number"
                     "GROUP BY b.Name"
    );
    executeAndCheck(request);
}

void printTeamsEarnings(char *startDate, char *endDate) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT Surname,  0.2 * Price AS Earning"
                     "FROM Crew_members c"
                     "JOIN Flights_performed f"
                     "ON  c.Bus_Number = f.Bus_Number"
                     "WHERE StartDate > %s "
                     "AND Arrival_Date < %s", startDate, endDate);
    executeAndCheck(request);
}

void printMostExpensiveTripData() {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT f.Price, b.Name c.Surname"
                     "FROM Flights_performed f"
                     "JOIN Bus b"
                     "ON  f.Bus_Number = b.Number"
                     "JOIN Crew_members c"
                     "ON b.Number = c.Bus_Number"
                     "WHERE Price >= (SELECT MAX(Price) "
                     "FROM Flights_performed)");
    executeAndCheck(request);
}

void printMaximalTotalDistanceBusData() {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT b.Name, b.Mileage, SUM(f.Passengers) AS PassengersCount"
                     "FROM Bus b"
                     "JOIN Crew_members c"
                     "ON  b.Number = c.Bus_Number"
                     "JOIN Flights_performed f"
                     "ON  b.Number = f.Bus_Number"
                     "GROUP BY b.Name"
                     "HAVING b.Mileage = MAX(b.Mileage)"
    );
    executeAndCheck(request);
}


int addBus(bus bus) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Bus(Number, Name, Mileage) VALUES(%s, %s, %lf);", bus.number, bus.name,
            bus.totalDistance);
    return executeAndCheck(request);
}

void addMember(struct crew_member member) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Crew_members(Personnel_number, "
                     "Surname, Experience, Category, Address, Birth_year, Bus_Number) "
                     "VALUES(%d, %s, %d, %s, %s, %d, %d);", member.id, member.surname, member.workExperience,
            member.category, member.address, member.birthYear, member.busNumber);
    executeAndCheck(request);
}

void addCompletedTrip(struct completed_trip trip) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Flights_performed(ID, "
                     "Bus_Number, Excursion_routes_Name, StartDate"
                     ", Arrival_Date, Passengers_number, Price) "
                     "VALUES(%d, %s, %s, %s, %s, %d, %lf);", trip.id, trip.busNumber, trip.tripName,
            trip.arrivalDate, trip.appearDate, trip.passengersCount,
            trip.price);
    executeAndCheck(request);
}

void addExcursion(struct tour tour) {
    char request[DEFAULT_SIZE];
    sprintf(request, "INSERT into Excursion_routes(Name, "
                     "Starting_point, Final_point) "
                     "VALUES(%s, %s, %s);", tour.name, tour.startPoint, tour.destination);
    executeAndCheck(request);
}


void removeBus(int id) {
    executeRemove(id, "Bus");
}

void removeMember(int id) {
    executeRemove(id, "Crew_members");
}

void removeCompetedTrip(int id) {
    executeRemove(id, "Flights_performed");
}

void removeExcursion(int id) {
    executeRemove(id, "Excursion_routes");
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

void checkInfo(struct tour tour) {
    int infoValid;
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT * FROM Flights_performed"
                     "WHERE Excursion_routes_Name = %s"
                     "AND Excursion_routes_Starting_point = %s"
                     "AND Excursion_routes_Final_point = %s",
            tour.name, tour.startPoint, tour.destination);

    infoValid = sqlite3_exec(db, request, validateCallback, 0, NULL);
    if (infoValid) {
        char addRequest[DEFAULT_SIZE];
        sprintf(addRequest, "INSERT INTO Excursion_routes(Name, Starting_point, Final_point)"
                            "VALUES(%s,%s,%s)",
                tour.name, tour.startPoint, tour.destination);
    }
    else{
        printf("%s", "Data is invalid!");
    }
}


void printCrewsEarrings(double persentage, char *startDate, char *endDate) {
    char request[DEFAULT_SIZE];
    sprintf(request, "CREATE VIEW Crew_Earnings AS"
                     "SET SELECT c.Surname, %lf * SUM(f.Price)"
                     "AS Earning"
                     "FROM Crew_members c"
                     "JOIN Flights_performed f"
                     "ON c.Bus_Number = f.Bus_Number"
                     "GROUP BY c.Surname"
                     "HAVING f.StartDate > %s"
                     "AND f.Arrival_Date < %s",
            persentage, startDate, endDate);
    executeAndCheck(request);
}

void printCrewsEarringByDate(int crewID, char *dateString) {
    char request[DEFAULT_SIZE];
    sprintf(request, "SELECT SUM(Price) AS Earning"
                     "FROM Crew_members c"
                     "JOIN Flights_performed f"
                     "ON c.Bus_Number = f.Bus_Number"
                     "HAVING f.Arrival_Date = %s"
                     "AND c.Bus_Number = %d",
            dateString, crewID);
    executeAndCheck(request);
}

void executeAndCheck(char *request) {
    char *error = 0;
    if (sqlite3_exec(db, request, callback, 0, &error) != SQLITE_OK) {
        printf("%s\n", error);
    }
}
