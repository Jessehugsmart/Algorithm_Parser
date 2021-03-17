
#include "jansson.h"
#include <string.h>
#include "algoParse.h"

//#include "hug_windoor_state.h"

json_t *magValue, *originValue, *sizeValue, *configFile, *regionsValue, *stateValue, *windoorType;
hug_windoor_state_t testState;




int main()
{
    json_error_t err;
    char* jsonFile;

    //inputJSON(jsonFile);
    printf("%s link", jsonFile);
    configFile = json_load_file("C:/Users/sasoun/Downloads/PARSETEST2.json", 0, &err);
    createCSV(fileName);
    

    if (!configFile)
    {
        printf("Parse failed!  %s \n", err.text);
        return 1;
    }


    printf("object size = %zu  array size = %zu  \n", json_object_size(regionsValue), json_array_size(regionsValue));
    printf("SUMSTATES %u", handle.sumStates);
    algoparseErrHandle(handle.sumStates);

    windoorType = json_object_get(configFile, "device");
    algoparseErrHandle(windoorType, __LINE__);
    windoorType = json_object_get(windoorType, "windoorType");
    algoparseErrHandle(windoorType, __LINE__);
    setWindoor(json_string_value(windoorType), &handle.windoor_type);

    regionsValue = json_object_get(configFile, "regions");
    algoparseErrHandle(regionsValue, __LINE__);
    handle.sumStates = json_array_size(regionsValue);     //set number of conf states
    handle.p_confSphere = (hug_windoor_state_sphere_lookup_table_t *)calloc(handle.sumStates, sizeof(hug_windoor_state_sphere_lookup_table_t));


    //windoorType = json_array_get(windoorType, 0);


    printf("Windoor type = %u", handle.windoor_type);


    for (uint16_t i = 0; i < handle.sumStates; i++) //Loop for collectng all config points
    {
        originValue = json_array_get(regionsValue, i);
        algoparseErrHandleLoop(originValue, __LINE__);

        sizeValue = json_object_get(originValue, "size");
        handle.p_confSphere[i].size = json_integer_value(sizeValue);
        algoparseErrHandleLoop(sizeValue, __LINE__);


        stateValue = json_object_get(originValue, "state");
        printf("json string = %s \n", json_string_value(stateValue));
        setState(json_string_value(stateValue), &handle.p_confSphere[i], i);
        algoparseErrHandleLoop(stateValue, __LINE__);        

        originValue = json_object_get(originValue, "origin");
        printf("ID size = %zi \n", json_integer_value(originValue));
        algoparseErrHandleLoop(originValue, __LINE__);

        magValue = json_object_get(originValue, "x");
        algoparseErrHandleLoop(magValue, __LINE__);

        handle.p_confSphere[i].magPos.xMagPos = json_integer_value(magValue);

        magValue = json_object_get(originValue, "y");
        handle.p_confSphere[i].magPos.yMagPos = json_integer_value(magValue);
        algoparseErrHandleLoop(magValue, __LINE__);

        magValue = json_object_get(originValue, "z");
        handle.p_confSphere[i].magPos.zMagPos = json_integer_value(magValue);
        algoparseErrHandleLoop(magValue, __LINE__);

        printf("%u x = %i, y= %i, z= %i size = %i State = %u \n", i, handle.p_confSphere[i].magPos.xMagPos, 
                                                                handle.p_confSphere[i].magPos.yMagPos, handle.p_confSphere[i].magPos.zMagPos, handle.p_confSphere[i].size, handle.p_confSphere[i].windoor_state);
        addToCSV(fileName, handle.p_confSphere[i].magPos.xMagPos, handle.p_confSphere[i].magPos.yMagPos, handle.p_confSphere[i].magPos.zMagPos,  handle.p_confSphere[i].size, handle.p_confSphere[i].windoor_state);

    }

    regionsValue = json_object_get(configFile, "points");
    handle.p_magPoint = (hug_windoor_mag_data_t*)calloc(json_array_size(regionsValue), sizeof(hug_windoor_mag_data_t));

    printf("array size = %zu",json_array_size(regionsValue));

    for (size_t i = 0; i < json_array_size(regionsValue); i++)
    {
        originValue = json_array_get(regionsValue, i);
        originValue = json_object_get(originValue, "position");
        magValue = json_object_get(originValue, "x");

        handle.p_magPoint[i].xMagPos = json_integer_value(magValue);
        magValue = json_object_get(originValue, "y");
        handle.p_magPoint[i].yMagPos = json_integer_value(magValue);

        magValue = json_object_get(originValue, "z");
        handle.p_magPoint[i].zMagPos = json_integer_value(magValue);
        printf("%zu x = %i, y= %i, z=%i \n", i, handle.p_magPoint[i].xMagPos, handle.p_magPoint[i].yMagPos, handle.p_magPoint[i].zMagPos);
        addToCSV(fileName, handle.p_magPoint[i].xMagPos, handle.p_magPoint[i].yMagPos, handle.p_magPoint[i].zMagPos,  0, 0);

    }
    hug_windoor_initialise_hitpoint_array();    

/*     for (handle.magCounter = 0; handle.magCounter <= json_array_size(regionsValue); handle.magCounter++) //TODO:fix these
    {
    } */


    return 0;
}