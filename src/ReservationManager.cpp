
#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/ReservationManager.h"
#include <string>

using namespace std;

// Create an empty reservation list, waiting queue, and cancellation stack.
ReservationList::ReservationList()
{
    head = NULL;
    waitingFront = NULL;
    waitingRear = NULL;
    cancellationTop = NULL;
}

// Free all dynamically allocated nodes.
ReservationList::~ReservationList()
{
    while (head) {
        Reservation* temp = head;
        head = head->next;
        delete temp;
    }

    while (waitingFront) {
        QueueNode* temp = waitingFront;
        waitingFront = waitingFront->next;
        delete temp;
    }

    while (cancellationTop) {
        StackNode* temp = cancellationTop;
        cancellationTop = cancellationTop->next;
        delete temp;
    }
}

// Insert a reservation at the front of the linked list.
bool ReservationList::insert(int id, int studentID, string studentName,
                             string resourceID, string date)
{
    if (search(id)) {
        return false;
    }

    Reservation* newReservation =
        new Reservation(id, studentID, studentName, resourceID, date);

    newReservation->next = head;
    head = newReservation;

    return true;
}

// Cancel a reservation and save it in the cancellation stack.
bool ReservationList::remove(int reservationID, string& removedResourceID)
{
    Reservation* temp = head;

    if (temp && temp->getId() == reservationID) {
        removedResourceID = temp->getResourceId();

        StackNode* newNode = new StackNode{
            temp->getId(),
            temp->getStudentId(),
            temp->getStudentName(),
            temp->getResourceId(),
            temp->getDate(),
            cancellationTop
        };

        cancellationTop = newNode;
        head = temp->next;
        delete temp;
        return true;
    }

    while (temp && temp->next) {
        if (temp->next->getId() == reservationID) {
            Reservation* nodeToDelete = temp->next;

            removedResourceID = nodeToDelete->getResourceId();

            StackNode* newNode = new StackNode{
                nodeToDelete->getId(),
                nodeToDelete->getStudentId(),
                nodeToDelete->getStudentName(),
                nodeToDelete->getResourceId(),
                nodeToDelete->getDate(),
                cancellationTop
            };

            cancellationTop = newNode;

            temp->next = nodeToDelete->next;
            delete nodeToDelete;
            return true;
        }

        temp = temp->next;
    }

    return false;
}

// Search for a reservation by its ID.
bool ReservationList::search(int reservationID) const
{
    Reservation* temp = head;

    while (temp) {
        if (temp->getId() == reservationID) {
            return true;
        }

        temp = temp->next;
    }

    return false;
}

// Display all active reservations.
void ReservationList::print() const
{
    Reservation* temp = head;

    if (!temp) {
        cout << "No active reservations.\n";
        return;
    }

    while (temp) {
        cout << temp->getId() << " "
             << temp->getStudentId() << " "
             << temp->getStudentName() << " "
             << temp->getResourceId() << " "
             << temp->getDate() << endl;

        temp = temp->next;
    }
}

// Add a student to the end of the waiting queue.
void ReservationList::enqueueWaiting(int studentID, string studentName,
                                     string resourceID, string date)
{
    QueueNode* newNode = new QueueNode{
        studentID, studentName, resourceID, date, NULL
    };

    if (!waitingRear) {
        waitingFront = waitingRear = newNode;
    } else {
        waitingRear->next = newNode;
        waitingRear = newNode;
    }

    cout << "Student " << studentName
         << " added to the waiting list for resource "
         << resourceID << ".\n";
}

// Display everyone currently waiting.
void ReservationList::displayWaitingList() const
{
    cout << "\n--- Waiting List Queue ---\n";

    if (!waitingFront) {
        cout << "Waiting list is empty.\n";
        return;
    }

    QueueNode* temp = waitingFront;

    while (temp) {
        cout << "Student: " << temp->studentName
             << " (" << temp->studentID << ")"
             << " waiting for resource: " << temp->resourceID
             << " Date: " << temp->date << "\n";

        temp = temp->next;
    }
}

// Count everyone waiting, or only those waiting for one resource.
int ReservationList::waitingCount(string resourceID) const
{
    int total = 0;
    QueueNode* temp = waitingFront;

    while (temp) {
        if (resourceID == "" || temp->resourceID == resourceID) {
            total++;
        }

        temp = temp->next;
    }

    return total;
}

// Remove the first student waiting for the specified resource.
bool ReservationList::popNextWaiting(string resourceID, int& studentID,
                                     string& studentName, string& date)
{
    QueueNode* temp = waitingFront;
    QueueNode* previous = NULL;

    while (temp) {
        if (temp->resourceID == resourceID) {
            studentID = temp->studentID;
            studentName = temp->studentName;
            date = temp->date;

            if (previous) {
                previous->next = temp->next;
            } else {
                waitingFront = temp->next;
            }

            if (temp == waitingRear) {
                waitingRear = previous;
            }

            delete temp;
            return true;
        }

        previous = temp;
        temp = temp->next;
    }

    return false;
}

