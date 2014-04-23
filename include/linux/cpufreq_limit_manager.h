/*
 * Author: Alucard_24 XDA
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

#ifndef _LINUX_CPUFREQ_LIMIT_MANAGER_H
#define _LINUX_CPUFREQ_LIMIT_MANAGER_H

#ifdef CONFIG_LOW_CPUCLOCKS
#define CPU_MIN_FREQ	162000
#else
#define CPU_MIN_FREQ	384000
#endif

#define CPU_MAX_FREQ	1890000
#define CPU_MAX_ONCALL_FREQ	1134000

int update_cpufreq_limit(unsigned int limit_type, bool limit_status);

#endif
