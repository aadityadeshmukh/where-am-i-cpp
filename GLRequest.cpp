#include "GLRequest.hpp"
#include "cJSON.h"

void GLRequest::printLocationInfo(char * response){
    const cJSON * location = NULL;
    const cJSON * lat = NULL;
    const cJSON * lng = NULL;
    const cJSON * accuracy = NULL;

    cJSON* parsedJSON = cJSON_Parse(response);
    if(parsedJSON == NULL)
        goto end;

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