/**
 * @file   IesSdk.h
 * @brief  Structure definitions for IES
 *
 * Copyright (C) 2015 Toshiba Corporation. All right reserved.
 */
#ifndef IESSDK_H
#define IESSDK_H

typedef enum {
    WRONG = -6,
    CONNECTION_FAILURE =-5,
    ACK_FAILURE = -4,
    EXEC_FAILURE = -3,
    COMM_FAILURE = -2,
    INVALID_PARAM = -1,
    SUCCESS = 0,
    RUNNING = 1,
    STANDBY = 2,
    RPM_STATUS = 3
} IesSdk_ReturnValue_t;

/*
 * Definition communication port.
 */
typedef enum {
    COM_BUS_INITIAL_VALUE=-1,         /**< Initialized value communication */
    COM_BUS_UART_0	= 0,              /**< Communication UART port 0 */
    COM_BUS_UART_1,                   /**< Communication UART port 1 */
    COM_BUS_UART_2,                   /**< Communication UART port 2 */
    COM_BUS_UART_3,                   /**< Communication UART port 3 */
    COM_BUS_UART_4,                   /**< Communication UART port 4 */
    COM_BUS_CAN                       /**< Communication CAN port */
} Bus;

#endif

