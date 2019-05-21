/**
 * @file   ImageComm.h
 *
 * @brief  ImageComm cleass - common header
 *
 * COPYRIGHT (C) 2013 TOSHIBA INFOMATION SYSTEMS ALL RIGHTS RESERVED
 *
 * @note   This header files refer to "req_image_module.txt"
 *
 */

#ifndef IMAGE_COMM_H
#define IMAGE_COMM_H

#include <stdint.h>

/**
 * @brief   型定義
 */

typedef struct {
	uint16_t r;  /* 検出結果の番号               */
	int16_t  x;  /* 検出領域の中心点のx座標      */
	int16_t  y;  /* 検出領域の中心点のy座標      */
	uint16_t w;  /* 検出領域の幅                 */
	uint16_t h;  /* 検出領域の高さ               */
	uint16_t dummey; /* 予約領域/MPE側との調整用 */
} detect_info_t;


typedef struct {
	int16_t x;   /* X座標 [mm]（正面方向) */
	int16_t y;   /* Y座標 [mm] (右方向)   */
	int16_t z;   /* Z座標 [mm]（高さ）    */
} location_info_t;


typedef struct {
	detect_info_t    detect;   /* 検出結果   */
	location_info_t  location; /* 実空間座標 */
} recog_info_t;


typedef enum {
	TARGET_ID = 0,
	OBSTACLE_ID,
	NUM_RECOG_IDS
} Recog_ID;


typedef enum {
	DETECTED_NONE		= 0x00,
	DETECTED_TARGET	= 0x01,
	DETECTED_OBSTACLE	= 0x02,
	DETECTED_BOTH
} ImageResult;
#define NON_PRECISION_BIT		0x80
#define PARTIAL_DETECTED_BIT	0x40

/**
 * @brief   プロトタイプ宣言
 */
bool ImageComm_initialize(void);
bool ImageComm_sync(void);
ImageResult ImageComm_get_result(recog_info_t *recog_info);
bool ImageComm_terminate(void);


#endif /* IMAGE_COMM_H */
