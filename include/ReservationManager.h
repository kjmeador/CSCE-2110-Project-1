#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "Reservation.h"
#include <string>
using namespace std;

class ReservationList {
private:
    Reservation* head;

    // Waiting queue node (first-in, first-out).
    struct QueueNode {
        int studentID;
        string studentName;
        string resourceID;
        string date;
        QueueNode* next;
    };

    QueueNode* waitingFront;
    QueueNode* waitingRear;

    // Cancellation stack node (last-in, first-out).
    struct StackNode {
        int id;
        int studentID;
        string studentName;
        string resourceID;
        string date;
        StackNode* next;
    };

    StackNode* cancellationTop;

public:
    ReservationList();
    ~ReservationList();

    // Active reservation linked list.
    bool insert(int id, int studentID, string studentName,
                string resourceID, string date);
    bool remove(int reservationID, string& removedResourceID);
    bool search(int reservationID) const;
    void print() const;

    // Waiting queue operations.
    void enqueueWaiting(int studentID, string studentName,
                        string resourceID, string date);
    void displayWaitingList() const;
    int waitingCount(string resourceID = "") const;
    bool popNextWaiting(string resourceID, int& studentID,
                        string& studentName, string& date);

    // Cancellation history and undo.
    bool peekCancellationResourceID(string& resourceID) const;
    bool undoCancellation(string& restoredResourceID);
    void displayCancellationHistory() const;

    // Report calculations.
    int count() const;
    int highestReservationID() const;
    int countForResource(string resourceID) const;
    int mostRequestedResourceCount() const;
    void printMostRequestedResources() const;

    // Save and load active reservations.
    bool loadFromFile(string filename);
    bool saveToFile(string filename) const;
};

#endif
