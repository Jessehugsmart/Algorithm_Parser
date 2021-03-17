/**
 * @file hug_windoor_state.h
 * @author Danius Kalvaitis (danius@hugsmart.com)
 * @author Bart Garcia Nathan (bart.garcia.nathan@gmail.com)
 * @brief Functions for sphere lookup table validation and windoor state identification.
 * @version 0.1
 * @date 2019-07-30
 * 
 * @copyright (c) Hug Technology 2019 All rights reserved
 * 
 * @defgroup    hug_windoor_state   sphere lookup table state detection
 * @{
 * @ingroup     hug_windoor_state_detection
 */

#ifndef HUG_WINDOOR_STATE_H
#define HUG_WINDOOR_STATE_H

#include <stdbool.h> 
#include <inttypes.h>
#include "algoParse.h"

#define HUG_WINDOOR_CLOSED_INDEX 0
#define HUG_TUNNEL_ERROR_AMOUNT 3



/**@brief Flag to show if mag point has been correctly configured with a closed entry pouint snf is in 'closed' state. If mag point leaves closed state, flag resets */
bool closed_flag;
/**@brief flag to show if the mag point has passed through the past entry point on the past 5 iterations */
bool tunnel_flag;
/**@brief flag to show if mag point is in the 'entry point' config sphere */
bool mag_point_flag;
/**@brief count of the sum of consecutive errors for the hit points  */
uint8_t tunnel_err_idx;
/**@brief array to save the previous state of the windoor */
uint8_t state_arr[2];


/**
 * @brief      configure flags for state detection
 * 
 * @details     Configures flags for 
 * 
 * @param[in]   p_current_point     Pointer to magnetic point.
 * @param[in]   p_sphere_lookup_table  Pointer to sphere lookup table.
 * @param[out]  p_hug_windoor_state Pointer to where the windoor state will be returned. Returns @ref HUG_WINDOOR_STATE_UNKNOWN is the magnetic point was not found in any of the spherees.
 * 
 * @retval      NRF_SUCCESS         On success.
 * @retval      NRF_ERROR_NULL      If p_current_point or p_sphere_lookup_table or p_hug_windoor_state was NULL.
 */
void hug_windoor_flag_configure(bool *marginally_flag, bool *hp_flag, hug_windoor_mag_data_t *current_point, hug_windoor_mag_data_t *p_lookup_table);


/**
 * @brief       Determine the windoor state.
 * 
 * @details     Iterates through sphere lookup table until finds a sphere in which the specified magnetic point is.
 * 
 * @param[in]   p_current_point     Pointer to magnetic point.
 * @param[in]   p_sphere_lookup_table  Pointer to sphere lookup table.
 * @param[out]  p_hug_windoor_state Pointer to where the windoor state will be returned. Returns @ref HUG_WINDOOR_STATE_UNKNOWN is the magnetic point was not found in any of the spherees.
 * 
 * @retval      NRF_SUCCESS         On success.
 * @retval      NRF_ERROR_NULL      If p_current_point or p_sphere_lookup_table or p_hug_windoor_state was NULL.
 */
void hug_windoor_state_determine(hug_windoor_mag_data_t * p_current_point, hug_windoor_state_sphere_lookup_table_t * p_sphere_lookup_table, hug_windoor_state_t * p_hug_windoor_state);

/**
 * @brief       Check if mag point is in sphere.
 * 
 * @details     checks current state of windoor to see if mag pos has moved outside the state.
 * 
 * @param[in]   Windoor_current_state     Pointer to windoor current state coordinates.
 * @param[in]   Mag_pos  Pointer to current mag position.
 * @return[out]  returns true if mag data is still inside sphere
 */
bool hug_windoor_math_sphere_check(hug_windoor_state_sphere_lookup_table_t *Windoor_current_state, hug_windoor_mag_data_t *Mag_pos); //check if point is in sphere

/**
 * @brief       Check if spheres clash.
 * 
 * @details     checks two sphere states to see if clashing, used to check BB8 tunnelling between state '0' and initial tunnelling sphere.
 * 
 * @param[in]   p_sphere_0     Pointer to sphere state.
 * @param[in]   p_sphere_1     Pointer to sphere state.
 * @return[out]  returns true if state spheres are clashing
 */
bool hug_windoor_math_sphere_clash(hug_windoor_state_sphere_lookup_table_t *p_sphere_0, hug_windoor_state_sphere_lookup_table_t *p_sphere_1); //check if spheres collide
/**
 * @brief       Check if past five points have passed through the entry point.
 * 
 * @details     draws a line through the past 5 mag points, then divides the line into multiple points to check if they have passed through the entry sphere.
 * @param[in]   *mag_dynamic     Pointer to sphere state.
 * @param[in]   *mag_cbuffer     Pointer to circular buffer.
 * @param[in]   *tunnel_sphere   Pointer to entry sphere
 * @return[out]  returns true if line has passed through the entry state
 */
bool hug_windoor_state_configure_line_flag(hug_windoor_mag_data_t *mag_dynamic, hug_windoor_mag_data_t *mag_cbuffer, hug_windoor_mag_data_t *tunnel_sphere);

/**
 * @brief       updates state characteristic .
 * 
 * @details     Uses configured flags to update the state characteristic.
 * 
 * @param[in]   marginally     Pointer to sphere state.
 * @param[in]   *current_point     Pointer to circular buffer.
 * @param[in]   *sphere_lookup_table   Pointer to entry sphere.
 * @param[in]   *hug_windoor_state       Pointer to current windoor state.
 * @return[out]  returns windoor state.
 */
hug_windoor_state_t hug_windoor_state_configure_state(bool marginally, hug_windoor_mag_data_t *current_point, hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, hug_windoor_state_t *hug_windoor_state);

/**
 * @brief       checks tunnel sphere array
 * 
 * @details     checks tunnel sphere array to see if the tunnel sphere has previously been crossed and if not, it is added to the array.
 * 
 * @param[in]   *sphere_lookup_table   Pointer to entry sphere.
 * @param[in]    counter 
 */
void hug_windoor_tunnel_hit_check(hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, uint8_t counter);
/**
 * @brief       configures hitpoint flag.
 * 
 * @details     raises the tunnel flag if the given amount of the tunnel config spheres have been hit
 * 
 * @param[in]   *sphere_lookup_table   Pointer to entry sphere.
 * @param[in]    *hit_flag. 
 */
void hug_windoor_hitpoint_configure(hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, bool *hp_flag);

/**
 * @brief       ignores any minor errors when the mag point leaves the tunnel config
 * 
 * @details     this function stops the tunnel idx from resetting every time the mag point leaves the tunnelling section. The idx will reset only If the mag point
 *              has left the tunnelling config for 3 consequetive readings.
 * 
 * @param[in]   *sphere_lookup_table   Pointer to entry sphere.
 * @param[in]    *hit_flag. 
 */
void hug_windoor_state_tunnel_error_correct();

void hug_windoor_initialise_hitpoint_array();

void hug_windoor_state_save_previous_state(uint8_t *windoor_state);





#endif // HUG_WINDOOR_STATE_H

/** @} */
