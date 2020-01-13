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
#include "udi_cdc.h"
#include "semphr.h"

static SemaphoreHandle_t tx_sem = NULL;
static SemaphoreHandle_t rx_sem = NULL;

void uart_config(uint8_t port,usb_cdc_line_coding_t *cfg)
{
}

void uart_rx_notify(uint8_t port)
{
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(rx_sem, &xHigherPriorityTaskWoken);
}

void uart_tx_notify(uint8_t port)
{
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(tx_sem, &xHigherPriorityTaskWoken);
}

void
sio_putchar(uint8_t c)
{
	while (!udi_cdc_is_tx_ready()) {
		xSemaphoreTake(tx_sem, 0xffff);
	}
	udi_cdc_putc(c);
}

void
sio_putstr(const char* data)
{
	while(*data != '\0') {
		sio_putchar(*data);
		data++;
	}
}

char
sio_getchar()
{
	uint8_t data;

	while (!udi_cdc_is_rx_ready()) {
		xSemaphoreTake(rx_sem, 0xffff);
	}
	data = udi_cdc_getc();
		
	return data;
}

void
sio_init()
{
	tx_sem = xSemaphoreCreateBinary();
	rx_sem = xSemaphoreCreateBinary();
}

