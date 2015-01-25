/*
 * sd_fn.c
 *
 * Created: 24/01/2015 18:44:19
 *  Author: Mahe
 */ 

#include <asf.h>
#include "sd_fn.h"

void show_files(const TCHAR *path) {
	DIR dj;
	memset(&dj,0,sizeof(dj));
	
	res = f_opendir(&dj, path);
	
	FILINFO f;
	memset(&f,0,sizeof(f));
	
	uint8_t position = 0;
	
	while((res = f_readdir(&dj, &f)) == FR_OK) {	
		if(!add_list((const uint8_t *)f.fname, 1))
			break;
		position = (current_y-LIST_HEIGHT)/LIST_HEIGHT;
		strcpy(tfunction[position], f.fname);
		memset(&f,0,sizeof(f));
		++position;
	}
	
//	if(res == FR_OK)
		next_arrow();
}