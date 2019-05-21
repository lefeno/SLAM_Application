/**
 * @file   Frontwiew.h
 * @brief  Frontwiew class - common header
 *
 * Copyright (C) 2015 Toshiba Corporation. All right reserved.
 */

#ifndef FRONTWIEW_H
#define FRONTWIEW_H

#include <stdint.h>
#include <stdbool.h>
#include "Image.h"
#define OBSTACLE_NUM 1

/**
 * @brief   Type definition
 */

typedef struct {
    detect_info_t    detect;   /* Detection result */
    location_info_t  location; /* Real space coordinates */
} recog_info_t;

typedef struct {
    int16_t distance; /* Proximity sensor */
    recog_info_t target;
    recog_info_t obstacle[OBSTACLE_NUM];
} front_info_t;

/* Recognition ID of object */
typedef enum {
    TARGET_ID = 0,
    OBSTACLE_ID,
    NUM_RECOG_IDS
} Recog_ID;

#define NON_PRECISION_BIT       0x80U
#define PARTIAL_DETECTED_BIT    0x40U

#endif /* FRONTVIEW_H */
