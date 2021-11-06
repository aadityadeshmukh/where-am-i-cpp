#include <iostream>
#include <cstring>
#include <string>

#include <C:\lcurl\include\curl\curl.h> // curl support https://curl.haxx.se/
using namespace std;

#define API_URL "https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyApvjl7RQfOkuOX3oRhGED0NEx_f1NP7A0"

class GLRequest{
public:
    void getLocationInfo(char * jsonPayload);
    void printLocationInfo(char * response);
private:
    struct MemoryStruct{
        char* memory;
        size_t size;
    };

    static size_t writeMemoryCallback(void *contents,
	                                  size_t size,
									  size_t nmemb,
									  void *userp);
};