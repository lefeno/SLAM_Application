/*
 * Port.h
 *
 *  Created on: Apr 14, 2016
 *      Author: thangdd
 */

#ifndef DRIVERS_UART_INCLUDE_PORT_H_
#define DRIVERS_UART_INCLUDE_PORT_H_

#include <stdint.h>
#include <stddef.h>

#include "IesSdk.h"

typedef enum {
    COMM_ERROR_DETECT_OFF = 0,
    COMM_ERROR_DETECT_ON = 1 << 0,
    COMM_ERROR_DETECT_RECV_BREAK = 1 << 1,
    COMM_ERROR_DETECT_SEND_BREAK = 1 << 2,
    COMM_ERROR_DETECT_RECV     = 1 << 3,
    COMM_ERROR_DETECT_SEND     = 1 << 4 
} comm_error_detect_e;

typedef struct {
    int error_detection; /* 0: Off, 1: reset, 2: continue. */
} comm_port_t;

#ifdef __cplusplus
extern "C" {
#endif
/* Local Port functions */
extern int32_t Port_initialize(Bus bus);
extern int32_t Port_finalize(Bus bus);
extern int32_t Port_read(Bus bus, void *ptr, size_t maxsize);
extern int32_t Port_write(Bus bus, const void *ptr, size_t maxsize);

/* TODO: move to private */
void Port_set_error_detection(Bus bus, int error_detection);
int32_t Port_error_detection_is_enabled(Bus bus);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_UART_INCLUDE_PORT_H_ */
