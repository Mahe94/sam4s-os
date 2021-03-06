/**
 * \file
 *
 * \brief SD/MMC card example with FatFs
 *
 * Copyright (c) 2012 - 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage SD/MMC/SDIO Card with FatFs Example
 *
 * \section Purpose
 *
 * This example shows how to implement the SD/MMC stack with the FatFS.
 * It will mount the file system and write a file in the card.
 *
 * The example outputs the information through the standard output (stdio).
 * To know the output used on the board, look in the conf_example.h file
 * and connect a terminal to the correct port.
 *
 * While using Xplained Pro evaluation kits, please attach I/O1 Xplained Pro
 * extension board to EXT1.
 *
 * \section Usage
 *
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application.
 * Refert to conf_example.h file.
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- SD/MMC/SDIO Card Example on FatFs --
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     Please plug an SD, MMC or SDIO card in slot.
 *    \endcode
 */

#include <asf.h>
#include "conf_example.h"
#include <string.h>

#include "conf_board.h"
#include "conf_clock.h"
#include "resistive_touch_board.h"
#include "rtouch_calibrate.h"
#include "smc.h"

#define ILI93XX_LCD_CS      1

struct ili93xx_opt_t g_ili93xx_display_opt;

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

#define STRING_EOL    "\r"
#define STRING_HEADER "-- SPI_Touchscreen Example --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** Tick Counter united by ms */
static volatile uint32_t g_ul_tick_count = 0;

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */


/**
 * \brief Handler for SysTick interrupt. Increments the timestamp counter.
 */
void SysTick_Handler(void)
{
	g_ul_tick_count++;

	/* Call TSD_TimerHandler per 10ms */
	if ((g_ul_tick_count % 10) == 0) {
		rtouch_process();
	}
}

/**
 * \brief Callback called when the pen is pressed on the touchscreen.
 *
 * \param x horizontal position (in pixel if ts calibrated).
 * \param y vertical position (in pixel if ts calibrated).
 */
static void event_handler(rtouch_event_t const *event)
{
	switch(event->type) {
	case RTOUCH_MOVE:
		printf("Pen moved at     (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	case RTOUCH_PRESS:
		printf("Pen pressed at   (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	case RTOUCH_RELEASE:
		printf("Pen released at  (%03u, %03u)\n\r", (unsigned int) event->panel.x, (unsigned int) event->panel.y);
		break;
	default:
		break;
	}
}

/**
 *  \brief Configure the Console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main(void)
{
	

	Ctrl_status status;
//	FRESULT res;
	FATFS fs;

/**
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};
**/
	irq_initialize_vectors();
	cpu_irq_enable();

	sysclk_init();
	board_init();
	
	configure_console();

	/* Enable clocks for push buttons management */
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);

	/* Output example information */
	puts(STRING_HEADER);

	/* Configure systick for 1 ms. */
	puts("Configure system tick to get 1ms tick period.\r");
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("Systick configuration error\r");
		while (1);
	}

