#include "GLWifiPoints.hpp"
#include "cJSON.h"
using namespace std;

int GLWifiPoints::getWifiNetworks(){
	cout << "Fetching Wifi access points." << endl;

	//open handle
	//The WlanOpenHandle function opens a connection to the server.
	m_wifiAP accessPoint = {"00:00:00:00:00:00", 0};
	DWORD idwMaxClient = 2;
	DWORD oCurVersion = 0;
	HANDLE oClientHandle = 0;
	DWORD dwResult = 0;
	DWORD dwBssResult = 0;
	DWORD retVal = 0;
	dwResult = WlanOpenHandle(idwMaxClient, NULL, &oCurVersion, &oClientHandle);
	if(dwResult != ERROR_SUCCESS){
		cerr << "Error: WLAN Open Handle returned: " << dwResult << endl;
	}
	//The WlanEnumInterfaces function enumerates all of the wireless LAN interfaces currently enabled on the local computer.
  	PWLAN_INTERFACE_INFO_LIST opInterfaceList  = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;
	PWLAN_BSS_LIST    opWlanBssList = NULL;
	PWLAN_BSS_ENTRY pBssEntry = NULL;
	dwResult = WlanEnumInterfaces(oClientHandle, NULL, &opInterfaceList);
	if(dwResult != ERROR_SUCCESS){
		cerr << "Error: Fetching interface list failed with error: " << dwResult << endl;
	}
	else{
		for(int i = 0; i < (int)opInterfaceList->dwNumberOfItems; i++){
			pIfInfo = (PWLAN_INTERFACE_INFO)&opInterfaceList->InterfaceInfo[i];
			//get network BSS list
			//function retrieves a list of the basic service set (BSS) entries of the wireless network or networks on a given wireless LAN interface.
			dwBssResult = WlanGetNetworkBssList(oClientHandle, &pIfInfo->InterfaceGuid, NULL, dot11_BSS_type_any, 0, 0, &opWlanBssList);
			if(dwBssResult != ERROR_SUCCESS){
				cerr << "ERROR: Fetching BSS results returned code: " << dwBssResult << endl;
				retVal = 1;
 			}
			else{
				//found bss list
				cout << "Found " << opWlanBssList->dwNumberOfItems << " access points." << endl;
				for(int j = 0; j < opWlanBssList->dwNumberOfItems; j++){
					pBssEntry = (PWLAN_BSS_ENTRY)&opWlanBssList->wlanBssEntries[j];
					sprintf(accessPoint.macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", 
							pBssEntry->dot11Bssid[0], 
							pBssEntry->dot11Bssid[1],
							pBssEntry->dot11Bssid[2],
							pBssEntry->dot11Bssid[3],
							pBssEntry->dot11Bssid[4],
							pBssEntry->dot11Bssid[5]);
					accessPoint.signalStrength = pBssEntry->lRssi;
					cout << accessPoint.macAddress << endl;
					cout << accessPoint.signalStrength << endl;
					m_wifiAPList.push_back(accessPoint);
				}
			}
		}
	}

	//
	return retVal;
}

char* GLWifiPoints::buildRequestJSON(){
	cJSON * pWifiAccessList = NULL;
	char* outJSON = NULL;
	
	cJSON * locationReq = cJSON_CreateObject();
	if(cJSON_AddStringToObject(locationReq, "considerIp", "true") == NULL){
		//problem
		goto end;
	}

	pWifiAccessList = cJSON_AddArrayToObject(locationReq, "wifiAccessPoints");
	if(pWifiAccessList == NULL)
	{
		//problem
		goto end;
	}

	for(m_wifiAP ap : m_wifiAPList){
		cJSON * accessPt = cJSON_CreateObject();
		if(cJSON_AddStringToObject(accessPt, "macAddress", ap.macAddress) == NULL){
			//problem
			goto end;
		}
		if(cJSON_AddNumberToObject(accessPt, "signalStrength", ap.signalStrength) == NULL){
			//problem
			goto end;
		}

		cJSON_AddItemToArray(pWifiAccessList, accessPt);
		outJSON = cJSON_Print(locationReq);
		if(outJSON == NULL) 
		{
			cerr << "ERROR: Failed to print JSON for the wifi access points" << endl;
		}
	}

	end:
		cJSON_Delete(locationReq);

	return outJSON;
}