#include <iostream>
#include <string>
#include <vector>

#include "../include/Resource.h"
#include "../include/ReservationManager.h"

using namespace std;

void displayMenu()
{
    cout << "\n===== Campus Resource Reservation System =====\n";
    cout << "1. View Resources\n";
    cout << "2. Create Reservation\n";
    cout << "3. Cancel Reservation\n";
    cout << "4. View Waiting Lists\n";
    cout << "5. Undo Cancellation\n";
    cout << "6. Search Reservations\n";
    cout << "7. Sort Resources\n";
    cout << "8. Generate Report\n";
    cout << "9. Exit\n";
    cout << "\nEnter Choice: ";
}

void viewResources(vector<Resource>& resources)
{
    cout << "\n--- Resources ---\n";

    if (resources.empty())
    {
        cout << "No resources found.\n";
        return;
    }

    for (const Resource& resource : resources)
    {
        cout << "ID: " << resource.getResourceID()
             << " | Name: " << resource.getResourceName()
             << " | Type: " << resource.getResourceType()
             << " | Availability: "
             << (resource.getAvailability() ? "Available" : "Unavailable")
             << '\n';
    }
}

int main()
{
    vector<Resource> resources = loadResources("data/resources.txt");
    ReservationList reservations;

    int choice = 0;
    int nextReservationID = 1;

    cout << "Campus Resource Reservation System started.\n";

    do
    {
        displayMenu();

        cin >> choice;

        switch (choice)
        {
            case 1:
                viewResources(resources);
                break;

            case 2:
            {
                int studentID;
                int resourceID;
                string studentName;
                string date;

                cout << "Enter student ID: ";

                cout << "Enter student name: ";
                getline(cin, studentName);

                cout << "Enter resource ID: ";
                cin >> resourceID;

                cout << "Enter reservation date: ";
                getline(cin, date);

                Resource* resource =
                    searchResource(resources, to_string(resourceID));

                if (resource == nullptr)
                {
                    cout << "Resource not found.\n";
                }
                else if (!resource->getAvailability())
                {
                    cout << "Resource is unavailable.\n";
                    cout << "Waiting-list functionality needs to be connected.\n";
                }
                else
                {
                    reservations.insert(
                        nextReservationID,
                        studentID,
                        studentName,
                        resourceID,
                        date
                    );

                    resource->setAvailability(false);

                    cout << "Reservation created successfully.\n";
                    cout << "Reservation ID: " << nextReservationID << '\n';

                    nextReservationID++;
                }

                break;
            }

            case 3:
            {
                int reservationID;

                cout << "Enter reservation ID to cancel: ";
                cin >> reservationID;

                reservations.remove(reservationID);

                cout << "Cancellation request processed.\n";
                cout << "Cancellation history and resource availability "
                     << "updates still need to be connected.\n";

                break;
            }

            case 4:
                cout << "Waiting-list display needs to be connected "
                     << "to WaitingListQueue.\n";
                break;

            case 5:
                cout << "Undo cancellation needs to be connected "
                     << "to CancellationStack.\n";
                break;

            case 6:
            {
                int reservationID;

                cout << "Enter reservation ID to search: ";

                cin >> reservationID;

                reservations.search(reservationID);
                break;
            }

            case 7:
            {
                int criteria;

                cout << "\nSort Resources By:\n";
                cout << "1. Resource ID\n";
                cout << "2. Resource Name\n";
                cout << "3. Resource Type\n";
                cout << "4. Availability\n";
                cout << "Enter criteria: ";

                cin >> criteria;

                if (criteria < 1 || criteria > 4)
                {
                    cout << "Invalid sorting criteria.\n";
                    break;
                }

                sortResources(resources, criteria);

                cout << "Resources sorted successfully.\n";
                viewResources(resources);

                break;
            }

            case 8:
                cout << "\n--- System Report ---\n";
                cout << "Total resources: " << resources.size() << '\n';

                cout << "\nResource Details:\n";
                viewResources(resources);

                cout << "\nActive Reservations:\n";
                reservations.print();

                break;

            case 9:
                cout << "Exiting Campus Resource Reservation System.\n";
                break;

            default:
                cout << "Invalid choice. Enter a number from 1 to 9.\n";
        }

    } while (choice != 9);

    return 0;
}

