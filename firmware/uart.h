/*
 * Copyright (c) 2017 Bernd Walter
 * All rights reserved.
 *
 * $URL$
 * $Date$
 * $Author$
 * $Rev$
 */

#ifndef _UART_H_
#define _UART_H_

void uart_rx_notify(uint8_t port);
void uart_tx_notify(uint8_t port);
void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg);
void uart_open(uint8_t port);
void uart_close(uint8_t port);

void sio_init();
void sio_putchar(uint8_t c);
void sio_putstr(const char* data);
char sio_getchar();

#endif // _UART_H_
