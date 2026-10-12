#include <iostream>
#include <string>
#include <vector>
#include "../include/Resource.h"
#include "../include/ReservationManager.h"
using namespace std;

// Displays the main menu.
void displayMenu() {
    cout << "\n===== Campus Resource Reservation System =====\n"
         << "1. View Resources\n"
         << "2. Create Reservation\n"
         << "3. Cancel Reservation\n"
         << "4. View Waiting Lists\n"
         << "5. Undo Cancellation\n"
         << "6. Search Reservations\n"
         << "7. Sort Resources\n"
         << "8. Generate Report\n"
         << "9. Exit\n"
         << "Enter Choice: ";
}

// Displays all available and unavailable resources.
void viewResources(vector<Resource>& resources) {
    cout << "\n--- Resources ---\n";

    if (resources.empty()) {
        cout << "No resources found. Check data/resources.txt.\n";
        return;
    }

    for (const Resource& r : resources) {
        cout << "ID: " << r.getResourceID()
             << " | Name: " << r.getResourceName()
             << " | Type: " << r.getResourceType()
             << " | Availability: "
             << (r.getAvailability() ? "Available" : "Unavailable")
             << '\n';
    }
}

// Validates positive integer input.
bool readPositiveInt(const string& prompt, int& value) {
    cout << prompt;

    if (!(cin >> value) || value <= 0) {
        cout << "Invalid input. Enter a positive whole number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }

    cin.ignore(1000, '\n');
    return true;
}

int main() {
    // Load resources and saved reservations.
    vector<Resource> resources = loadResources("data/resources.txt");
    ReservationList reservations;

    if (!reservations.loadFromFile("data/reservations.txt")) {
        cout << "Warning: could not load saved reservations.\n";
    }

    int nextID = reservations.highestReservationID() + 1;

    // Mark resources with active reservations as unavailable.
    for (Resource& r : resources) {
        if (reservations.countForResource(r.getResourceID()) > 0) {
            r.setAvailability(false);
        }
    }

    cout << "Campus Resource Reservation System started.\n";

    int choice = 0;

    do {
        displayMenu();

        if (!(cin >> choice)) {
            cout << "Invalid input. Enter a number from 1 to 9.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            viewResources(resources);
            break;

        case 2: {
            // Collect student and resource information.
            int sid;
            string name, rid, date;

            if (!readPositiveInt("Enter student ID: ", sid)) {
                break;
            }

            cout << "Enter student name: ";
            getline(cin, name);

            cout << "Enter resource ID: ";
            getline(cin, rid);

            cout << "Enter reservation date (e.g. 2026-10-15): ";
            getline(cin, date);

            if (name.empty() || rid.empty() || date.empty()) {
                cout << "Name, resource ID, and date cannot be empty.\n";
                break;
            }

            Resource* r = searchResource(resources, rid);

            if (r == nullptr) {
                cout << "Resource not found.\n";
                break;
            }

            // Add unavailable resources to the waiting queue.
            if (!r->getAvailability()) {
                reservations.enqueueWaiting(sid, name, rid, date);
                cout << "Resource unavailable. Added to waiting list.\n";
                break;
            }

            if (reservations.insert(nextID, sid, name, rid, date)) {
                r->setAvailability(false);
                cout << "Reservation created. Reservation ID: "
                     << nextID << '\n';
                ++nextID;
            } else {
                cout << "Reservation could not be created.\n";
            }
            break;
        }

        case 3: {
            // Cancel a reservation and assign the resource to the next waiting student.
            int id;

            if (!readPositiveInt("Enter reservation ID to cancel: ", id)) {
                break;
            }

            string rid;

            if (!reservations.remove(id, rid)) {
                cout << "No active reservation found with that ID.\n";
                break;
            }

            Resource* r = searchResource(resources, rid);
            int sid;
            string name, date;

            if (r && reservations.popNextWaiting(rid, sid, name, date)) {
                reservations.insert(nextID, sid, name, rid, date);
                r->setAvailability(false);

                cout << "Reservation cancelled. Waiting student "
                     << name << " received reservation ID "
                     << nextID << ".\n";
                ++nextID;
            } else {
                if (r) {
                    r->setAvailability(true);
                }

                cout << "Reservation cancelled and added to cancellation history.\n";
            }
            break;
        }

        case 4:
            reservations.displayWaitingList();
            break;

        case 5: {
            // Restore the most recently cancelled reservation.
            string rid;

            if (!reservations.undoCancellation(rid)) {
                break;
            }

            Resource* r = searchResource(resources, rid);

            if (r) {
                r->setAvailability(false);
            }

            cout << "Cancellation undone; reservation restored.\n";
            break;
        }

        case 6: {
            int id;

            if (readPositiveInt("Enter reservation ID to search: ", id)) {
                if (!reservations.search(id)) {
                    cout << "No reservation found with that ID.\n";
                }
            }
            break;
        }

        case 7: {
            // Sort resources using the selected field.
            int criteria;

            cout << "\nSort Resources By:\n"
                 << "1. Resource ID\n"
                 << "2. Resource Name\n"
                 << "3. Resource Type\n"
                 << "4. Availability\n";

            if (!readPositiveInt("Enter criteria: ", criteria)) {
                break;
            }

            if (criteria < 1 || criteria > 4) {
                cout << "Invalid sorting criteria.\n";
                break;
            }

            sortResources(resources, criteria);
            cout << "Resources sorted successfully.\n";
            viewResources(resources);
            break;
        }

        case 8: {
            // Generate reservation, utilization, and waiting-list reports.
            cout << "\n--- System Report ---\n"
                 << "Total resources: " << resources.size() << '\n'
                 << "Active reservations: " << reservations.count() << '\n';

            viewResources(resources);

            cout << "\n--- Active Reservations ---\n";
            reservations.print();

            cout << "\n--- Resource Utilization ---\n";
            for (const Resource& r : resources) {
                cout << r.getResourceName() << " (" << r.getResourceID()
                     << "): "
                     << reservations.countForResource(r.getResourceID())
                     << " active reservation(s)\n";
            }

            cout << "\n--- Most Requested Resources ---\n";
            reservations.printMostRequestedResources();

            cout << "\n--- Waiting-List Statistics ---\n";
            for (const Resource& r : resources) {
                cout << r.getResourceName() << " (" << r.getResourceID()
                     << "): "
                     << reservations.waitingCount(r.getResourceID())
                     << " student(s) waiting\n";
            }

            reservations.displayCancellationHistory();
            break;
        }

        case 9:
            cout << "Exiting Campus Resource Reservation System.\n";
            break;

        default:
            cout << "Invalid choice. Enter a number from 1 to 9.\n";
        }

    } while (choice != 9);

    // Save active reservations before exiting.
    if (!reservations.saveToFile("data/reservations.txt")) {
        cout << "Warning: could not save reservations.\n";
    }

    return 0;
}
