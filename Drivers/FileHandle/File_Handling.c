/*
 * File_Handling_RTOS.c
 *
 *  Created on: 26-June-2020
 *      Author: Controllerstech.com
 */

#include "File_Handling.h"
#include "stm32f4xx_hal.h"
#include "screens.h"



/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */



extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;

static bool is_USB_Mounted = false;
static bool is_USB_Ready = false;
void USB_mark_Ready(bool ready)
{
	is_USB_Ready = ready;
	if (ready) {
		printf("USB is ready for use.\r\n");
	} else {
		printf("USB is not ready.\r\n");
	}
}
void USB_mark_Mounted(bool mounted)
{
	is_USB_Mounted = mounted;
	if (mounted) {
		printf("USB is mounted.\r\n");
	} else {
		printf("USB is unmounted.\r\n");
	}
}
bool USB_IsReady(void)
{
	return is_USB_Ready;
}
bool USB_IsMounted(void)
{
	return is_USB_Mounted;
}
FRESULT Mount_USB (void)
{
	FRESULT res = f_mount(&USBHFatFS, USBHPath, 1);
	if ( res == FR_OK)
	{
		
		printf("USB Mounted Successfully\r\n");
		USB_mark_Mounted(true);
	}
	else {
		printf("USB Mount Failed with error code: %d\r\n", res);
		USB_mark_Mounted(false);
	}
	return res;
}

