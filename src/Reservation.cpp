#include "../include/Reservation.h"
#include <string>

using namespace std;

Reservation::Reservation()
    : id(0), studentId(0), studentName(""),
      resourceId(""), date(""), next(NULL) {}

Reservation::Reservation(int id, int studentId, string studentName, string resourceId, string date)
    : id(id), studentId(studentId), studentName(studentName),
      resourceId(resourceId), date(date), next(NULL) {}

int Reservation::getId() {
    return id;
}

int Reservation::getStudentId() {
    return studentId;
}

string Reservation::getStudentName() {
    return studentName;
}

string Reservation::getResourceId() {
    return resourceId;
}

string Reservation::getDate() {
    return date;
}
