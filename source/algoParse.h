#ifndef algoParse_h
#define algoParse_h

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#define HUG_WINDOOR_LINE_POINTS 5 /**< Amount of points along bb8 line to be passed into detection function. */
#define HUG_WINDOOR_MAG_CBUF_SIZE 5 
#define HUG_WINDOOR_CLOSED_INDEX 0
#define HUG_TUNNEL_ERROR_AMOUNT 3


typedef struct 
{
    int16_t xMagPos;
    int16_t yMagPos;
    int16_t zMagPos;
} hug_windoor_mag_data_t;

typedef struct 
{
    hug_windoor_mag_data_t magPos;
    uint8_t windoor_state;
    uint16_t size;
} hug_windoor_state_sphere_lookup_table_t;


typedef struct 
{
    uint8_t  sumStates;                 //*Number of Config Spheres
    hug_windoor_state_sphere_lookup_table_t *p_confSphere;           //*Pointer to conf sphere for each state
    hug_windoor_mag_data_t *p_magPoint;              //*
    uint8_t magCounter;                             /**<used to emulate the cbuf on the device (not used in actual algorithm). */
    uint8_t windoor_type;
} configurators;

typedef enum
{
	HUG_WINDOOR_STATE_UNCONFIGURED = 0,			  /**< Unconfigured. */
	HUG_WINDOOR_STATE_CLOSED = 1,				  /**< Closed. */
	HUG_WINDOOR_STATE_OPEN = 2,					  /**< Open. */
	HUG_WINDOOR_STATE_MARGINALLY_OPEN = 3,		  /**< Marginally open. */
	HUG_WINDOOR_STATE_AIR_MODE = 4,				  /**< Air mode. */
	HUG_WINDOOR_STATE_TILT = 5,					  /**< Tilt position. */
	HUG_WINDOOR_STATE_LOCKED = 6,				  /**< Locked. */
	HUG_WINDOOR_STATE_UNLOCKED = 7,				  /**< Unlocked. */
	HUG_WINDOOR_STATE_AIR_MODE_LOCKED = 8,		  /**< Air mode, locked. */
	HUG_WINDOOR_STATE_AIR_MODE_UNLOCKED = 9,	  /**< Air mode, unlocked. */
	HUG_WINDOOR_STATE_UNKNOWN = 10,				  /**< Windoor state unknown. */
	HUG_WINDOOR_STATE_TAMPER_ALERT = 11,		  /**< Tampering alert. */
	HUG_WINDOOR_STATE_HUG_KEY_INSERTED = 12,	  /**< Key has been inserted. */
	HUG_WINDOOR_STATE_HUG_KEY_REMOVED = 13,		  /**< Key has been removed. */
	HUG_WINDOOR_STATE_ERROR = 14,				  /**< Error generated if the states are clashing togheter, the magnetometer is too far away. */
	HUG_WINDOOR_STATE_ERROR_PROBE_NOT_FOUND = 15, /**< Error generated when minimag/probe is not detected. */
	HUG_WINDOOR_STATE_TUNNEL_1 = 16,			  /**< State between closed + air */
	HUG_WINDOOR_STATE_TUNNEL_2 = 17,
	HUG_WINDOOR_STATE_CONFIG_STATE_RECORDED = 69, /**< Configuration state recorded, to be sent when config step is done. */
} hug_windoor_state_t;

/**
 * @brief Supported windoor types.
 * 
 */
typedef enum
{
	HUG_WINDOOR_TYPE_UNKNOWN = 0,		   /**< Unknown windoor type. */
	HUG_WINDOOR_TYPE_CASEMENT = 1,		   /**< Casement. */
	HUG_WINDOOR_TYPE_TILT = 2,			   /**< TILT - Closed, Tilt, Tilt*/
	HUG_WINDOOR_TYPE_SASH = 3,			   /**< Sash. */
	HUG_WINDOOR_TYPE_DOOR = 4,			   /**< Door. */
	HUG_WINDOOR_TYPE_SASH_W_AIR_MODE = 5,  /**< Sash with air mode. */
	HUG_WINDOOR_TYPE_CASEMENT_W_LOCK = 6,  /**< Casement with smart handle. */
	HUG_WINDOOR_TYPE_CASEMENT_MINIMAG = 7, /**< Casement using minimag only. */
	HUG_WINDOOR_TYPE_TILT_COT = 8,		   /**< TILT - Closed, Open, Tilt. */
	HUG_WINDOOR_TYPE_DOOR_ALUK = 9,		   /**< TILT - Closed, Open, Tilt. */
	HUG_WINDOOR_TYPE_CANADOOR = 11,		   /**< Canadoor */
	HUG_WINDOOR_TYPE_AMOUNT,			   /**< Amount of windoor types. */
} hug_windoor_type_t;

void setState (const char *strText, hug_windoor_state_sphere_lookup_table_t *confSphere, uint8_t i);

void setWindoor(const char *strText, uint8_t *windoorType);

void hug_windoor_state_determine(hug_windoor_mag_data_t *p_current_point, hug_windoor_state_sphere_lookup_table_t *p_sphere_lookup_table, hug_windoor_state_t *p_hug_windoor_state); //TODO: move this funcetion so it only executes once

void addToCSV(char* fileName, int16_t x, int16_t y, int16_t z, uint8_t rad, uint8_t state);

/**@brief Flag to show if mag point has been correctly configured with a closed entry pouint snf is in 'closed' state. If mag point leaves closed state, flag resets */
bool closed_flag;
/**@brief flag to show if the mag point has passed through the past entry point on the past 5 iterations */
bool tunnel_flag;
/**@brief flag to show if mag point is in the 'entry point' config sphere */
bool mag_point_flag;
/**@brief count of the sum of consecutive errors for the hit points  */
uint8_t tunnel_err_idx;
/**< IDX to track where the tunnelling state ends*/
uint8_t tun1IDX;
uint8_t tun_req;
//**< IDX for marginally open state to be set
uint8_t marginallyIDX;
//**< struct used to handle all the configuration data to configure algo
configurators handle;
FILE *fp;
uint8_t tun_count;
uint8_t *p_hug_hit_point_array;//HUG_WINDOOR_BUFFER_SIZE];
/**@brief hitpoint index for knowing how many tunnel spheres have been hit */
uint8_t hp_idx;
bool hit_point_flag;
typedef struct 
{
	uint8_t flagArr[100][100];
	uint8_t xCount;
	uint8_t yCount;
}flagArr;

flagArr flagData;
char fileName[100];

uint8_t marginallyOpenEnum;

#endif
