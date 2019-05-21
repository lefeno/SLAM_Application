/*
 * utils.h
 *
 *  Created on: Aug 18, 2016
 *      Author: trucndt
 */

#ifndef UTILS_H
#define UTILS_H
/* To avoid this error:
 *      undefined reference to `boost::system::generic_category()'
 */
#define BOOST_SYSTEM_NO_DEPRECATED

#include <stdint.h>
#include <iostream>
#include <stdlib.h>

int makeRealTimeThread();
void dieWithError(const char *anErrMess);

#endif
