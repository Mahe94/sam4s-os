/*
 * touch.h
 *
 * Created: 24/01/2015 19:58:35
 *  Author: Mahe
 */ 


#ifndef TOUCH_H_
#define TOUCH_H_

#define  MULTI_FACTOR      1024

struct touch {
	char tname[100];
	uint8_t tfunction;
}T;

char tname[10][100];
uint8_t tfunction[10];
uint8_t position;
uint32_t touch_X, touch_Y;

void touch_init(void);
void find_touch(void);
void get_touch(void);


#endif /* TOUCH_H_ */