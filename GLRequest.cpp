#include "GLRequest.hpp"
#include "cJSON.h"

void GLRequest::printLocationInfo(char * response){
    const cJSON * location = NULL;
    const cJSON * lat = NULL;
    const cJSON * lng = NULL;
    const cJSON * accuracy = NULL;

    cJSON* parsedJSON = cJSON_Parse(response);
    if(parsedJSON == NULL)
    {
        const char *errorPtr = cJSON_GetErrorPtr();
		if (errorPtr != NULL) 
		{
			cerr << "ERROR: cJSON_GetErrorPty returned: " << errorPtr << endl;
		}
        goto end;
    }

    location = cJSON_GetObjectItemCaseSensitive(parsedJSON, "location");
    if(location == NULL)
        goto end;
    lat = cJSON_GetObjectItemCaseSensitive(location, "lat");
    lng = cJSON_GetObjectItemCaseSensitive(location, "lng");
    accuracy = cJSON_GetObjectItemCaseSensitive(parsedJSON, "accuracy");
    if((lat != NULL) && (lng != NULL) && (lat != NULL) &&(accuracy != NULL) ){
        cout << "You are within " << cJSON_Print(accuracy) << "m of" << cJSON_Print(lat) << "N " << cJSON_Print(lng) << "E" << endl;
    }
    end:
        cJSON_Delete(parsedJSON);
}

void GLRequest::getLocationInfo(char *jsonPayload)
{
	// initialize curl 
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = curl_easy_init();

	// sumbit POST request to Google Geolocation API
	if (curl) 
	{
		// set content type 
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		
		// memory to store POST response 
		struct MemoryStruct chunk;
		chunk.memory = NULL;
		chunk.size = 0; // memory will be grown as needed by writeMemoryCallback() function

		// set curl options 
		curl_easy_setopt(curl, CURLOPT_URL, API_URL);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback); // send all data to this function
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); // pass 'chunk' struct to callback

		// Perform the request, res will get the return code 
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) 
		{
			cerr << "ERROR: curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		}
		else 
		{
			// chunk.memory points to a memory block that is chunk.size bytes big and contains the response
			printLocationInfo(chunk.memory);
		}

		// cleanup libcurl
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		free(chunk.memory);
	}
	else
	{
		cerr << "ERROR: curl_easy_init() failed" << endl;
	}
}