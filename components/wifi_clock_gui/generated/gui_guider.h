/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *main;
	lv_obj_t *main_back;
	lv_obj_t *main_img_day1;
	lv_obj_t *main_label_time;
	lv_obj_t *main_lmeter_temp;
	lv_obj_t *main_lmeter_humidity;
	lv_obj_t *main_label_temp;
	lv_obj_t *main_label_humidity;
	lv_obj_t *main1;
	lv_obj_t *main1_back;
	lv_obj_t *main1_img_day3;
	lv_obj_t *main1_img_day1;
	lv_obj_t *main1_img_day2;
	lv_obj_t *main1_label_time;
	lv_obj_t *main1_lmeter_temp;
	lv_obj_t *main1_lmeter_humidity;
	lv_obj_t *main1_label_temp;
	lv_obj_t *main1_label_humidity;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_main(lv_ui *ui);
void setup_scr_main1(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif