/*
 * Uart.h
 *
 *  Created on: Apr 14, 2016
 *      Author: thangdd
 */

#ifndef DRIVERS_UART_SRC_UART_H_
#define DRIVERS_UART_SRC_UART_H_

#include <stdint.h>
#include <stddef.h>

/* For Bus, if bus is remove, this is removed too. */
#include "IesSdk.h"

#if defined(TMPR454)

#define COMM_FRAME_MAX 3
#define COMM_FRAME_BUF_SIZE 256

#else   /* defined(TMPR454) */

#define COMM_FRAME_MAX 6
#define COMM_FRAME_BUF_SIZE 256

#endif   /* defined(TMPR454) */

#if defined (TMPR454)
/* if porting for TMPR454, use memory map */
#include "TMPR454_memmap.h"
#else
#define VAR_SEC(sec)
#endif

#if defined (TMPV7528)

#pragma section data .ddr2_data_1 far
#pragma section area .ddr2_bss_1 far
#define VIS_MEM(sec) __section(sec)

#else

#define VIS_MEM(sec)

#endif

typedef struct {
    unsigned char buf[COMM_FRAME_BUF_SIZE];
    int index;
    int length;
    int bcc;
    int retry_count;
    int transmit_count;
    int status;
    int timeout;
    int timer_id;
} comm_uart_t;

typedef struct uart {
    int32_t (*initialize)(Bus bus);
    int32_t (*finalize)(Bus bus);
    int32_t (*receive)(Bus bus);
    int32_t (*transmit)(Bus bus, int c);
} uart_commmon_t;

/*
 * Definition error code
 */
enum {
    COM_NO_ERROR = 0,                 /**< Communication execute success */
    COM_INVALID_VALUE = -1,           /**< Communication invalid value */
    COM_DRIVER_ERROR = -2,            /**< Communication driver error */
    COM_NOT_INITIALIZED = -3          /**< Communication not initialized */
};

extern uart_commmon_t uart_common;

#ifdef __cplusplus
extern "C" {
#endif

extern int Uart_initialize(Bus bus);
extern int32_t Uart_finalize(Bus bus);

extern int32_t Uart_send_frame(Bus bus, const void *ptr, size_t len);
extern int32_t Uart_receive_frame(Bus bus, void *ptr, size_t maxsize);

/* TODO: move to private */
int32_t frame_receiving_control(Bus bus);
int32_t Uart_send(Bus bus, const void *ptr, size_t len, int32_t time_counter);
int32_t Uart_read(Bus bus, void *buf, size_t size, int32_t time_counter);
void Uart_initialize_transmistion(Bus bus);
void Uart_initialize_object(Bus bus, int timerID);
void Uart_set_retry(Bus bus, int32_t retry);
void Uart_set_timeout(Bus bus, int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_UART_SRC_UART_H_ */