// Restore the most recently cancelled reservation.
bool ReservationList::undoCancellation(string& restoredResourceID)
{
    if (!cancellationTop) {
        cout << "No cancellations to undo.\n";
        return false;
    }

    StackNode* temp = cancellationTop;

    if (search(temp->id)) {
        cout << "Cannot restore: reservation ID already exists.\n";
        return false;
    }

    if (countForResource(temp->resourceID) > 0) {
        cout << "Cannot restore: resource is already reserved. "
             << "Cancellation remains in history.\n";
        return false;
    }

    restoredResourceID = temp->resourceID;

    Reservation* newReservation = new Reservation(
        temp->id,
        temp->studentID,
        temp->studentName,
        temp->resourceID,
        temp->date
    );

    newReservation->next = head;
    head = newReservation;

    cancellationTop = temp->next;
    delete temp;

    cout << "Cancellation undone.\n";
    return true;
}

// Display cancellation history, newest cancellation first.
void ReservationList::displayCancellationHistory() const
{
    cout << "\n--- Cancellation History ---\n";

    if (!cancellationTop) {
        cout << "No cancelled reservations.\n";
        return;
    }

    StackNode* temp = cancellationTop;

    while (temp) {
        cout << "Cancelled reservation ID: " << temp->id
             << " Student: " << temp->studentName
             << " Resource ID: " << temp->resourceID
             << " Date: " << temp->date << "\n";

        temp = temp->next;
    }
}

// Count active reservations.
int ReservationList::count() const
{
    int total = 0;
    Reservation* temp = head;

    while (temp) {
        total++;
        temp = temp->next;
    }

    return total;
}

// Find the highest active reservation ID.
int ReservationList::highestReservationID() const
{
    int highest = 0;
    Reservation* temp = head;

    while (temp) {
        if (temp->getId() > highest) {
            highest = temp->getId();
        }

        temp = temp->next;
    }

    return highest;
}

// Count active reservations for one resource.
int ReservationList::countForResource(string resourceID) const
{
    int total = 0;
    Reservation* temp = head;

    while (temp) {
        if (temp->getResourceId() == resourceID) {
            total++;
        }

        temp = temp->next;
    }

    return total;
}

// Return the highest number of active reservations for one resource.
int ReservationList::mostRequestedResourceCount() const
{
    int highest = 0;
    Reservation* temp = head;

    while (temp) {
        int resourceCount = countForResource(temp->getResourceId());

        if (resourceCount > highest) {
            highest = resourceCount;
        }

        temp = temp->next;
    }

    return highest;
}

// Display every resource tied for the highest active reservation count.
void ReservationList::printMostRequestedResources() const
{
    int highest = mostRequestedResourceCount();

    cout << "\n--- Most Requested Resources ---\n";

    if (highest == 0) {
        cout << "No active reservations.\n";
        return;
    }

    Reservation* temp = head;

    while (temp) {
        bool alreadyPrinted = false;
        Reservation* previous = head;

        while (previous != temp) {
            if (previous->getResourceId() == temp->getResourceId()) {
                alreadyPrinted = true;
                break;
            }

            previous = previous->next;
        }

        if (!alreadyPrinted &&
            countForResource(temp->getResourceId()) == highest) {
            cout << "Resource ID: " << temp->getResourceId()
                 << " Reservations: " << highest << "\n";
        }

        temp = temp->next;
    }
}

// Load active reservations from a comma-separated file.
bool ReservationList::loadFromFile(string filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string idText, studentText, studentName, resourceID, date;

        if (!getline(ss, idText, ',') ||
            !getline(ss, studentText, ',') ||
            !getline(ss, studentName, ',') ||
            !getline(ss, resourceID, ',') ||
            !getline(ss, date)) {
            continue;
        }

        try {
            int id = stoi(idText);
            int studentID = stoi(studentText);

            insert(id, studentID, studentName, resourceID, date);
        } catch (...) {
            cout << "Skipping invalid reservation record.\n";
        }
    }

    file.close();
    return true;
}

// Save active reservations to a comma-separated file.
bool ReservationList::saveToFile(string filename) const
{
    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    Reservation* temp = head;

    while (temp) {
        file << temp->getId() << ","
             << temp->getStudentId() << ","
             << temp->getStudentName() << ","
             << temp->getResourceId() << ","
             << temp->getDate() << "\n";

        temp = temp->next;
    }

    file.close();
    return true;
}
