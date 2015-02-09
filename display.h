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

void clear_screen(void);
void next_arrow(void);
uint8_t check_space(void);
uint8_t add_list(char *p, uint8_t priority, uint8_t fn);

#endif /* DISPLAY_H_ */