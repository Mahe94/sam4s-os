/*
 * display.h
 *
 * Created: 24/01/2015 16:28:29
 *  Author: Mahe
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define LIST_HEIGHT 33
#define LIST_WIDTH 230
#define LIST_COLOR 0x808080
#define PADDING 10


uint32_t current_x;
uint32_t current_y;

void add_list(const uint8_t *p, uint8_t priority);

#endif /* DISPLAY_H_ */