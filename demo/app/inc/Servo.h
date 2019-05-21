/**
 * @file Servo.h
 * Servo control API definitions.
 * 
 * Copyright (C) 2015 Toshiba Corporation. All rights reserved.
 */

#ifndef SERVO_H
#define SERVO_H

#include "IesSdk.h"

/* macro definitions */
#define TASK_MAX 4
#define MOTOR_MAX 2
#define COMM_MAX 3

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Position information of Servo.
 */
typedef struct {
    double x;                            /**< Store coordinate value in X direction [mm] */
    double y;                            /**< Store coordinate value in Y direction [mm] */
    double rad;                          /**< Store angle direction [radian] */
} Servo_Position_t;

/**
 * Define servo parameter information
 */
typedef struct {
   double move;
   double move_rad_acc;                     /**< Acceleration of angular velocity when move direct */
   double move_rad_max;                     /**< The maximum rotation angle speed of Valkyrie [rad/ms] */
   double move_rad_rot_acc;                 /**< Acceleration of angular velocity when rotate */
   double move_max;                         /**< The maximum movement speed of Valkyrie [mm/10ms] */
   double move_acc;                         /**< Move acceleration of Valkyrie [mm/10ms^2] */
   double distance_threshold;
   double rotation_threshold;
} Servo_Param_t;

/**
 * Define servo moving status 
 */
typedef enum {
    RUNNING_S = 1,
    STANDBY_S = 2,
    RPM_STATUS_S = 3,
    WRONG_MOVEMENT = 4,
    MAX_MOVEMENT_STATUS = 5
} Servo_MovingStatus_t;

/**
 * Speed information of motor.
 */
typedef struct {
    int right_rpm;                       /**< Speed of right motor in [mm/ms]  */
    int left_rpm;                        /**< Speed of left motor in [mm/ms] */
} Servo_MotorSpeed_t;

typedef struct {
    int target_rpm;
    int detection_rpm;
} Servo_ControlMotor_t;

typedef struct {
    float speed;
    float acc;
    Servo_ControlMotor_t motors[MOTOR_MAX];
} Servo_Control_t;

typedef struct {
    float utilization[TASK_MAX];
} Servo_PerfCpu_t;

typedef struct {
    int allocated;
    int total;
} Servo_PerfRam_t;

typedef struct {
    int transmitted;
    int received;
} Servo_PerfNetwork_t;

typedef struct {
    Servo_PerfCpu_t cpu;
    Servo_PerfRam_t ram;
    Servo_PerfNetwork_t network[COMM_MAX];
} Servo_Perf_t;

typedef struct {
    int failed;
    int times;
} Servo_DiagNetwork_t;

typedef struct {
    Servo_DiagNetwork_t network[COMM_MAX];
} Servo_Diag_t;

/**
 * Define servo information
 */
typedef struct {
    Servo_Control_t control;
    Servo_Perf_t performance;
    Servo_Diag_t diagnosis;
} Servo_Info_t;

typedef enum {
    NORMAL,
    LEARNING_MANUAL,
    LEARNING_AUTO
} Servo_Mode_t;

/* Define APIs*/
extern IesSdk_ReturnValue_t Servo_initialize(int coordinate_system);
extern IesSdk_ReturnValue_t Servo_move_direct(double dx, double dy, double velocity);
extern IesSdk_ReturnValue_t Servo_move_direct_absolute(double x, double y, double velocity);
extern IesSdk_ReturnValue_t Servo_rotate(double rad, double velocity);
extern IesSdk_ReturnValue_t Servo_set_rpm(short right, short left);
extern IesSdk_ReturnValue_t Servo_get_odometer(Servo_Position_t *p_coordinate);
extern IesSdk_ReturnValue_t Servo_get_param(Servo_Param_t *p_param);
extern IesSdk_ReturnValue_t Servo_get_info(Servo_Info_t *p_info);
extern IesSdk_ReturnValue_t Servo_set_mode(Servo_Mode_t mode);
extern IesSdk_ReturnValue_t Servo_check_movement(double dx, double dy);
extern IesSdk_ReturnValue_t Servo_active_update_task(void);

#ifdef __cplusplus
}
#endif

#endif  /* SERVO_H */

/*
 * Local Variables:
 * coding:utf-8-dos
 * indent-tabs-mode:nil
 * mode:c
 * c-set-style:k&r
 * c-basic-offset:4
 * End:
 *
 * vim: set fenc=utf-8 ff=dos sw=4 ts=4 sts=0 et cin cino=(0,\:0:
 */
