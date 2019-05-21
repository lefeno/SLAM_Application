/**
 * @file   Image.h
 * @brief  Image Recognition API definitions
 *
 * Copyright (C) 2015 Toshiba Corporation. All right reserved.
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>

#include "IesSdk.h"

/**
 * @struct detect_info_t
 * @brief The detection information of an object (target or obstacle) on image.
 */
typedef struct {
    uint16_t r;  /**< The reserved parameter. Do not refer this value. */
    int16_t  x;  /**< X-coordinate of the center point of the detection area [pixel] */
    int16_t  y;  /**< Y-coordinate of the center point of the detection area [pixel] */
    uint16_t w;  /**< The width of the detection area in image [pixel] */
    uint16_t h;  /**< The height of the detection area in image [pixel] */
    uint16_t dummey; /**< For adjustment of a reserved area / MPE side */
} detect_info_t;

/**
 * @struct location_info_t
 * @brief The location of target or obstacle in real space.
 */
typedef struct {
    int16_t x;   /**< Store coordinate value in X direction [mm] */
    int16_t y;   /**< Store coordinate value in Y direction [mm] */
    int16_t z;   /**< Store coordinate value in Z direction [mm]
                      Note: currently, coordinate value in Z direction is always 0. */
} location_info_t;

/**
 * @struct hand_info_t
 * @brief This data structure store the result of hand gesture recognition.
 */
typedef struct {
    int8_t result;  /**< The result of hand detection
                          0: No detect
                          1: The hand detected
                     */
    uint8_t num;    /**< The number of fingers of hand gesture */
} hand_info_t;

/**
 * @struct face_info_t
 * @brief This data structure store the result of Human face detection.
 */
typedef struct {
    uint8_t result;  /**< face detection result */
    uint8_t num; 	 /**< number of faces */
} face_info_t;

/* Define the new struct for get coordinate APIs */
typedef struct {
    int8_t result;				/* The detection result */
    location_info_t target;		/* The real space coordinate of target */
    location_info_t obstacle;	/* The real space coordinate of obstacle */
} coordinate_t;

typedef struct {
    int8_t result;				/* The detection result */
    detect_info_t target;		/* The detection information of target */
    detect_info_t obstacle;		/* The detection information of obstacle */
} detection_info_t;

/* Recognition parameter type of Image_set_recognition_param API */
typedef struct {
    int32_t  color_type;
    int16_t   h_th[2];
    int16_t   s_th[2];
    int16_t   d_th[2];
    int32_t  tar_w;
    int32_t  tar_h;
    int32_t  obs_w;
    int32_t  obs_h;
    int32_t   d_offset_y;
} recognition_param_t;

/* Hand gesture parameter type of Image_set_hand_gesture_param API */
typedef struct {
    int16_t hand_th;
    int16_t h_th[2];
    int16_t s_th[2];
} hand_gesture_param_t;

/**
 * @struct face_detection_param_t
 * @brief Storage parameters to detect Human face
 */
typedef struct {
    int16_t face_th; /**< Threshold for Area criteria in verify step */
    int16_t h_th[2]; /**< Threshold for Hue in segment step */
    int16_t s_th[2]; /**< Threshold for Saturation in segment step */
} face_detection_param_t;

/* Output image parameter type of Image_select_output_image API */
typedef struct {
    uint8_t first_src;
    uint8_t second_src;
    uint8_t third_src;
    uint8_t fourth_src;
} select_output_image_t;

/* Structure for setting Recognition parameters */
typedef struct {
    uint8_t id;
    recognition_param_t recog_param;
} set_recog_param_t;

/* Structure for setting Hand gesture */
typedef struct {
    uint8_t id;
    hand_gesture_param_t hand_ges_param;
} set_hand_gesture_param_t;

/**
 * @struct set_face_detection_param_t
 * @brief This structure is used by Image_set_face_detection_param API
 */
typedef struct {
    uint8_t id; /**< Command ID of Image_set_face_detection_param API */
    face_detection_param_t face_detect_param; /**< parameters to detect Human face */
} set_face_detection_param_t;

/* Structure for setting Image Output */
typedef struct {
    uint8_t id;
    select_output_image_t output_param;
} set_select_output_param_t;

/* List of supported output image ID */
typedef enum {
    NO_IMG        = 0x00,
    LEFT_IMG      = 0x01,
    RIGHT_IMG     = 0x02,
    H_IMG         = 0x03,
    S_IMG         = 0x04,
    H_S_IMG       = 0x05,
    DISPARITY_IMG = 0x06,
    DEPTH_IMG     = 0x07,
    DETECT_BINARY_IMG = 0x08,
    MATCHING_IMG  = 0x09,
    LABELING_IMG  = 0x0A,
    EDGE_IMG      = 0x0B
} Image_ID;

/* Detection information of the target and the obstacle */
typedef enum {
    DETECTED_NONE       = 0x00,
    DETECTED_TARGET     = 0x01,
    DETECTED_OBSTACLE   = 0x02,
    DETECTED_BOTH       = 0x03,
    EXECUTION_FAILURE   = 0x04,
    COMMUNICATION_FAILURE = 0x05
} ImageResult;

/* Define some API of Image Recognition SDK */
IesSdk_ReturnValue_t Image_initialize(void);
IesSdk_ReturnValue_t Image_terminate(void);
IesSdk_ReturnValue_t Image_set_recognition_param(const recognition_param_t* p_setting);
IesSdk_ReturnValue_t Image_set_hand_gesture_param(const hand_gesture_param_t* p_setting);
IesSdk_ReturnValue_t Image_set_face_detection_param(const face_detection_param_t* p_setting);
IesSdk_ReturnValue_t Image_select_output_image(const select_output_image_t* p_setting);
IesSdk_ReturnValue_t Image_get_coordinate(coordinate_t *p_value);
IesSdk_ReturnValue_t Image_get_detection_info(detection_info_t* p_value);
IesSdk_ReturnValue_t Image_get_hand_gesture(hand_info_t* p_value);
IesSdk_ReturnValue_t Image_get_face_detection_result(face_info_t* p_value);

#endif /* IMAGE_H */
