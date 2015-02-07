/*
 * touch.c
 *
 * Created: 24/01/2015 19:59:26
 *  Author: Mahe
 */ 

#include <asf.h>
#include "touch.h"
#include "board.h"
#include "ioport.h"
#include "rtouch.h"
#include "rtouch_calibrate.h"
#include "resistive_touch_board.h"
#include "ads7843.h"

uint32_t g_ul_width = ILI93XX_LCD_WIDTH;
uint32_t g_ul_height = ILI93XX_LCD_HEIGHT;

void touch_init() {
	uint8_t i;
	for(i=0; i<10; ++i) {
		memset(tname[i],0,100);
		memset(tfunction[i],0,sizeof(tfunction[i]));	
	}
}
/**
void get_point(rtouch_point_t *p_raw, rtouch_point_t *p_point) {
	int32_t point_x,point_y;
	point_x = calibration_point_0.panel.x
	- (((int32_t) calibration_point_0.raw.x - (int32_t) p_raw->x)
	* MULTI_FACTOR) / g_l_xslope;

	point_y = calibration_point_0.panel.y
	- (((int32_t) calibration_point_0.raw.y - (int32_t) p_raw->y)
	* MULTI_FACTOR) / g_l_yslope;

	p_point->x = (point_x > 0) ? point_x : 0;
	p_point->x = (p_point->x > g_ul_width) ? g_ul_width : p_point->x;
	p_point->y = (point_y > 0) ? point_y : 0;
	p_point->y = (p_point->y > g_ul_height) ? g_ul_height : p_point->y;
}
**/
struct touch* find_touch(void) {
	uint32_t x, y, val, X, Y;
	
	rtouch_wait_pressed();
	rtouch_get_raw_point(&x, &y);
	rtouch_wait_released();
	
	rtouch_point_t raw, actual;
	raw.x = x;
	raw.y = y;
	
//	get_point(&raw, &actual);
	val = 9-(raw.y-150)/180;
	 T.tfunction = tfunction[val];
	 strcpy(T.tname ,tname[val]);
	 return &T;
}
