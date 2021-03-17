/**
 * @file hug_windoor_state.c
 * @author Danius Kalvaitis (danius@hugsmart.com)
 * @author Bart Garcia Nathan (bart.garcia.nathan@gmail.com)
 * @version 0.1
 * @date 2019-07-30
 * 
 * @copyright (c) Hug Technology 2019 All rights reserved
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include "hug_windoor_state.h"
#include <math.h>
#include "algoParse.h"



void hug_windoor_state_determine(hug_windoor_mag_data_t *p_current_point, hug_windoor_state_sphere_lookup_table_t *p_sphere_lookup_table, hug_windoor_state_t *p_hug_windoor_state) //TODO: move this funcetion so it only executes once
{
	hug_windoor_mag_data_t mag_dynamic;
	bool line_point_flag = false;
	bool marg_flag = false;		 //flag to override any clashing between marginally open & open
	hit_point_flag = false; //flag to show if the mag point has passed through desired amount of tunneling configs

	hug_windoor_flag_configure(&marg_flag, &hit_point_flag, p_current_point, p_sphere_lookup_table);
	if (!tunnel_flag || !closed_flag)
	{
		line_point_flag = hug_windoor_state_configure_line_flag(&mag_dynamic, handle.p_magPoint, &p_sphere_lookup_table[HUG_WINDOOR_CLOSED_INDEX+1]);
	}

	closed_flag = ((mag_point_flag && tunnel_flag) || (mag_point_flag && line_point_flag)) && (hit_point_flag);
	hug_windoor_state_configure_state(marg_flag, p_current_point, p_sphere_lookup_table, p_hug_windoor_state);

	printf("\n hit point =%d mag point =%d Line Point =%d tunnel =%d closed =%d marginally open =%d  \n", hit_point_flag, mag_point_flag, line_point_flag,  tunnel_flag, closed_flag, marg_flag);
	addToCSV(fileName, hit_point_flag, mag_point_flag, line_point_flag,  tunnel_flag, closed_flag, marg_flag, p_hug_windoor_state, hp_idx, tun_req);
}

void hug_windoor_flag_configure(bool *marginally_flag, bool *hp_flag, hug_windoor_mag_data_t *current_point, hug_windoor_mag_data_t *p_lookup_table)
{
	hug_windoor_hitpoint_configure(handle.p_confSphere, hp_flag);
	uint8_t *marginally_index = malloc(sizeof(uint8_t));

	switch (handle.windoor_type)
	{
	case HUG_WINDOOR_TYPE_CASEMENT:
		*marginally_index = marginallyIDX;
		break;
	case HUG_WINDOOR_TYPE_CASEMENT_W_LOCK:
		*marginally_index = marginallyIDX;
	default:
		break;
	}

	if (handle.windoor_type == (HUG_WINDOOR_TYPE_CASEMENT_W_LOCK || HUG_WINDOOR_TYPE_CASEMENT))
	{
		*marginally_flag = hug_windoor_math_sphere_check(&handle.p_confSphere[*marginally_index], current_point);
	}

	tunnel_flag = hug_windoor_math_sphere_check(&handle.p_confSphere[HUG_WINDOOR_CLOSED_INDEX + 1] , current_point);
	mag_point_flag = hug_windoor_math_sphere_check(&handle.p_confSphere[HUG_WINDOOR_CLOSED_INDEX], current_point); //flag to check if mag point is in closed state
}

bool hug_windoor_math_sphere_check(hug_windoor_state_sphere_lookup_table_t *Windoor_current_state, hug_windoor_mag_data_t *Mag_pos) //check if point is in sphere
{
	//printf("SIIIIZE = %i larger = %i \n",(pow((Mag_pos->xMagPos - Windoor_current_state->magPos.xMagPos), 2) + pow((Mag_pos->yMagPos - Windoor_current_state->magPos.yMagPos), 2) + pow((Mag_pos->zMagPos -Windoor_current_state->magPos.zMagPos), 2)),pow(Windoor_current_state->size, 2));

	int32_t x = (pow((Mag_pos->xMagPos - Windoor_current_state->magPos.xMagPos), 2) + pow((Mag_pos->yMagPos - Windoor_current_state->magPos.yMagPos), 2)+ pow((Mag_pos->zMagPos -Windoor_current_state->magPos.zMagPos), 2));

	int32_t y = (pow(Windoor_current_state->size, 2));
	// printf("x mag pos = %i, x spehre =%i \n", Mag_pos->xMagPos, Windoor_current_state->magPos.xMagPos);
	// printf("x = %i, y =%i \n",x,y);

	if ( x <= y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint16_t hug_windoor_math_distance_between_points(hug_windoor_mag_data_t *p_point1, hug_windoor_mag_data_t *p_point2)
{
	return sqrt(pow(p_point1->xMagPos - p_point2->xMagPos, 2) + pow(p_point1->yMagPos - p_point2->yMagPos, 2) + pow(p_point1->zMagPos - p_point2->zMagPos, 2));
}

bool hug_windoor_math_sphere_clash(hug_windoor_state_sphere_lookup_table_t *p_sphere_0, hug_windoor_state_sphere_lookup_table_t *p_sphere_1) //check if spheres collide
{
	return (hug_windoor_math_distance_between_points(&p_sphere_0->magPos, &p_sphere_1->magPos) < (p_sphere_0->size + p_sphere_1->size));
}

hug_windoor_mag_data_t hug_windoor_math_line_delta(hug_windoor_mag_data_t *mag_point1, hug_windoor_mag_data_t *mag_point2)
{
	hug_windoor_mag_data_t line_point;
	//printf("POINT 1 + %i POINT 2 = %i", mag_point1->xMagPos, mag_point2->xMagPos)

	line_point.xMagPos = (mag_point1->xMagPos - mag_point2->xMagPos)/HUG_WINDOOR_LINE_POINTS;
	line_point.yMagPos = (mag_point1->yMagPos - mag_point2->yMagPos)/HUG_WINDOOR_LINE_POINTS;
	line_point.zMagPos = (mag_point1->zMagPos - mag_point2->zMagPos)/HUG_WINDOOR_LINE_POINTS;

	return line_point;
}

hug_windoor_mag_data_t hug_windoor_math_point_generator(hug_windoor_mag_data_t *mag_dynamic, hug_windoor_mag_data_t *mag_point, hug_windoor_mag_data_t *delta_point, uint8_t i)
{

	mag_dynamic->xMagPos = mag_point->xMagPos - delta_point->xMagPos*i;
	mag_dynamic->yMagPos = mag_point->yMagPos - delta_point->yMagPos*i;
	mag_dynamic->zMagPos = mag_point->zMagPos - delta_point->zMagPos*i;

	//printf("x = %i y= %i z= %i mag point 1 = %i 2 = %i \n", mag_dynamic->xMagPos, mag_dynamic->yMagPos, mag_dynamic->zMagPos, handle.p_magPoint[i].xMagPos, handle.p_magPoint[i+1].xMagPos);

	return *mag_dynamic;
}

bool hug_windoor_state_configure_line_flag(hug_windoor_mag_data_t *mag_dynamic, hug_windoor_mag_data_t *mag_cbuffer, hug_windoor_state_sphere_lookup_table_t *tunnel_sphere)
{
	bool line_flag;
	uint8_t j = handle.magCounter;

	for (uint8_t i = 1; i <= HUG_WINDOOR_MAG_CBUF_SIZE; i++)
	{
		hug_windoor_mag_data_t mag_delta = hug_windoor_math_line_delta(&handle.p_magPoint[j], &handle.p_magPoint[handle.magCounter+i]);
		for (uint8_t k = 0; k <= HUG_WINDOOR_LINE_POINTS; k++)
		{
			*mag_dynamic = hug_windoor_math_point_generator(mag_dynamic, &handle.p_magPoint[j], &mag_delta, k);
			// printf("delta = %i dynamic x = %i", mag_delta.x_axis, mag_dynamic->x_axis);
			line_flag = hug_windoor_math_sphere_check(tunnel_sphere, mag_dynamic);

			if (line_flag)
			{
				return true;
			}
		}
		j++;
	}
	return false;
}

hug_windoor_state_t hug_windoor_state_configure_state(bool marginally, hug_windoor_mag_data_t *current_point, hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, hug_windoor_state_t *hug_windoor_state)
{
	uint8_t counter;
	if (closed_flag)
	{
		switch (handle.windoor_type)
		{
		case HUG_WINDOOR_TYPE_DOOR:
		case HUG_WINDOOR_TYPE_SASH_W_AIR_MODE:
		case HUG_WINDOOR_TYPE_CASEMENT_W_LOCK:
		case HUG_WINDOOR_TYPE_DOOR_ALUK:
			*hug_windoor_state = HUG_WINDOOR_STATE_LOCKED;
			break;
		case HUG_WINDOOR_TYPE_TILT:
		case HUG_WINDOOR_TYPE_TILT_COT:
		case HUG_WINDOOR_TYPE_SASH:
		case HUG_WINDOOR_TYPE_CASEMENT:
			*hug_windoor_state = HUG_WINDOOR_STATE_CLOSED;
			break;
		default:
			*hug_windoor_state = HUG_WINDOOR_STATE_UNKNOWN;
			break;
		}
	}

	/*  Iterate from table back to implement reverse priority on the boxes.
	    Closed sphere will normally be at the front of the table making it the most secure sphere. */
	else if (marginally)
	{
		*hug_windoor_state = HUG_WINDOOR_STATE_MARGINALLY_OPEN;
	}

	else
	{
		for (counter = handle.sumStates; counter >= 0; counter--)
		{
			//Test current point, break if there is a match
			if ((hug_windoor_math_sphere_check(&handle.p_confSphere[counter - 1], current_point)) && (&handle.p_confSphere[counter - 1].windoor_state == (HUG_WINDOOR_STATE_LOCKED || HUG_WINDOOR_STATE_CLOSED)) && (closed_flag != true)) //if mag point is in closed state without closed flag, send tamper
			{
				*hug_windoor_state = HUG_WINDOOR_STATE_TAMPER_ALERT;
				break;
			}

			else if ((hug_windoor_math_sphere_check(&handle.p_confSphere[counter - 1], current_point)) && (handle.p_confSphere[counter - 1].windoor_state != (HUG_WINDOOR_STATE_LOCKED || HUG_WINDOOR_STATE_CLOSED))) //TODO: see if you can remove this line and find the bug
			{
				*hug_windoor_state = handle.p_confSphere[counter - 1].windoor_state;

				if ((counter != HUG_WINDOOR_CLOSED_INDEX + 2) && (counter != HUG_WINDOOR_CLOSED_INDEX + 1) && (counter != HUG_WINDOOR_CLOSED_INDEX))
				{
					tunnel_flag = false;
				}

				if (*hug_windoor_state == HUG_WINDOOR_STATE_TUNNEL_1)
				{
					hug_windoor_tunnel_hit_check(sphere_lookup_table, counter);
				}
				break;
			}

			else if (counter == 0)
			{
				*hug_windoor_state = HUG_WINDOOR_STATE_UNKNOWN;
				printf("unknown state");
				tunnel_flag = false;

				break;
			}
		}

		if ((*hug_windoor_state != HUG_WINDOOR_STATE_TUNNEL_1) && (*hug_windoor_state != HUG_WINDOOR_STATE_CLOSED) && (*hug_windoor_state != HUG_WINDOOR_STATE_AIR_MODE_LOCKED)) //&& HUG_WINDOOR_STATE_CLOSED || HUG_WINDOOR_STATE_LOCKED))
		{
			hug_windoor_state_tunnel_error_correct(); // if mag point has left closed/locked/tunnel1 state then reset the hitpoint array
		}
	}
	printf("STAAAAAAATE =  %u \n", *hug_windoor_state);
	hug_windoor_state_save_previous_state(hug_windoor_state);

	return *hug_windoor_state;
}

