#include <iostream>
#include "GLWifiPoints.hpp"
#include "GLRequest.hpp"
using namespace std;

int main() {
	GLWifiPoints wifiAccess;
	wifiAccess.getWifiNetworks();
	char* jsonPayLoad = wifiAccess.buildRequestJSON();

	GLRequest req;
	req.getLocationInfo(jsonPayLoad);

	return 0;
}