#include "algoParse.h"
#include "jansson.h"
#include <string.h>


void setWindoor(const char *strText, uint8_t *windoorType)
{
    if (strcmp(strText, "Casement") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_CASEMENT;
    }

    else if (strcmp(strText, "Unknown") == 0)
    {
        *windoorType = HUG_WINDOOR_STATE_UNKNOWN;
    }

    else if (strcmp(strText, "Tilt") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_TILT;
    }
    else if (strcmp(strText, "Sash") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_SASH;
    }
    else if (strcmp(strText, "Door") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_DOOR;
    }
    else if (strcmp(strText, "Sash with Air Mode") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_SASH_W_AIR_MODE;
    }
    else if (strcmp(strText, "Casement with Lock") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_CASEMENT_W_LOCK;
    }
    else if (strcmp(strText, "Casement MiniMag") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_CASEMENT_MINIMAG;
    }
    else if (strcmp(strText, "Tilt COT") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_TILT_COT;
    }
    else if (strcmp(strText, "ALUK Door") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_DOOR_ALUK;
    }
    else if (strcmp(strText, "Canadoor") == 0)
    {
        *windoorType = HUG_WINDOOR_TYPE_CANADOOR;
    }
}

void setState(const char *strText, hug_windoor_state_sphere_lookup_table_t *confSphere, uint8_t i)
{
    if (strcmp(strText, "Unconfigured") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_UNCONFIGURED;
    }
  
    else if (strcmp(strText, "Closed") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_CLOSED;
    } 
    else if (strcmp(strText, "Open") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_OPEN;
    }
    else if (strcmp(strText, "MarginallyOpen") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_MARGINALLY_OPEN;
        marginallyIDX = i;
    }
    else if (strcmp(strText, "AirMode") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_AIR_MODE;
    }
    else if (strcmp(strText, "Tilt") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_TILT;
    }
    else if (strcmp(strText, "Locked") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_LOCKED;
    }
    else if (strcmp(strText, "Unlocked") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_UNLOCKED;
    }
    else if (strcmp(strText, "AirModeUnocked") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_TUNNEL_1;
        tun1IDX = i;
    }
    else if (strcmp(strText, "AirModeLocked") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_TUNNEL_1;
        tun1IDX = i;
    }
    else if (strcmp(strText, "BetweenClosedAndAirMode") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_TUNNEL_1;
        tun1IDX = i;
    }
    else if (strcmp(strText, "Unknown") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_UNKNOWN;
    }
    else if (strcmp(strText, "TamperAlert") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_TAMPER_ALERT;
    }
    else if (strcmp(strText, "HugKeyInserted") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_HUG_KEY_INSERTED;
    }
    else if (strcmp(strText, "HugKeyRemoved") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_HUG_KEY_REMOVED;
    }
    else if (strcmp(strText, "Error") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_ERROR;
    }
    else if (strcmp(strText, "ProbeNotFound") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_ERROR_PROBE_NOT_FOUND;
    }
    else if (strcmp(strText, "ConfigStateRecorded") == 0)
    {
        confSphere->windoor_state = HUG_WINDOOR_STATE_CONFIG_STATE_RECORDED;
    }
    printf("STATE = %u", confSphere->windoor_state);
}

void algoparseErrHandle(int err, int line)
{
    
    if (!err == NULL)
    {
        printf("SUCCESS Line %d\n",line);        
    }
    
    else
    {
        printf("ERROR PARAM NULL %d\n",line);
    }
    
}

void algoparseErrHandleLoop(int err, int line)
{
    if (err ==NULL )
    {
        printf("ERROR PARAM NULL LOOP %d\n",line);
    }
    
}


 
void configCSV(char* fileName)
{
    char* path = "./../../../Output/";

    fileName= strcat(fileName,".csv");
    fileName= strcat(path, fileName);
    fp=fopen(fileName,"w+");

    fprintf(fp,"x, y, z, Rad, State");

    //fclose(fp);

    printf("\n %s file created",fileName);

}

void inputJSON(char* fileName)
{
    char* path = "./../../../Input/";

    gets(fileName);
    fileName= strcat(fileName,".json");

    fileName= strcat(path, fileName);

    printf("\n %s file created",fileName);

}

void createCSV(char* fileName)
{
    printf("\n Enter the filename :");

    gets(fileName);

    printf("\n Creating %s.csv file", fileName);

    configCSV(fileName);
}



void addToCSV(char* fileName, int16_t x, int16_t y, int16_t z, uint8_t rad, uint8_t state)
{
    flagData.yCount++;

    fprintf(fp,"\n %i, %i, %i, %u, %u",x, y, z, rad, state);

}


