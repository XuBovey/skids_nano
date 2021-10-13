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

#include "icon_weather_0_sunny.h"
#include "icon_weather_1_clear.h"
#include "icon_weather_4_cloudy.h"
#include "icon_weather_5_partly_cloudy.h"
#include "icon_weather_6_partly_cloudy.h"
#include "icon_weather_9_overcast.h"
#include "icon_weather_10_shower.h"

// #include "icon_weather_11_thundershowicon_weather_er.h"
// #include "icon_weather_12_thundershower_with_hail.h"
// #include "icon_weather_13_light_rain.h"
// #include "icon_weather_14_moderate_rain.h"
// #include "icon_weather_15_heavy_rain.h"
// #include "icon_weather_16_storm.h"
// #include "icon_weather_17_heavy_storm.h"
// #include "icon_weather_18_severe_storm.h"
// #include "icon_weather_19_ice_rain.h"
// #include "icon_weather_20_sleet.h"

// #include "icon_weather_21_snow_flurry.h"
// #include "icon_weather_22_light_snow.h"
// #include "icon_weather_23_moderate_snow.h"
// #include "icon_weather_24_heavy_snow.h"
// #include "icon_weather_25_snowstorm.h"
// #include "icon_weather_26_dust.h"
// #include "icon_weather_27_sand.h"
// #include "icon_weather_28_duststorm.h"
// #include "icon_weather_29_sandstorm.h"
// #include "icon_weather_30_foggy.h"

// #include "icon_weather_31_haze.h"
// #include "icon_weather_32_windy.h"
// #include "icon_weather_33_blustery.h"
// #include "icon_weather_34_hurricane.h"
// #include "icon_weather_36_tornado.h"
// #include "icon_weather_37_cold.h"
#include "icon_weather_99_unknown.h"

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
const lv_img_dsc_t *icon_unknown = &icon_weather_99_unknown;
const lv_img_dsc_t *icon_table[] = {
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                &icon_weather_0_sunny, 
                                // &icon_weather_1_clear, 
                                // &icon_weather_0_sunny,
                                // &icon_weather_1_clear,
                                // &icon_weather_4_cloudy,
                                // &icon_weather_5_partly_cloudy,
                                // &icon_weather_6_partly_cloudy,
                                // &icon_weather_5_partly_cloudy, // 7
                                // &icon_weather_6_partly_cloudy, // 8
                                // &icon_weather_9_overcast,
                                // &icon_weather_10_shower,
                                // &icon_weather_11_thundershowicon_weather_er,
                                // &icon_weather_12_thundershower_with_hail,
                                // &icon_weather_13_light_rain,
                                // &icon_weather_14_moderate_rain,
                                // &icon_weather_15_heavy_rain,
                                // &icon_weather_16_storm,
                                // &icon_weather_17_heavy_storm,
                                // &icon_weather_18_severe_storm,
                                // &icon_weather_19_ice_rain,
                                // &icon_weather_20_sleet,
                                // &icon_weather_21_snow_flurry,
                                // &icon_weather_22_light_snow,
                                // &icon_weather_23_moderate_snow,
                                // &icon_weather_24_heavy_snow,
                                // &icon_weather_25_snowstorm,
                                // &icon_weather_26_dust,
                                // &icon_weather_27_sand,
                                // &icon_weather_28_duststorm,
                                // &icon_weather_29_sandstorm,
                                // &icon_weather_30_foggy,
                                // &icon_weather_31_haze,
                                // &icon_weather_32_windy,
                                // &icon_weather_33_blustery,
                                // &icon_weather_34_hurricane,
                                // &icon_weather_36_tornado,
                                // &icon_weather_37_cold,
                                &icon_weather_0_sunny 
                                };

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    lv_style_set_border_opa(ui->main_lmeter_temp, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(ui->main_lmeter_humidity, LV_STATE_DEFAULT, 0);

    lv_img_set_src(ui->main_img_day1, icon_table[0] );
}

#define LABEL_PROPERTY_TEXT 0
void set_label_property(lv_obj_t * target, int id, char * val)
{
    if(id == LABEL_PROPERTY_TEXT) lv_label_set_text(target, val);
    return;
}

