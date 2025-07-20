/*
 * screens.c
 *
 *  Created on: Jun 22, 2025
 *      Author: mosta
 */

#include "lvgl.h"
#include "screens.h"
#include "stdio.h"
#include "File_Handling.h"
static file_browser_t browser;

// Function prototypes
static void create_file_browser_ui(void);
static void update_list_display(void);
static void handle_encoder_input(lv_event_t *e);
static void scroll_list(int direction);
static void select_item(void);

void show_usb_check_screen(void)
{
    // Clean the active screen
    lv_obj_clean(lv_scr_act());

    // Create a container to center contents vertically
    lv_obj_t *container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_center(container);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 0, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // Create the label
    lv_obj_t *label = lv_label_create(container);
    lv_label_set_text(label, "Checking USB...");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0); // Use your preferred font
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -30);

    // Create the spinner below the label
    lv_obj_t *spinner = lv_spinner_create(container, 1000, 60); // 1000ms rotation, 60° arc
    lv_obj_set_size(spinner, 40, 40);
    lv_obj_align_to(spinner, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
}

// Create the file browser UI
static void create_file_browser_ui(void)
{
    // Create screen
    browser.screen = lv_scr_act();
    lv_obj_set_scrollbar_mode(browser.screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(browser.screen, LV_OBJ_FLAG_SCROLLABLE);
    // Create path label at top
    browser.path_label = lv_label_create(browser.screen);
    lv_obj_set_width(browser.path_label, LV_PCT(100));
    lv_obj_align(browser.path_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(browser.path_label, "/");
    lv_obj_set_style_text_align(browser.path_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_color(browser.path_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(browser.path_label, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_ver(browser.path_label, 0, 0);
    lv_coord_t h = lv_obj_get_height(browser.path_label);
    // Create container for list items
    browser.list = lv_obj_create(browser.screen);
    lv_obj_set_size(browser.list, LV_PCT(100), LV_PCT(100) - 2 * h);
    lv_obj_align_to(browser.list, browser.path_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_all(browser.list, 0, 0);
    lv_obj_set_style_border_width(browser.list, 1, 0);
    lv_obj_clear_flag(browser.list, LV_OBJ_FLAG_SCROLLABLE);
    h = lv_obj_get_height(browser.list);
    const int itemHeight = h / 6;
    // Create 5 list item labels
    for (int i = 0; i < 5; i++)
    {
        browser.list_items[i] = lv_label_create(browser.list);
        lv_obj_set_width(browser.list_items[i], LV_PCT(100));
        lv_obj_set_pos(browser.list_items[i], 0, i * itemHeight);
        lv_obj_set_height(browser.list_items[i], itemHeight);
        lv_obj_set_style_pad_left(browser.list_items[i], 10, 0);
        lv_obj_set_style_pad_ver(browser.list_items[i], 5, 0);
        lv_label_set_text(browser.list_items[i], "");

        // Add background for better visibility
        lv_obj_set_style_bg_opa(browser.list_items[i], LV_OPA_COVER, 0);
        lv_obj_set_style_bg_color(browser.list_items[i], lv_color_hex(0x202020), 0);
        lv_obj_set_style_border_width(browser.list_items[i], 1, 0);
        lv_obj_set_style_border_color(browser.list_items[i], lv_color_hex(0x404040), 0);
    }

    // Create input group for external key events
    browser.input_group = lv_group_create();
    lv_group_add_obj(browser.input_group, browser.screen);

    // Add event handler for key input (from lv_group_send_data)
    lv_obj_add_event_cb(browser.screen, handle_encoder_input, LV_EVENT_KEY, NULL);

    // Make screen focusable
    lv_obj_add_flag(browser.screen, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_outline_width(browser.screen, 0, LV_STATE_FOCUSED);

    // Initialize variables
    browser.selected_index = 0;
    browser.scroll_offset = 0;
    browser.total_items = 0;
}

// Update the list display based on current data and scroll position
static void update_list_display(void)
{
    if (!browser.current_dir)
        return;

    // Update path
    lv_label_set_text(browser.path_label, browser.current_dir->path);

    // Calculate total items (add parent directory if not root)
    browser.total_items = browser.current_dir->item_count;
    bool has_parent = strcmp(browser.current_dir->path, "/") != 0;
    if (has_parent)
    {
        browser.total_items++;
    }

    // Update list items
    for (int i = 0; i < 5; i++)
    {
        int item_index = browser.scroll_offset + i;
        bool is_selected = (item_index == browser.selected_index);

        if (item_index < browser.total_items)
        {
            char display_text[80];
            const char *icon = "";

            if (has_parent && item_index == 0)
            {
                // Parent directory
                snprintf(display_text, sizeof(display_text), "%s   ..",
                         is_selected ? "> " : "  ");
                lv_obj_set_style_bg_color(browser.list_items[i],
                                          is_selected ? lv_color_hex(0x0080FF) : lv_color_hex(0x202020), 0);
            }
            else
            {
                // Regular item
                int data_index = has_parent ? item_index - 1 : item_index;
                file_item_t *item = &browser.current_dir->items[data_index];

                icon = item->is_directory ? "[D]" : "[F]";
                snprintf(display_text, sizeof(display_text), "%s%s %s",
                         is_selected ? "> " : "  ", icon, item->name);

                lv_obj_set_style_bg_color(browser.list_items[i],
                                          is_selected ? lv_color_hex(0x0080FF) : lv_color_hex(0x202020), 0);
            }

            lv_label_set_text(browser.list_items[i], display_text);
            lv_obj_clear_flag(browser.list_items[i], LV_OBJ_FLAG_HIDDEN);

            // Update text color for selection
            lv_obj_set_style_text_color(browser.list_items[i],
                                        is_selected ? lv_color_white() : lv_color_hex(0xCCCCCC), 0);
        }
        else
        {
            // Hide unused items
            lv_obj_add_flag(browser.list_items[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
}

// Handle key events (UP/DOWN/ENTER) sent via lv_group_send_data
static void handle_encoder_input(lv_event_t *e)
{
    uint32_t key = lv_event_get_key(e);

    switch (key)
    {
    case LV_KEY_UP:
        scroll_list(-1);
        break;
    case LV_KEY_DOWN:
        scroll_list(1);
        break;
    case LV_KEY_ENTER:
        select_item();
        break;
    }
}


// Scroll the list up or down
static void scroll_list(int direction)
{
    if (browser.total_items == 0)
        return;

    int new_index = browser.selected_index + direction;

    // Clamp to valid range
    if (new_index < 0)
    {
        new_index = 0;
    }
    else if (new_index >= browser.total_items)
    {
        new_index = browser.total_items - 1;
    }

    browser.selected_index = new_index;

    // Adjust scroll offset to keep selection visible
    if (browser.selected_index < browser.scroll_offset)
    {
        browser.scroll_offset = browser.selected_index;
    }
    else if (browser.selected_index >= browser.scroll_offset + 5)
    {
        browser.scroll_offset = browser.selected_index - 4;
    }

    // Ensure scroll offset is valid
    if (browser.scroll_offset < 0)
    {
        browser.scroll_offset = 0;
    }

    update_list_display();
}

/**
 * Callback when an item is selected in the file browser.
 * Refactored to use sys_context_t *sys_ctx for directory navigation.
 * Updates sys_ctx->current_directory.path as well.
 */
static void on_item_selected(file_item_t *item, const char *current_path)
{
    sys_context_t *sys_ctx = browser.sys_ctx;
    if (!sys_ctx) return;

    printf("Selected: %s (is_dir: %d) from path: %s\n",
           item->name, item->is_directory, current_path);

    if (strcmp(item->name, "..") == 0)
        {
            // Navigate to parent directory
            char *last_slash = strrchr(sys_ctx->current_directory.path, '/');
            if (last_slash && last_slash != sys_ctx->current_directory.path)
            {
                size_t new_length = last_slash - sys_ctx->current_directory.path;
                char new_path[256];
                strncpy(new_path, sys_ctx->current_directory.path, new_length);
                new_path[new_length] = '\0';
                strncpy(sys_ctx->current_directory.path, new_path, sizeof(sys_ctx->current_directory.path) - 1);
                sys_ctx->current_directory.path[sizeof(sys_ctx->current_directory.path) - 1] = '\0';
                Scan_USB_Dir(sys_ctx);
                file_browser_set_directory(&sys_ctx->current_directory);
            }
            else
            {
                // Already at root
                strncpy(sys_ctx->current_directory.path, "/", sizeof(sys_ctx->current_directory.path) - 1);
                sys_ctx->current_directory.path[sizeof(sys_ctx->current_directory.path) - 1] = '\0';
                Scan_USB_Dir(sys_ctx);
                file_browser_set_directory(&sys_ctx->current_directory);
            }
        }
        else if (item->is_directory)
        {
            // Navigate into directory
            char new_path[256];
            if (strcmp(sys_ctx->current_directory.path, "/") == 0)
                snprintf(new_path, sizeof(new_path), "/%s", item->name);
            else
                snprintf(new_path, sizeof(new_path), "%s/%s", sys_ctx->current_directory.path, item->name);
    
            strncpy(sys_ctx->current_directory.path, new_path, sizeof(sys_ctx->current_directory.path) - 1);
            sys_ctx->current_directory.path[sizeof(sys_ctx->current_directory.path) - 1] = '\0';
            Scan_USB_Dir(sys_ctx);
            file_browser_set_directory(&sys_ctx->current_directory);
        }
    else
    {
        // Handle file selection
        printf("File selected: %s/%s\n", sys_ctx->current_directory.path, item->name);
        // Implement file open or playback as needed
        sm_post_event(&sm_ctx, EVENT_SELECT_ITEM, item);
    }
}

// Select the current item
static void select_item(void)
{
    if (!browser.current_dir || browser.total_items == 0)
        return;

    bool has_parent = strcmp(browser.current_dir->path, "/") != 0;

    if (has_parent && browser.selected_index == 0)
    {
        // Parent directory selected - create a special item for it
        static file_item_t parent_item = {"..", true};
        on_item_selected(&parent_item, browser.current_dir->path);
    }
    else
    {
        // Regular item selected
        int data_index = has_parent ? browser.selected_index - 1 : browser.selected_index;
        if (data_index >= 0 && data_index < browser.current_dir->item_count)
        {
            file_item_t *selected = &browser.current_dir->items[data_index];
            on_item_selected(selected, browser.current_dir->path);
        }
    }
}

// Public API functions

// Initialize the file browser (no encoder dependency)
void file_browser_init(sys_context_t* sys_ctx)
{

    create_file_browser_ui();
    browser.sys_ctx = sys_ctx;
    Scan_USB_Dir(sys_ctx); // Load initial directory structure
    file_browser_set_directory(&sys_ctx->current_directory);
}

// Set the directory data
void file_browser_set_directory(directory_t *dir)
{
    browser.current_dir = dir;
    browser.selected_index = 0;
    browser.scroll_offset = 0;
    update_list_display();
}

// Get the input group for external key events
lv_group_t *file_browser_get_input_group(void)
{
    return browser.input_group;
}

// Get the file browser screen object
lv_obj_t *file_browser_get_screen(void)
{
    return browser.screen;
}

// Optionally, allow external system to send key events to the group
void file_browser_send_key(uint32_t key)
{
    lv_group_send_data(browser.input_group, key);
}