//	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);

	/* Initialize SD MMC stack */
	sd_mmc_init();
	
	pmc_enable_periph_clk(ID_SMC);

	/** Configure SMC interface for Lcd */
	smc_set_setup_timing(SMC, ILI93XX_LCD_CS, SMC_SETUP_NWE_SETUP(2)
	| SMC_SETUP_NCS_WR_SETUP(2)
	| SMC_SETUP_NRD_SETUP(2)
	| SMC_SETUP_NCS_RD_SETUP(2));
	smc_set_pulse_timing(SMC, ILI93XX_LCD_CS, SMC_PULSE_NWE_PULSE(4)
	| SMC_PULSE_NCS_WR_PULSE(4)
	| SMC_PULSE_NRD_PULSE(10)
	| SMC_PULSE_NCS_RD_PULSE(10));
	smc_set_cycle_timing(SMC, ILI93XX_LCD_CS, SMC_CYCLE_NWE_CYCLE(10)
	| SMC_CYCLE_NRD_CYCLE(22));
	#if ((!defined(SAM4S)) && (!defined(SAM4E)))
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
	| SMC_MODE_WRITE_MODE
	| SMC_MODE_DBW_8_BIT);
	#else
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
	| SMC_MODE_WRITE_MODE);
	#endif
	/** Initialize display parameter */
	g_ili93xx_display_opt.ul_width = ILI93XX_LCD_WIDTH;
	g_ili93xx_display_opt.ul_height = ILI93XX_LCD_HEIGHT;
	g_ili93xx_display_opt.foreground_color = COLOR_BLACK;
	g_ili93xx_display_opt.background_color = COLOR_WHITE;

	/** Switch off backlight */
	aat31xx_disable_backlight();

	/** Initialize LCD */
	ili93xx_init(&g_ili93xx_display_opt);

	/** Set backlight level */
	aat31xx_set_backlight(AAT31XX_AVG_BACKLIGHT_LEVEL);
	
	
	/** Turn on LCD */
	ili93xx_display_on();
	current_x = 0;
	current_y = 0;
	
	/* Initializes the PIO interrupt management for touchscreen driver */
	pio_handler_set_priority(PIOA, PIOA_IRQn, IRQ_PRIOR_PIO);
	pio_handler_set_priority(PIOB, PIOB_IRQn, IRQ_PRIOR_PIO);
	pio_handler_set_priority(PIOC, PIOC_IRQn, IRQ_PRIOR_PIO);

	/* Initialize touchscreen without calibration */
	rtouch_init(LCD_WIDTH, LCD_HEIGHT);
	rtouch_enable();
	rtouch_set_event_handler(event_handler);
	
	touch_init();
/**	
	while (1) {
		uc_result = rtouch_calibrate();
		if (uc_result == 0) {
			puts("Calibration successful !\r");
			break;
			} else {
			puts("Calibration failed; error delta is too big ! Please retry calibration procedure...\r");
		}
	}
**/

	status = sd_mmc_test_unit_ready(0);
	if (CTRL_FAIL == status) {
		add_list("PLEASE INSERT SD CARD", 0, 0);
		goto main_end_of_test;
	}
	
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE == res) {
		add_list("SD CARD CORRUPTED", 0, 0);
		goto main_end_of_test;
	}
	
	TCHAR path[100];
	memset(path,0,100);
	
	while (1) {
		main_screen:
		clear_screen();
		position = 0;
		add_list("MY-OS", 4, 0);
		add_list("FILE EXPLORER", 0, 1);
		add_list("GALLERY", 0, 2);
		add_list("MUSIC", 0, 3);
		add_list("PAINT", 0,4);	

		find_touch();
		switch(T.tfunction) {
			case 1:
					explorer:
					clear_screen();
					add_list("FILE EXPLORER", 0, 0);
					
					res = f_getcwd(path, 100);
					
					show_files(path, "");
					find_touch();
					
					if(T.tfunction == 10) {
						strcat(path, "/");
						strcat(path, T.tname);
						f_chdir(path);
						goto explorer;
					}
					if(!T.tfunction) 
						goto main_screen;
					break;
			case 2:
					gallery:
					clear_screen();
					add_list("GALLERY", 0, 0);
	
					res = f_getcwd(path, 100);
		
					show_files(path, "img");

					find_touch();
					if(T.tfunction == 10) {
						strcat(path, "/");
						strcat(path, T.tname);
						f_chdir(path);
						goto gallery;
					}
					else if(T.tfunction == 1) {
						TCHAR file_path[100];
						strcpy(file_path, path);
						strcat(file_path, "/");
						strcat(file_path, T.tname);
						show_image(file_path);
						find_touch();
						goto gallery;
					}
					if(!T.tfunction)
						goto main_screen;
					break;
					
				case 3:
						break;
						
				case 4:
						clear_screen();
						ili93xx_set_foreground_color(COLOR_BLACK);
						while(1) {
							get_touch();
							if(touch_X > 235 && touch_Y > 315)
								break;
							ili93xx_draw_filled_circle(touch_X, touch_Y, 2);
						}
						goto main_screen;
						break;
				default:
					break;
		}	

		while(1) {
			find_touch();}
	}
	
	main_end_of_test:
		while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
			find_touch();
		}
	
}
