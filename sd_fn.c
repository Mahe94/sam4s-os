/*
 * sd_fn.c
 *
 * Created: 24/01/2015 18:44:19
 *  Author: Mahe
 */ 

#include <asf.h>
#include "sd_fn.h"
#include <string.h>

void show_files(const TCHAR *path, const TCHAR *ext) {
	DIR dj;
	memset(&dj,0,sizeof(dj));
	
	res = f_opendir(&dj, path);
	
	FILINFO f;
	TCHAR *fname;
	memset(&f,0,sizeof(f));
	
	uint8_t position = 0, attrib = 1;
	
	while((res = f_readdir(&dj, &f)) == FR_OK) {
		if(f.fattrib==AM_DIR)
			attrib = 10;
		if(attrib != 10) {
			fname = f.fname;
			while(*fname != '.') ++fname;
			++fname;
			if(strcmp(fname, ext) && strcmp(ext, ""))
				continue;
		}		
		if(!add_list(f.fname, 1, attrib))
			break;
//		position = (current_y-LIST_HEIGHT)/LIST_HEIGHT;
//		strcpy(tfunction[position], f.fname);
		memset(&f,0,sizeof(f));
		attrib = 1;
//		++position;
	}
	
//	if(res == FR_OK)
		next_arrow();
}