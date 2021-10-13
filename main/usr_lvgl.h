/*
 * usr_lvgl.h
 *
 *  Created on: 2021��8��26��
 *      Author: Bovey
 */

#ifndef MAIN_USR_LVGL_H_
#define MAIN_USR_LVGL_H_

void usr_lvgl(void);
void time_update_to_lcd(char * data);
void temp_update_to_lcd(float data);
void humi_update_to_lcd(float data);
void weather_update_to_ldc(int day, char code_day);

#endif /* MAIN_USR_LVGL_H_ */