void hug_windoor_tunnel_hit_check(hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, uint8_t counter)
{
	bool idx_check = true;

	for (size_t i = 0; i <= (tun1IDX - 2); i++) //checks to see if tunnel idx is already in array
	{
		if (counter == p_hug_hit_point_array[i])
		{
			idx_check = false;
			break;
		}
	}

	if (idx_check)
	{
		p_hug_hit_point_array[hp_idx] = counter;
		hp_idx++;
	}
}

void hug_windoor_hitpoint_configure(hug_windoor_state_sphere_lookup_table_t *sphere_lookup_table, bool *hp_flag)
{
	if (tun1IDX <= 7)
	{
		*hp_flag = (hp_idx >= (tun1IDX - 2)) ? true : false;
		tun_req = (tun1IDX - 2);
	}
	else
	{
		switch (handle.windoor_type)
		{
		case HUG_WINDOOR_TYPE_CASEMENT:
			*hp_flag = (hp_idx >= ((tun1IDX - 2)/ 2)-2) ? true : false;
			tun_req = ((tun1IDX - 2)/ 2)-2;
			break;
		default:
			*hp_flag = (hp_idx >= (tun1IDX - 2) / 2) ? true : false;
			tun_req = (tun1IDX - 2) / 2;
			break;
		}
	}
	printf("tun req =%u, tun1 = %u", tun_req, tun1IDX);
}

void hug_windoor_state_tunnel_error_correct()
{
	(tunnel_err_idx == HUG_TUNNEL_ERROR_AMOUNT) ? hug_windoor_initialise_hitpoint_array() : ++tunnel_err_idx;
}

void hug_windoor_initialise_hitpoint_array()
{
	p_hug_hit_point_array = (uint8_t*)calloc(tun1IDX, sizeof(uint8_t));
	hp_idx = 0;
	tunnel_err_idx = 0;
}

void hug_windoor_state_save_previous_state(uint8_t *windoor_state)
{
	state_arr[1] = state_arr[0];

	state_arr[0] = *windoor_state;

	printf("state_arr curreent = %u", state_arr[1]);
}