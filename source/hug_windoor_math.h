/**
 * @file hug_windoor_math.h
 * @author Danius Kalvaitis (danius@hugsmart.com)
 * @brief Hug windoor mathematical functions to help with windoor state configuration and detection.
 * @version 0.1
 * @date 2019-07-30
 * 
 * @copyright (c) Hug Technology 2019 All rights reserved
 * 
 * @defgroup    hug_windoor_math    Mathematical functions
 * @{
 * @ingroup     hug_windoor_state_detection
 */

#ifndef HUG_WINDOOR_MATH
#define HUG_WINDOOR_MATH


#include <stdbool.h>

#define HUG_WINDOOR_MIN_BOX_SIZE 25  /**< Minimum allowed box size. */
#define HUG_WINDOOR_MAX_BOX_SIZE 100 /**< MAXIMUM allowed box size. */
#define HUG_WINDOOR_MAX_TUNNEL_SIZE 35 /**< MAXIMUM allowed box size. */
#define HUG_WINDOOR_LINE_POINTS 5 /**< Amount of points along bb8 line to be passed into detection function. */

/**
 * @brief       Calculate distance between two magnetic 3D points.
 * 
 * @param[in]   p_point1    Pointer to first 3D magnetic point.
 * @param[in]   p_point2    Pointer to second 3D magnetic point.
 * 
 * @return      uint16_t    Distance between two 3D magnetic points.
 */
uint16_t hug_windoor_math_distance_between_points(hug_windoor_mag_data_t *p_point1, hug_windoor_mag_data_t *p_point2);
/**
 * @brief       Calculate center point between two magnetic 3D points.
 * 
 * @param[in]   p_point0        Pointer to magnetic point 0.
 * @param[in]   p_point1        Pointer to magnetic point 1.
 * @param[out]  p_center_point  Pointer to where the calculated center point will be written.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      NRF_ERROR_NULL  If p_point0 or p_point1 or p_center_point is NULL.
 */
ret_code_t hug_windoor_math_center_between_two_points(hug_windoor_mag_data_t *p_point0, hug_windoor_mag_data_t *p_point1, hug_windoor_mag_data_t *p_center_point);

/**
 * @brief           Set specified sphere size.
 * 
 * @details         Updates the sphere size and min max thresholds.
 * 
 * @param[in,out]   p_sphere           Pointer to a sphere
 * @param[in]       sphere_size_mult        Sphere multiplier.
 * 
 * @retval          NRF_SUCCESS     On success.
 * @retval          NRF_ERROR_NULL  If p_sphere was NULL.
 */
ret_code_t hug_windoor_math_set_sphere_size(hug_windoor_state_sphere_centre_t *p_sphere, uint16_t sphere_size_mult);


ret_code_t hug_windoor_math_set_radius(hug_windoor_state_sphere_centre_t *p_sphere, uint16_t Rad_sphere);


/**
 * @brief           Set center point of specified sphere.
 * 
 * @param[in,out]   p_sphere           Pointer to a sphere.
 * @param[in]       p_sphere_center    Pointer to a new sphere center.
 * 
 * @retval          NRF_SUCCESS     On success.
 * @retval          NRF_ERROR_NULL  If p_sphere or p_sphere_center is NULL.
 */
ret_code_t hug_windoor_set_sphere_centre(hug_windoor_state_sphere_centre_t *p_sphere, hug_windoor_mag_data_t *p_sphere_centre);

/**
 * @brief           Get magnitude of magnetic point.
 * 
 * @param[in]       p_mag_data  Pointer to magnetic point.
 * 
 * @return          uint16_t    Magnetic point magnitude.
 */
uint16_t hug_windoor_math_get_mag_point_magnitude(hug_windoor_mag_data_t *p_mag_data);

/**
 * @brief           Generate sphere size.
 * 
 * @details         Uses distance and multiplier value to generate a sphere size.
 * 
 * @param[in]       distance    Distance between magnetic points.
 * @param[in]       sphere_mult    sphere multiplier value.
 * 
 * @return          uint16_t    sphere size.
 */
uint16_t hug_windoor_math_generate_sphere_size(uint16_t distance, float sphere_mult);
uint16_t hug_windoor_math_generate_box_size(double distance, double box_mult);
uint16_t hug_windoor_math_generate_sphere_tunnel_size(uint16_t distance);

/**
 * @brief           check if point falls on a line between 2 seperate points.
 * 
 * @details         uses algorithm to determine whether given point is on a line.
 * 
 * @param[in]       mag_point1    magnetometer data for start of the line.
 * @param[in]       mag_point2    magnetometer data for end of the line.
 * @param[in]       check_point    point to be checked against the line.
 * 
 * @return          bool          if true, point is on the line
 */
bool hug_windoor_math_check_line(hug_windoor_mag_data_t mag_point1, hug_windoor_mag_data_t mag_point2, hug_windoor_mag_data_t check_point);
/**
 * @brief           calulates difference between two 3D points.
 * 
 * @note            This function is used to calculate the difference between two 3D points, the result is then divided by the amount of times that points are to be
 *                  checked along the line between the two.
 * 
 * @param[in]      mag_point1     pointer to mag point at the head of the cbuf
 * @param[in]      mag_point2     pointer to mag point at the tail of cbuf
 * @param[out]      line_point    XYZ values for difference between the mag points divided by number of points to check
 * 
 * @retval          NRF_SUCCESS         On success.
 * @retval          NRF_ERROR_NULL      If p_config_buffer or p_sphere_lookup_table was NULL.
 */
hug_windoor_mag_data_t hug_windoor_math_line_delta(hug_windoor_mag_data_t *mag_point1, hug_windoor_mag_data_t *mag_point2);

/**
 * @brief           generates various points along a line between two seperate poitns
 * 
 * @note            returns multiple points incrementally based on the difference between two XYZ points.
 * 
 * @param[in]       mag_dynamic     pointer to array to save calculated data
 * @param[in]       mag_point       pointer to head value in cbuff
 * @param[in]       delta_point     pointer to delta point 
 * 
 * @retval          NRF_SUCCESS         On success.
 * @retval          NRF_ERROR_NULL      If p_config_buffer or p_sphere_lookup_table was NULL.
 */
hug_windoor_mag_data_t hug_windoor_math_point_generator(hug_windoor_mag_data_t *mag_dynamic, hug_windoor_mag_data_t *mag_point, hug_windoor_mag_data_t *delta_point, uint8_t i);

/**
 * @brief           Save averaged configuration.
 * 
 * @note            Use this function to save static data sphere configuration.
 * 
 * @param[in]       p_config_buffer     Pointer to configuration buffer that contains collected raw data.
 * @param[out]      p_sphere_lookup_table  Pointer to sphere lookup table where the configuration will be saved in a form of new sphere.
 * @param[in]       windoor_state       Windoor state that will be assigned to the new sphere.
 * 
 * @retval          NRF_SUCCESS         On success.
 * @retval          NRF_ERROR_NULL      If p_config_buffer or p_sphere_lookup_table was NULL.
 */
ret_code_t hug_windoor_math_save_average_config(hug_windoor_sphere_config_buffer_t *p_config_buffer, hug_windoor_state_sphere_lookup_table_t *p_sphere_lookup_table, hug_windoor_state_t windoor_state);

#endif // HUG_WINDOOR_MATH

/** @} */



