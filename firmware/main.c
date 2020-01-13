/*
 * Copyright (c) 2017 Bernd Walter
 * All rights reserved.
 *
 * $URL$
 * $Date$
 * $Author$
 * $Rev$
 */

#include "asf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "conf_usb.h"
#include "gcode.h"

void
_init()
{
}

#define Main_TASK_PRIORITY	(tskIDLE_PRIORITY + 2)

void dataloop();

static void Main_Task(void *pvParameters)
{
	(void)pvParameters;

	sio_init();
	udc_start();

	dataloop();
}

void
dataloop()
{
	sio_putstr("\r\nBWCT PnP-pulsefeeder\r\n");

#define GCODE_BUFLEN 128
	static char data[GCODE_BUFLEN];
	uint8_t wpos = 0;
	char tmp;
	bool comment = false;

	for (;;) {
		tmp = sio_getchar();
		if (tmp == '\r' || tmp == '\n') {
			comment = false;
			data[wpos] = '\0';
			if (wpos != 0) {
				parsegcode(data);
				sio_putstr("ok\r\n");
			}
			wpos = 0;
		} else {
			if (tmp == ';') {
				comment = true;
			}
			if (!comment) {
				data[wpos] = tmp;
				wpos++;
				if (wpos >= GCODE_BUFLEN - 1) {
					sio_putstr("line too long");
					wpos = 0;
				}
			}
		}
	}

	for (;;) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void
ioinit()
{
	PORT->Group[0].OUTSET.reg =
	    PORT_PA03 |
	    PORT_PA00 |
	    PORT_PA10 |
	    PORT_PA04 |
	    PORT_PA07 |
	    PORT_PA08 |
	    PORT_PA13 |
	    PORT_PA16;

	PORT->Group[0].DIRSET.reg =
	    PORT_PA03 |
	    PORT_PA00 |
	    PORT_PA10 |
	    PORT_PA04 |
	    PORT_PA07 |
	    PORT_PA08 |
	    PORT_PA13 |
	    PORT_PA16;

	PORT->Group[1].OUTSET.reg =
	    PORT_PB07 |
	    PORT_PB08 |
	    PORT_PB30 |
	    PORT_PB05;

	PORT->Group[1].DIRSET.reg =
	    PORT_PB07 |
	    PORT_PB08 |
	    PORT_PB30 |
	    PORT_PB05;
}

void
setpin(int pin, bool val)
{
	switch(pin) {
	case 0:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA03;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA03;
		}
		break;
	case 1:
		if (val) {
			PORT->Group[1].OUTCLR.reg = PORT_PB30;
		} else {
			PORT->Group[1].OUTSET.reg = PORT_PB30;
		}
		break;
	case 2:
		if (val) {
			PORT->Group[1].OUTCLR.reg = PORT_PB05;
		} else {
			PORT->Group[1].OUTSET.reg = PORT_PB05;
		}
		break;
	case 3:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA00;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA00;
		}
		break;
	case 4:
		if (val) {
			PORT->Group[1].OUTCLR.reg = PORT_PB07;
		} else {
			PORT->Group[1].OUTSET.reg = PORT_PB07;
		}
		break;
	case 5:
		if (val) {
			PORT->Group[1].OUTCLR.reg = PORT_PB08;
		} else {
			PORT->Group[1].OUTSET.reg = PORT_PB08;
		}
		break;
	case 6:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA10;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA10;
		}
		break;
	case 7:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA04;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA04;
		}
		break;
	case 8:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA07;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA07;
		}
		break;
	case 9:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA08;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA08;
		}
		break;
	case 10:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA13;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA13;
		}
		break;
	case 11:
		if (val) {
			PORT->Group[0].OUTCLR.reg = PORT_PA16;
		} else {
			PORT->Group[0].OUTSET.reg = PORT_PA16;
		}
		break;
	}
}

int
main(void)
{
	system_init();

	ioinit();

	xTaskCreate(Main_Task, "Main", 512, NULL, Main_TASK_PRIORITY, NULL);

	vTaskStartScheduler();

	for (;;) {
	}

	return 0;
}

static volatile uint32_t off_timer[12];

void vApplicationTickHook( void )
{
	for (int i = 0; i < 12; i++) {
		if (off_timer[i] != 0) {
			if (--off_timer[i] == 0) {
				setpin(i, 0);
			}
		}
	}
}

void
set_off_timer(int pin, uint32_t ms)
{
	if (pin >= 0 && pin <= 11) {
		off_timer[pin] = ms / portTICK_PERIOD_MS;
	}
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	(void) pcTaskName;
	(void) pxTask;

	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

void vApplicationMallocFailedHook( void )
{
	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

bool main_cdc_enable(uint8_t port)
{
	//main_b_cdc_enable = true;
	// Open communication
	//uart_open(port);
	return true;
}

void main_cdc_disable(uint8_t port)
{
	//main_b_cdc_enable = false;
	// Close communication
	//uart_close(port);
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		//ui_com_open(port);
	} else {
		// Host terminal has close COM
		//ui_com_close(port);
	}
}

void main_suspend_action(void)
{
	//ui_powerdown();
}

void main_resume_action(void)
{
	//ui_wakeup();
}

void main_sof_action(void)
{
	//if (!main_b_cdc_enable) {
	//	return;
	//}
	//ui_process(udd_get_frame_number());
}


