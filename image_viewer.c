/*
 * image_viewer.c
 *
 * Created: 25/01/2015 15:04:22
 *  Author: Mahe
 */ 

#include <asf.h>
#include "display.h"

void show_image(const TCHAR *path) {
//				char test_file_name[] = "0:sd_mmc_test.txt";
				FIL file_object;
//				test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
				res = f_open(&file_object,
				(char const *)path, FA_READ);
				//				FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK) {
					//			printf("[FAIL] res %d\r\n", res);
					return ;
				}
				//		printf("[OK]\r\n");
				
				char pixel[9], *ptr;
				uint32_t hex;
				
				ili93xx_display_off();
				clear_screen();
				gpio_set_pin_low(LED0_GPIO);
				for(int y=2; y<=320+1; ++y) {
					for(int x=2; x<=240+1; ++x) {
						if(0 == f_gets(pixel, 9, &file_object))
						break;
						hex = strtoul(pixel, &ptr, 16);
						ili93xx_set_foreground_color(hex);
						ili93xx_draw_pixel(x, y);
					}
				}
				gpio_set_pin_high(LED0_GPIO);
				ili93xx_display_on();
				
}