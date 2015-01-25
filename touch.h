/*
 * touch.h
 *
 * Created: 24/01/2015 19:58:35
 *  Author: Mahe
 */ 


#ifndef TOUCH_H_
#define TOUCH_H_

#define  MULTI_FACTOR      1024

char tfunction[10][100];

void touch_init(void);
TCHAR* find_touch(void);


#endif /* TOUCH_H_ */