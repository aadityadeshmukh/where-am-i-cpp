#include <iostream>
#include <list>
#include <windows.h>
#include <wlanapi.h>
using namespace std;

class GLWifiPoints{
	public:
	int getWifiNetworks();
	private:
	struct m_wifiAP{
		char macAddress[44];
		long signalStrength;
	};
	list<m_wifiAP> m_wifiAPList;
};