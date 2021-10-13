// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    lv_style_set_border_opa(ui->main_lmeter_temp, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(ui->main_lmeter_humidity, LV_STATE_DEFAULT, 0);

    lv_img_set_src(ui->main_img_day1,  &icon_weather_0 );
}

#define LABEL_PROPERTY_TEXT 0
void set_label_property(lv_obj_t * target, int id, char * val)
{
    if(id == LABEL_PROPERTY_TEXT) lv_label_set_text(target, val);
}

