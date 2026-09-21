#include "Resource.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

Resource::Resource()
{
    resourceID = "";
    resourceName = "";
    resourceType = "";
    available = true;
}

Resource::Resource(string id, string name, string type, bool availability)
{
    resourceID = id;
    resourceName = name;
    resourceType = type;
    available = availability;
}

string Resource::getResourceID() const
{
    return resourceID;
}

string Resource::getResourceName() const
{
    return resourceName;
}

string Resource::getResourceType() const
{
    return resourceType;
}

bool Resource::getAvailability() const
{
    return available;
}

void Resource::setAvailability(bool availability)
{
    available = availability;
}

vector<Resource> loadResources(string filename)
{
    vector<Resource> resources;
    ifstream file(filename);

    if (!file)
    {
        cout << "Error opening resource file." << endl;
        return resources;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string id;
        string name;
        string type;
        string availability;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, availability, ',');

        bool available = (availability == "1");

        resources.push_back(Resource(id, name, type, available));
    }

    file.close();

    return resources;
}

Resource* searchResource(vector<Resource>& resources, string resourceID)
{
    for (Resource& resource : resources)
    {
        if (resource.getResourceID() == resourceID)
        {
            return &resource;
        }
    }

    return nullptr;
}

int partition(vector<Resource>& resources, int low, int high, int criteria)
{
    string pivot;

    if (criteria == 1)
    {
        pivot = resources[high].getResourceID();
    }
    else if (criteria == 2)
    {
        pivot = resources[high].getResourceName();
    }
    else if (criteria == 3)
    {
        pivot = resources[high].getResourceType();
    }
    else
    {
        pivot = resources[high].getAvailability() ? "1" : "0";
    }

    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        string current;

        if (criteria == 1)
        {
            current = resources[j].getResourceID();
        }
        else if (criteria == 2)
        {
            current = resources[j].getResourceName();
        }
        else if (criteria == 3)
        {
            current = resources[j].getResourceType();
        }
        else
        {
            current = resources[j].getAvailability() ? "1" : "0";
        }

        if (current < pivot)
        {
            i++;
            swap(resources[i], resources[j]);
        }
    }

    swap(resources[i + 1], resources[high]);

    return i + 1;
}

void quickSort(vector<Resource>& resources, int low, int high, int criteria)
{
    if (low < high)
    {
        int pivotIndex = partition(resources, low, high, criteria);

        quickSort(resources, low, pivotIndex - 1, criteria);
        quickSort(resources, pivotIndex + 1, high, criteria);
    }
}

void sortResources(vector<Resource>& resources, int criteria)
{
    if (!resources.empty())
    {
        quickSort(resources, 0, resources.size() - 1, criteria);
    }
}
