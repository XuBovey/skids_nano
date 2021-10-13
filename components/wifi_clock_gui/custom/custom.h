// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_

#include "gui_guider.h"

void custom_init(lv_ui *ui);
void set_label_property(lv_obj_t * target, int id, char * val);

extern const lv_img_dsc_t *icon_table[];
extern const lv_img_dsc_t *icon_unknown;

#endif /* EVENT_CB_H_ */
