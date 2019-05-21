/**
 * @file   Network.h
 * @brief  Network API definitions
 *
 * Copyright (C) 2015 Toshiba Corporation. All right reserved.
 */
#ifndef NETWORK_H
#define NETWORK_H

#include "IesSdk.h"


typedef void (*mqtt_call_back_t) (void*,int);

/* Define the new struct for MQTT receive API*/
typedef struct{
    const char *topic_id;
    int (*func) (void*,int);
}Topic_callback_t;

/* Define some API for Network SDK*/
extern IesSdk_ReturnValue_t Network_MQTT_init(const char *addr, int valkyrie_id);
extern IesSdk_ReturnValue_t Network_MQTT_send(const char *topic_id, void *p_payload, int size);
extern IesSdk_ReturnValue_t Network_MQTT_set_receive_func(const char *topic_id, mqtt_call_back_t func);


#endif /* NETWORK_H */
