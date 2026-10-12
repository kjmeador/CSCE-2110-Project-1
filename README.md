**# CSCE-2110-Project-1**
Campus Resource Reservation System

Language: C++

Current Features
- Load resource information from a file
- Store resource information
- Display all resources
- Display resource availability
- Create reservations
- Cancel reservations
- Display active reservations
- Validate reservation requests
- Store active reservations in a linked list
- Insert reservation
- Remove reservation
- Search reservations by ID
- Traverse and display reservations
- Store waiting lists using queues
- Add student to waiting list
- Remove student from waiting list
- Display waiting lists
- Store cancelled reservations using a stack
- Restore the most recently cancelled reservation
- Display cancellation history
- Sort resources by ID, name, type, and availability
- Generate reservation and resource utilization reports
- Display most requested resources
- Display waiting-list statistics
- Save active reservations to a file
- Load saved reservations when the program starts

Compile by installing g++ and running

`g++ -std=c++14 src/main.cpp src/Resource.cpp src/Reservation.cpp src/ReservationManager.cpp -o reservation_system`

Run the program using

`./reservation_system`

On Windows PowerShell, run

`.\reservation_system.exe`
