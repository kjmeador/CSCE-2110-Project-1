#include "Reservation.h"
#include <string>
using namespace std;

//Create a Linked List Class to store reservations
class ReservationList
{
private:
    Reservation* head;

public:
    ReservationList();

    void insert(int id, int studentID, string studentName, int resourceID, string date);
    void remove(int reservationID);
    void search(int reservationID);
    void print(); 
};
