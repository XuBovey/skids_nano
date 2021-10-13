/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"


void setup_scr_main1(lv_ui *ui){

	//Write codes main1
	ui->main1 = lv_obj_create(NULL, NULL);

	//Write codes main1_back
	ui->main1_back = lv_cont_create(ui->main1, NULL);

	//Write style LV_CONT_PART_MAIN for main1_back
	static lv_style_t style_main1_back_main;
	lv_style_init(&style_main1_back_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_back_main
	lv_style_set_radius(&style_main1_back_main, LV_STATE_DEFAULT, 1);
	lv_style_set_bg_color(&style_main1_back_main, LV_STATE_DEFAULT, lv_color_make(0xf2, 0xf2, 0xf2));
	lv_style_set_bg_grad_color(&style_main1_back_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_main1_back_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_back_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_main1_back_main, LV_STATE_DEFAULT, lv_color_make(0x75, 0x75, 0x75));
	lv_style_set_border_width(&style_main1_back_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_main1_back_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_main1_back_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_main1_back_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_main1_back_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_main1_back_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->main1_back, LV_CONT_PART_MAIN, &style_main1_back_main);
	lv_obj_set_pos(ui->main1_back, 0, 0);
	lv_obj_set_size(ui->main1_back, 240, 240);
	lv_obj_set_click(ui->main1_back, false);

	//Write codes main1_img_day3
	ui->main1_img_day3 = lv_img_create(ui->main1_back, NULL);

	//Write style LV_IMG_PART_MAIN for main1_img_day3
	static lv_style_t style_main1_img_day3_main;
	lv_style_init(&style_main1_img_day3_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_img_day3_main
	lv_style_set_image_recolor(&style_main1_img_day3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_main1_img_day3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_main1_img_day3_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->main1_img_day3, LV_IMG_PART_MAIN, &style_main1_img_day3_main);
	lv_obj_set_pos(ui->main1_img_day3, 165, 5);
	lv_obj_set_size(ui->main1_img_day3, 65, 65);

	//Write codes main1_img_day1
	ui->main1_img_day1 = lv_img_create(ui->main1_back, NULL);

	//Write style LV_IMG_PART_MAIN for main1_img_day1
	static lv_style_t style_main1_img_day1_main;
	lv_style_init(&style_main1_img_day1_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_img_day1_main
	lv_style_set_image_recolor(&style_main1_img_day1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_main1_img_day1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_main1_img_day1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->main1_img_day1, LV_IMG_PART_MAIN, &style_main1_img_day1_main);
	lv_obj_set_pos(ui->main1_img_day1, 5, 5);
	lv_obj_set_size(ui->main1_img_day1, 65, 65);

	//Write codes main1_img_day2
	ui->main1_img_day2 = lv_img_create(ui->main1_back, NULL);

	//Write style LV_IMG_PART_MAIN for main1_img_day2
	static lv_style_t style_main1_img_day2_main;
	lv_style_init(&style_main1_img_day2_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_img_day2_main
	lv_style_set_image_recolor(&style_main1_img_day2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_main1_img_day2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_main1_img_day2_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->main1_img_day2, LV_IMG_PART_MAIN, &style_main1_img_day2_main);
	lv_obj_set_pos(ui->main1_img_day2, 85, 5);
	lv_obj_set_size(ui->main1_img_day2, 65, 65);

	//Write codes main1_label_time
	ui->main1_label_time = lv_label_create(ui->main1_back, NULL);
	lv_label_set_text(ui->main1_label_time, "default");
	lv_label_set_long_mode(ui->main1_label_time, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->main1_label_time, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for main1_label_time
	static lv_style_t style_main1_label_time_main;
	lv_style_init(&style_main1_label_time_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_label_time_main
	lv_style_set_radius(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_main1_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_main1_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_main1_label_time_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_main1_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xfc, 0xfc, 0xfc));
	lv_style_set_text_font(&style_main1_label_time_main, LV_STATE_DEFAULT, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_main1_label_time_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_main1_label_time_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->main1_label_time, LV_LABEL_PART_MAIN, &style_main1_label_time_main);
	lv_obj_set_pos(ui->main1_label_time, 0, 180);
	lv_obj_set_size(ui->main1_label_time, 240, 0);
	lv_cont_set_layout(ui->main1_back, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->main1_back, LV_FIT_NONE);

	//Write codes main1_lmeter_temp
	ui->main1_lmeter_temp = lv_linemeter_create(ui->main1, NULL);

	//Write style LV_LINEMETER_PART_MAIN for main1_lmeter_temp
	static lv_style_t style_main1_lmeter_temp_main;
	lv_style_init(&style_main1_lmeter_temp_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_lmeter_temp_main
	lv_style_set_radius(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_right(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_top(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_bottom(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_line_color(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_line_width(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_line_opa(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 255);
	lv_style_set_scale_grad_color(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_scale_end_color(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, lv_color_make(0xfc, 0xf7, 0xfc));
	lv_style_set_scale_width(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 10);
	lv_style_set_scale_end_line_width(&style_main1_lmeter_temp_main, LV_STATE_DEFAULT, 2);
	lv_obj_add_style(ui->main1_lmeter_temp, LV_LINEMETER_PART_MAIN, &style_main1_lmeter_temp_main);
	lv_obj_set_pos(ui->main1_lmeter_temp, 10, 75);
	lv_obj_set_size(ui->main1_lmeter_temp, 100, 100);
	lv_linemeter_set_scale(ui->main1_lmeter_temp, 300, 20);
	lv_linemeter_set_range(ui->main1_lmeter_temp, 0, 100);
	lv_linemeter_set_value(ui->main1_lmeter_temp, 37);
	lv_linemeter_set_angle_offset(ui->main1_lmeter_temp, 0);

	//Write codes main1_lmeter_humidity
	ui->main1_lmeter_humidity = lv_linemeter_create(ui->main1, NULL);

	//Write style LV_LINEMETER_PART_MAIN for main1_lmeter_humidity
	static lv_style_t style_main1_lmeter_humidity_main;
	lv_style_init(&style_main1_lmeter_humidity_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_lmeter_humidity_main
	lv_style_set_radius(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_right(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_top(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_bottom(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_line_color(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_line_width(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_line_opa(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 255);
	lv_style_set_scale_grad_color(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xff, 0xff));
	lv_style_set_scale_end_color(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_scale_width(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 10);
	lv_style_set_scale_end_line_width(&style_main1_lmeter_humidity_main, LV_STATE_DEFAULT, 2);
	lv_obj_add_style(ui->main1_lmeter_humidity, LV_LINEMETER_PART_MAIN, &style_main1_lmeter_humidity_main);
	lv_obj_set_pos(ui->main1_lmeter_humidity, 130, 75);
	lv_obj_set_size(ui->main1_lmeter_humidity, 100, 100);
	lv_linemeter_set_scale(ui->main1_lmeter_humidity, 300, 20);
	lv_linemeter_set_range(ui->main1_lmeter_humidity, 0, 100);
	lv_linemeter_set_value(ui->main1_lmeter_humidity, 90);
	lv_linemeter_set_angle_offset(ui->main1_lmeter_humidity, 0);

	//Write codes main1_label_temp
	ui->main1_label_temp = lv_label_create(ui->main1, NULL);
	lv_label_set_text(ui->main1_label_temp, "37");
	lv_label_set_long_mode(ui->main1_label_temp, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->main1_label_temp, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for main1_label_temp
	static lv_style_t style_main1_label_temp_main;
	lv_style_init(&style_main1_label_temp_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_label_temp_main
	lv_style_set_radius(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_main1_label_temp_main, LV_STATE_DEFAULT, lv_color_make(0x30, 0x71, 0xe8));
	lv_style_set_bg_grad_color(&style_main1_label_temp_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_main1_label_temp_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_main1_label_temp_main, LV_STATE_DEFAULT, lv_color_make(0x0e, 0xf1, 0xe2));
	lv_style_set_text_font(&style_main1_label_temp_main, LV_STATE_DEFAULT, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_main1_label_temp_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_main1_label_temp_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->main1_label_temp, LV_LABEL_PART_MAIN, &style_main1_label_temp_main);
	lv_obj_set_pos(ui->main1_label_temp, 30, 110);
	lv_obj_set_size(ui->main1_label_temp, 64, 0);

	//Write codes main1_label_humidity
	ui->main1_label_humidity = lv_label_create(ui->main1, NULL);
	lv_label_set_text(ui->main1_label_humidity, "60");
	lv_label_set_long_mode(ui->main1_label_humidity, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->main1_label_humidity, LV_LABEL_ALIGN_CENTER);

	//Write style LV_LABEL_PART_MAIN for main1_label_humidity
	static lv_style_t style_main1_label_humidity_main;
	lv_style_init(&style_main1_label_humidity_main);

	//Write style state: LV_STATE_DEFAULT for style_main1_label_humidity_main
	lv_style_set_radius(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_main1_label_humidity_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_main1_label_humidity_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_main1_label_humidity_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_main1_label_humidity_main, LV_STATE_DEFAULT, lv_color_make(0x02, 0xf3, 0xf7));
	lv_style_set_text_font(&style_main1_label_humidity_main, LV_STATE_DEFAULT, &lv_font_montserratMedium_24);
	lv_style_set_text_letter_space(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_main1_label_humidity_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->main1_label_humidity, LV_LABEL_PART_MAIN, &style_main1_label_humidity_main);
	lv_obj_set_pos(ui->main1_label_humidity, 150, 110);
	lv_obj_set_size(ui->main1_label_humidity, 60, 0);
}