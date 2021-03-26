-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2021-03-25 17:48:16.498

-- tables
-- Table: Bus
CREATE TABLE Bus (
    Number integer NOT NULL CONSTRAINT Bus_pk PRIMARY KEY,
    Name text NOT NULL,
    Mileage double NOT NULL
);

-- Table: Crew_members
CREATE TABLE Crew_members (
    Personnel_number integer NOT NULL CONSTRAINT Crew_members_pk PRIMARY KEY,
    Surname text NOT NULL,
    Experience double NOT NULL,
    Category text NOT NULL,
    Address text NOT NULL,
    Birth_year integer NOT NULL,
    Bus_Number integer NOT NULL,
    CONSTRAINT Crew_members_Bus FOREIGN KEY (Bus_Number)
    REFERENCES Bus (Number)
);

-- Table: Excursion_routes
CREATE TABLE Excursion_routes (
    Name text NOT NULL,
    Statring_point text NOT NULL,
    Final_point text NOT NULL,
    CONSTRAINT Excursion_routes_pk PRIMARY KEY (Name,Statring_point,Final_point)
);

-- Table: Flights_performed
CREATE TABLE Flights_performed (
    ID integer NOT NULL CONSTRAINT Flights_performed_pk PRIMARY KEY,
    Bus_Number integer NOT NULL,
    Excursion_routes_Name text NOT NULL,
    Excursion_routes_Statring_point text NOT NULL,
    Excursion_routes_Final_point text NOT NULL,
    Passengers_number integer NOT NULL,
    Price double NOT NULL,
    CONSTRAINT Flights_performed_Bus FOREIGN KEY (Bus_Number)
    REFERENCES Bus (Number),
    CONSTRAINT Flights_performed_Excursion_routes FOREIGN KEY (Excursion_routes_Name,Excursion_routes_Statring_point,Excursion_routes_Final_point)
    REFERENCES Excursion_routes (Name,Statring_point,Final_point)
);

-- End of file.

