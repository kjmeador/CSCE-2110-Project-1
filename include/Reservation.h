#include <string>
using namespace std;

//Create the Reservation Class
class Reservation {
private:
    int id;
    int studentId;
    string studentName;
    int resourceId;
    string date;

public:
    //Default constructor and initialized constructor
    Reservation();
    Reservation(int id, int studentId, string studentName, int resourceId, string date);

    //Methods to return data from class
    int getId();
    int getStudentId();
    string getStudentName();
    int getResourceId();
    string getDate();
};
