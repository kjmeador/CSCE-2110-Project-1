#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>

using namespace std;

class Resource
{
private:
    string resourceID;
    string resourceName;
    string resourceType;
    bool available;

public:
    Resource();
    Resource(string id, string name, string type, bool availability);

    string getResourceID() const;
    string getResourceName() const;
    string getResourceType() const;
    bool getAvailability() const;

    void setAvailability(bool availability);
};

vector<Resource> loadResources(string filename);

Resource* searchResource(vector<Resource>& resources, string resourceID);

void sortResources(vector<Resource>& resources, int criteria);

#endif
