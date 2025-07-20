/*
 * events.c
 *
 *  Created on: Jul 5, 2025
 *      Author: fallal
 */
#include "events.h"
#include <stddef.h>
evCallback onMount = NULL;
evCallback onUnMount = NULL;
void setOnMount(evCallback ev)
{
	onMount = ev;
}
void callOnMount()
{
	if(onMount) onMount();
}
void setOnUnMount(evCallback ev)
{
	onUnMount = ev;
}
void callOnUnMount()
{
	if(onUnMount) onUnMount();
}
