/*
 * File_Handling_RTOS.h
 *
 *  Created on: 30-April-2020
 *      Author: Controllerstech
 */

#ifndef FILE_HANDLING_H_
#define FILE_HANDLING_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "states.h"

/* mounts the USB*/
FRESULT Mount_USB (void);

/* unmounts the USB*/
FRESULT Unmount_USB (void);

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_USB (char* pat);

/* Only supports removing files from home directory. Directory remover to be added soon */
FRESULT Format_USB (void);

/* write the data to the file
 * @ name : is the path to the file*/
FRESULT Write_File (char *name, char *data);

/* read data from the file
 * @ name : is the path to the file*/
FRESULT Read_File (char *name);

/* creates the file, if it does not exists
 * @ name : is the path to the file*/
FRESULT Create_File (char *name);

/* Removes the file from the USB
 * @ name : is the path to the file*/
FRESULT Remove_File (char *name);

/* creates a directory
 * @ name: is the path to the directory
 */
FRESULT Create_Dir (char *name);

/* checks the free space in the USB*/
void Check_USB_Details (void);

/* updates the file. write pointer is set to the end of the file
 * @ name : is the path to the file
 */
FRESULT Update_File (char *name, char *data);


void Scan_USB_Dir(sys_context_t* sys_ctx);

bool USB_IsReady(void);
bool USB_IsMounted(void);
void USB_mark_Ready(bool ready);
void USB_mark_Mounted(bool mounted);
#endif /* FILE_HANDLING_RTOS_H_ */