FRESULT Unmount_USB (void)
{
	FRESULT res = f_mount(NULL, USBHPath, 1);
	if (res == FR_OK)
	{
		printf("USB Unmounted Successfully\r\n");
		USB_mark_Mounted(false);
	}
	else {
		printf("USB Unmount Failed with error code: %d\r\n", res);
	}
	return res;
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_USB (char* pat)
{
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	printf("Dir: %s\r\n", USBHfno.fname);
                i = strlen(path);
                sprintf(&path[i], "/%s", USBHfno.fname);
                fresult = Scan_USB(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
            	printf("File: %s/%s\r\n", path, USBHfno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_USB (void)
{
    DIR dir;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
            	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
            	fresult = f_unlink(USBHfno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(USBHfno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}




FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		printf( "ERROR!!! *%s* does not exists\n\n", name);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	printf("ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	        return fresult;
	    }

	    else
	    {
	        printf("Opening file-->  *%s*  To WRITE data in it\n", name);
	    	fresult = f_write(&USBHFile, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		printf("ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
	    	}

	    	/* Close file */
	    	fresult = f_close(&USBHFile);
	    	if (fresult != FR_OK)
	    	{
	    		printf("ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
	    	}
	    	else
	    	{
	    		printf("File *%s* is WRITTEN and CLOSED successfully\n\n", name);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	{
		printf( "ERRROR!!! *%s* does not exists\n\n", name);
	    return fresult;
	}

	else
	{
		/* Open file to read */
		fresult = f_open(&USBHFile, name, FA_READ);

		if (fresult != FR_OK)
		{
			printf( "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
		    return fresult;
		}

		/* Read data from the file
		* see the function details for the arguments */
		printf( "Opening file-->  *%s*  To READ data from it\n", name);

		char *buffer = malloc(sizeof(f_size(&USBHFile)));
		fresult = f_read (&USBHFile, buffer, f_size(&USBHFile), &br);
		if (fresult != FR_OK)
		{
			printf( "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
		}
		else
		{
			printf(buffer);
			free(buffer);

			/* Close file */
			fresult = f_close(&USBHFile);
			if (fresult != FR_OK)
			{
				printf("ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
			}
			else
			{
				printf("File *%s* CLOSED successfully\n\n", name);
			}
		}
	    return fresult;
	}
}

FRESULT Create_File (char *name)
{
	fresult = f_stat(name, &USBHfno);
	if (fresult == FR_OK)
	{
		printf("ERROR!!! *%s* already exists!!!!\nUse Update_File\n\n", name);
		return fresult;
	}

	fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	if (fresult != FR_OK)
	{
		printf("ERROR!!! No. %d in creating file *%s*\n\n", fresult, name);
		return fresult;
	}

	printf("*%s* created successfully\nNow use Write_File to write data\n", name);

	fresult = f_close(&USBHFile);
	if (fresult != FR_OK)
	{
		printf("ERROR No. %d in closing file *%s*\n\n", fresult, name);
	}
	else
	{
		printf("File *%s* CLOSED successfully\n\n", name);
	}

	return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	fresult = f_stat(name, &USBHfno);
	if (fresult != FR_OK)
	{
		return fresult;
	}

	fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
	if (fresult != FR_OK)
	{
		return fresult;
	}

	fresult = f_write(&USBHFile, data, strlen(data), &bw);

	fresult = f_close(&USBHFile);

	return fresult;
}

FRESULT Remove_File (char *name)
{
	fresult = f_stat(name, &USBHfno);
	if (fresult != FR_OK)
	{
		printf("ERROR!!! *%s* does not exist\n\n", name);
		return fresult;
	}

	fresult = f_unlink(name);
	if (fresult == FR_OK)
	{
		printf("*%s* has been removed successfully\n\n", name);
	}
	else
	{
		printf("ERROR No. %d in removing *%s*\n\n", fresult, name);
	}

	return fresult;
}

FRESULT Create_Dir (char *name)
{
	fresult = f_mkdir(name);
	if (fresult == FR_OK)
	{
		printf("*%s* has been created successfully\n\n", name);
	}
	else
	{
		printf("ERROR No. %d in creating directory *%s*\n\n", fresult, name);
	}
	return fresult;
}

void Check_USB_Details (void)
{
	f_getfree("", &fre_clust, &pUSBHFatFS);
	total = (uint32_t)((pUSBHFatFS->n_fatent - 2) * pUSBHFatFS->csize * 0.5);
	printf("USB Total Size: \t%lu\n", total);
	free_space = (uint32_t)(fre_clust * pUSBHFatFS->csize * 0.5);
	printf("USB Free Space: \t%lu\n", free_space);
}
// scan the path given only the path and generate the directory structure
// it should count all items in the directory and store them in current directory
// putting directories first and then files
// should handle the previous directory call 
// log using printf
void Scan_USB_Dir(sys_context_t* sys_ctx)
{
	directory_t * current_directory = &sys_ctx->current_directory;
	DIR dir;
	FILINFO fno;
	FRESULT res;
	// Initialize current directory
	current_directory->item_count = 0;
	// Open the directory
	res = f_opendir(&dir, current_directory->path);
	assert_param(res == FR_OK); // Ensure the directory opened successfully

	// Read directory items
	while (1) {
		res = f_readdir(&dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0) break; // End of directory or error

		// Only add directories or sound files
		bool is_dir = (fno.fattrib & AM_DIR) ? true : false;
		bool is_sound = false;
		if (!is_dir) {
			const char *ext = strrchr(fno.fname, '.');
			if (ext) {
				if (strcasecmp(ext, ".wav") == 0 || strcasecmp(ext, ".mp3") == 0 ||
					strcasecmp(ext, ".aac") == 0 ) {
					is_sound = true;
				}
			}
		}
		if (is_dir || is_sound) {
			sys_ctx->current_directory.items = realloc(sys_ctx->current_directory.items,
				(sys_ctx->current_directory.item_count + 1) * sizeof(file_item_t));
			assert_param(sys_ctx->current_directory.items != NULL);
			printf("Found item: %s\n", fno.fname);
			sprintf(sys_ctx->current_directory.items[sys_ctx->current_directory.item_count].name, "%s", fno.fname);
			sys_ctx->current_directory.items[sys_ctx->current_directory.item_count].is_directory = is_dir;
			sys_ctx->current_directory.item_count++;
		}
	}

	f_closedir(&dir);
	// Sort items: directories first, then files
	for (int i = 0; i < sys_ctx->current_directory.item_count - 1; i++) {
		for (int j = i + 1; j < sys_ctx->current_directory.item_count; j++) {
			if (!sys_ctx->current_directory.items[i].is_directory && sys_ctx->current_directory.items[j].is_directory) {
				file_item_t temp = sys_ctx->current_directory.items[i];
				sys_ctx->current_directory.items[i] = sys_ctx->current_directory.items[j];
				sys_ctx->current_directory.items[j] = temp;
			}
		}
	}
	// print the directory structure
	printf("Directory structure for %s:\n", sys_ctx->current_directory.path);
	for (int i = 0; i < sys_ctx->current_directory.item_count; i++) {
		printf("%s %s\n", sys_ctx->current_directory.items[i].is_directory ? "[D]" : "[F]", sys_ctx->current_directory.items[i].name);
	}
}
