/*
 * display.c
 *
 * Created: 24/01/2015 16:29:14
 *  Author: Mahe
 */ 

#include "stdint-gcc.h"
#include "display.h"
#include <ili93xx.h>

void add_list(const uint8_t *p, uint8_t priority) {
	ili93xx_set_foreground_color(LIST_COLOR);
	ili93xx_draw_string(current_x + (1 +priority)*PADDING, current_y + PADDING, p);
	ili93xx_draw_line(current_x + PADDING, current_y + LIST_HEIGHT, current_x + LIST_WIDTH , current_y + LIST_HEIGHT );
	current_y += LIST_HEIGHT;
}