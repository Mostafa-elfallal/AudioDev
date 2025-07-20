/*
 * screens.h
 *
 *  Created on: Jun 22, 2025
 *      Author: mosta
 */

#ifndef SCREENS_SCREENS_H_
#define SCREENS_SCREENS_H_
#include "lvgl.h"
#include "states.h"
// Data structures


// UI components
typedef struct
{
    lv_obj_t *screen;
    lv_obj_t *path_label;
    lv_obj_t *list;
    lv_obj_t *list_items[5]; // Only 5 visible items
    lv_group_t *input_group;

    sys_context_t *sys_ctx; // Pointer to system context for directory data
    directory_t *current_dir; // Current directory being browsed
    int selected_index;
    int scroll_offset;
    int total_items;
} file_browser_t;
// USB Checking
void show_usb_check_screen(void);
// list
void file_browser_init(sys_context_t* sys_ctx);
void file_browser_set_directory(directory_t *dir);
lv_group_t *file_browser_get_input_group(void);
void file_browser_send_key(uint32_t key);
#endif /* SCREENS_SCREENS_H_ */
