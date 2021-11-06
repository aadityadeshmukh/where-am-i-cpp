#include <iostream>
using namespace std;

#define API_URL "https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyApvjl7RQfOkuOX3oRhGED0NEx_f1NP7A0"

class GLRequest{
public:
    void getLocationInfo(char * jsonPayload);
    void printLocationInfo(char * response);

};