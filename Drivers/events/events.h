/*
 * events.h
 *
 *  Created on: Jul 5, 2025
 *      Author: fallal
 */

#ifndef EVENTS_EVENTS_H_
#define EVENTS_EVENTS_H_

typedef void (*evCallback)(void);

void setOnMount(evCallback);
void callOnMount();
void setOnUnMount(evCallback);
void callOnUnMount();
#endif /* EVENTS_EVENTS_H_ */
