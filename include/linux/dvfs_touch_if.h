/*
 * Author: Chad Froebel <chadfroebel@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_DVFS_TOUCH_IF_H
#define _LINUX_DVFS_TOUCH_IF_H

#define CPU_MIN_FREQ	486000
#define CPU_MAX_FREQ	1890000

#define DVFS_MIN_TOUCH_LIMIT	1134000
#define DVFS_MIN_TOUCH_LIMIT_SECOND	810000

#define SYN_TOUCH_BOOSTER_OFF_TIME	300
#define SYN_TOUCH_BOOSTER_CHG_TIME	200
#define MXTS_TOUCH_BOOSTER_OFF_TIME 100
#define CYP_TOUCH_BOOSTER_OFF_TIME	300
#define CYP_TOUCH_BOOSTER_CHG_TIME	200
#define GPIO_KEY_BOOSTER_OFF_TIME	300
#define GPIO_KEY_BOOSTER_CHG_TIME	200

extern atomic_t dvfs_boost_mode;
extern atomic_t dvfs_min_touch_limit;
extern atomic_t dvfs_min_touch_limit_second;
extern atomic_t syn_touch_booster_off_time;
extern atomic_t syn_touch_booster_chg_time;
extern atomic_t mxts_touch_booster_off_time;
extern atomic_t cyp_touch_booster_off_time;
extern atomic_t cyp_touch_booster_chg_time;
extern atomic_t gpio_key_booster_off_time;
extern atomic_t gpio_key_booster_chg_time;

#endif
