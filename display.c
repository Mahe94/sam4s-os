/*
 * display.c
 *
 * Created: 24/01/2015 16:29:14
 *  Author: Mahe
 */ 

#include "stdint-gcc.h"
#include "display.h"
#include <ili93xx.h>

void clear_screen(void) {
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(0, 0, ILI93XX_LCD_WIDTH, ILI93XX_LCD_HEIGHT);
}

uint8_t check_space(void) {
	if(current_y > ILI93XX_LCD_HEIGHT-LIST_HEIGHT)
		return 0;
	else
		return 1;	
}

void next_arrow() {
	ili93xx_set_foreground_color(LIST_COLOR);
	ili93xx_draw_string(ILI93XX_LCD_WIDTH - (4*PADDING), current_y + PADDING, (const uint8_t *)">>>");
}

uint8_t add_list(const uint8_t *p, uint8_t priority) {
	if(!check_space())
		return 0;
	ili93xx_set_foreground_color(LIST_COLOR);
	ili93xx_draw_string(current_x + (1 +priority)*PADDING, current_y + PADDING, p);
	ili93xx_draw_line(current_x + PADDING, current_y + LIST_HEIGHT, current_x + LIST_WIDTH , current_y + LIST_HEIGHT );
	current_y += LIST_HEIGHT;
	return 1;
}
