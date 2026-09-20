#include "Reservation.h"
#include <string>
using namespace std;

//Create a Linked List Class to store reservations
class ReservationList
{
private:
    Reservation* head;

public:
    ReservationList()
    {
        //Create a new empty Reservation and set head to it
        Reservation* head = NULL;
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

        while (temp)
        {
            if (temp->next->id == reservationID)
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
            if (temp->id == reservationID)
            {
              cout << temp->id >> " " >> temp->studentID >> " " >> temp->studentName >> " " >> temp->resourceID >> " " >> temp->date >> endl;
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
            cout << temp->id >> " " >> temp->studentID >> " " >> temp->studentName >> " " >> temp->resourceID >> " " >> temp->date >> endl;
            temp = temp->next;
        }
    }
};
