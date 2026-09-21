#include <iostream>
#include <fstream>
#include <sstream>
#include "ReservationManager.h"
#include <string>

using namespace std;

struct QueueNode {
    int studentID;
    string studentName;
    int resourceID;
    string date;
    QueueNode* next;
};

struct StackNode {
    int id;
    int studentID;
    string studentName;
    int resourceID;
    string date;
    StackNode* next;
};

//Create a Linked List Class to store reservations
class ReservationList
{
private:
    Reservation* head;

public:
    ReservationList()
    {
        //Create a new empty Reservation and set head to it
        head = NULL;
    }

    //Create a reservation
    //Insert new reservation, basic front-sert
    void insert(int id, int studentID, string studentName, int resourceID, string date)
    {
        Reservation* newReservation = new Reservation (id, studentID, studentName, resourceID, date);
        newReservation->next = head;
        head = newReservation;
    }

    //Cancel a reservation
    //Linear search and remove reservation
    void remove(int reservationID)
    {
        Reservation* temp = head;
        
        //If head is the reservation to remove
        if (temp && temp->getId() == reservationID)
        {
            head = temp->next;
            delete temp;
            return;
        }
        
        while (temp && temp->next)
        {
            if (temp->next->getId() == reservationID)
            {
              //Create a pointer to delete next item
              Reservation * node_to_delete = temp->next;

              //Skip node to delete
              temp->next = temp->next->next;

              //Delete cancelled reservation
              delete node_to_delete;
              
              return;
            }

          //Move to next node in list
          temp = temp->next;
        }
    }

    //Search for a reservation
    void search(int reservationID)
    {
        Reservation* temp = head;

        while (temp)
        {
            //If reservation found, print it out
            if (temp->getId() == reservationID)
            {
              cout << temp->getId() << " " << temp->getStudentId() << " " << temp->getStudentName() << " " << temp->getResourceId() << " " << temp->getDate() << endl;
              return;
            }

          //Move to next node in list
          temp = temp->next;
        }

      //If go through whole list and never return, reservation not found
      cout << "No reservation found." << endl;
    }

    //View all current reservtions
    //Traverse list and print
    void print()
    {
        Reservation* temp = head;

        while (temp)
        {
            cout << temp->getId() << " " << temp->getStudentId() << " " << temp->getStudentName() << " " << temp->getResourceId() << " " << temp->getDate() << endl;
            temp = temp->next;
        }
    }
};
//waiting list queue
class WaitingListQueue {
private:
    QueueNode* front;
    QueueNode* rear;
public:
    WaitingListQueue() : front(nullptr), rear(nullptr) {}
    void enqueue(int studentID, string studentName, int resourceId, string date) {
        QueueNode* newNode = new QueueNode{studentID, studentName, resourceId, date, nullptr};
        if(!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "{waiting list} Student " << studentName << " added to the waiting for resource " << resourceId << ".\n";
    }
    bool dequeueForResource(int resourceID, int& outStudentID, string& outStudentName, string& outDate) {
        if(!front) return false;
        if(front->resourceID == resourceID ) {
            QueueNode* temp = front;
            outStudentID = front->studentID;
            outStudentName = front->studentName;
            outDate = front->date;
            if (!front) rear = nullptr;
            delete temp;
            return true;
        } 
        QueueNode* curr = front;
        while (curr->next && curr->next->resourceID != resourceID) {
            curr = curr->next;
        }
        if (curr->next) {
            QueueNode* temp = curr->next;
            outStudentID = temp->studentID;
            outStudentName = temp->studentName;
            outDate = temp->date;
            if(temp == rear) rear = curr;
            curr->next = temp->next;
            delete temp;
            return true;
        }
        return false;
    }
    void displayWaitingList() const {
        cout << "\n---Waiting List Queue ---\n";
        if(!front) {
            cout << "waiting list is empty.\n";
            return;
        }
        QueueNode* temp = front;
        while (temp) {
            cout << "Student: " << temp->studentName << " (" << temp->studentID
                 << ") waiting for resource: " << temp->resourceID
                 << "  date: " << temp->date << "\n";
            temp = temp->next;
        }
    }
};

class CancellationStack {
private:
    StackNode* top;
public:
    CancellationStack() : top(nullptr) {}

    void push(int id, int studentID, string studentName, int resourceID, string date) {
        StackNode* newNode = new StackNode{id, studentID, studentName, resourceID, date};
        newNode->next = top;
        top = newNode;
    }
    bool pop(int& outId, int& outStudentID, string& outStudentName, int& outResourceID, string& outDate) {
        if(!top) return false;
        StackNode* temp = top;
        outId = top->id;
        outStudentID = top->studentID;
        outStudentName = top->studentName;
        outResourceID = top->resourceID;
        outDate = top->date;
        top = top->next;
        delete temp;
        return true;
    }
    void displayHistory() const {
        cout << "\n--- Cancellation History ---\n";
        if(!top) {
            cout << "No cancelled reservations.\n";
            return;
        }
        StackNode* temp = top;
        while (temp) {
            cout << "Cancelled reservation ID: " << temp->id
                 << " student: " << temp->studentName
                 << " resource ID: " << temp->resourceID
                 << " date: " << temp->date << "\n";
            temp = temp->next;
        }
    }
};
