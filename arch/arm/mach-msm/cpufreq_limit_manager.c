/*
 * Author: Alucard_24 <alucard_24 XDA>
 *
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

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <mach/cpufreq.h>
#include <linux/cpu.h>
#include <linux/cpufreq_limit_manager.h>

#if defined(CONFIG_POWERSUSPEND)
#include <linux/powersuspend.h>
#elif defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
#endif  /* CONFIG_POWERSUSPEND || CONFIG_HAS_EARLYSUSPEND */

unsigned int scaling_min_suspend_freq = 378000;
unsigned int scaling_max_suspend_freq = CPU_MAX_FREQ;
unsigned int scaling_max_oncall_freq = CPU_MAX_ONCALL_FREQ;
unsigned int scaling_max_gps_freq = CPU_MAX_FREQ;

static bool suspended = false;
static bool gps_status = false;
static bool oncall_status = false;

int update_cpufreq_limit(unsigned int limit_type, bool limit_status)
{
	unsigned int cpu;
	unsigned int min_freq = MSM_CPUFREQ_NO_LIMIT;
	unsigned int max_freq = MSM_CPUFREQ_NO_LIMIT;

	switch (limit_type) {
	case 0:
		/* SUSPEND */
		suspended = limit_status;
		break;
	case 1:
		/* CALL */
		oncall_status = limit_status;
		break;
	case 2:
		/* GPS */
		gps_status = limit_status;
		break;
	default:
		break;
	}

	if (suspended) {
		min_freq = scaling_min_suspend_freq;
		max_freq = scaling_max_suspend_freq;
	}
	
	if (oncall_status && suspended) {
		min_freq = MSM_CPUFREQ_NO_LIMIT;
		max_freq = scaling_max_oncall_freq;
	}
		
	if (gps_status) {
		min_freq = MSM_CPUFREQ_NO_LIMIT;
		max_freq = scaling_max_gps_freq;
	}

	for_each_possible_cpu(cpu) {
		msm_cpufreq_set_freq_limits(cpu, min_freq, max_freq);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(update_cpufreq_limit);

#if defined(CONFIG_POWERSUSPEND) || defined(CONFIG_HAS_EARLYSUSPEND)
#ifdef CONFIG_POWERSUSPEND
static void __ref cpufreq_limit_suspend(struct power_suspend *handler)
#else
static void __ref cpufreq_limit_suspend(struct early_suspend *handler)
#endif
{
 	/* SUSPEND */
	update_cpufreq_limit(0, true);
}

#ifdef CONFIG_POWERSUSPEND
static void __cpuinit cpufreq_limit_late_resume(struct power_suspend *handler)
#else
static void __cpuinit cpufreq_limit_late_resume(
				struct early_suspend *handler)
#endif
{
	/* RESUME */
	update_cpufreq_limit(0, false);
}

#ifdef CONFIG_POWERSUSPEND
static struct power_suspend cpufreq_limit_suspend_driver = {
	.suspend = cpufreq_limit_suspend,
	.resume = cpufreq_limit_late_resume,
};
#else
static struct early_suspend cpufreq_limit_suspend_driver = {
	.level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 10,
	.suspend = cpufreq_limit_suspend,
	.resume = cpufreq_limit_late_resume,
};
#endif
#endif  /* CONFIG_POWERSUSPEND || CONFIG_HAS_EARLYSUSPEND */

static ssize_t show_scaling_min_suspend_freq(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_min_suspend_freq);
}

static ssize_t show_scaling_max_suspend_freq(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_max_suspend_freq);
}

static ssize_t show_scaling_max_oncall_freq(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_max_oncall_freq);
}

static ssize_t show_scaling_max_gps_freq(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_max_gps_freq);
}

static ssize_t store_scaling_min_suspend_freq(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, CPU_MAX_FREQ), CPU_MIN_FREQ);

	if (input != scaling_min_suspend_freq) {
		/* update only if valid value provided */
		scaling_min_suspend_freq = input;
	}

	return count;
}

static ssize_t store_scaling_max_suspend_freq(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, CPU_MAX_FREQ), CPU_MIN_FREQ);

	if (input != scaling_max_suspend_freq) {
		/* update only if valid value provided */
		scaling_max_suspend_freq = input;
	}

	return count;
}

static ssize_t store_scaling_max_oncall_freq(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, CPU_MAX_FREQ), CPU_MIN_FREQ);

	if (input != scaling_max_oncall_freq) {
		/* update only if valid value provided */
		scaling_max_oncall_freq = input;
	}

	return count;
}

static ssize_t store_scaling_max_gps_freq(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, CPU_MAX_FREQ), CPU_MIN_FREQ);

	if (input != scaling_max_gps_freq) {
		/* update only if valid value provided */
		scaling_max_gps_freq = input;
	}

	return count;
}

static struct kobj_attribute scaling_min_suspend_freq_attr =
	__ATTR(scaling_min_suspend_freq, 0666, show_scaling_min_suspend_freq,
			store_scaling_min_suspend_freq);

static struct kobj_attribute scaling_max_suspend_freq_attr =
	__ATTR(scaling_max_suspend_freq, 0666, show_scaling_max_suspend_freq,
			store_scaling_max_suspend_freq);

static struct kobj_attribute scaling_max_oncall_freq_attr =
	__ATTR(scaling_max_oncall_freq, 0666, show_scaling_max_oncall_freq,
			store_scaling_max_oncall_freq);

static struct kobj_attribute scaling_max_gps_freq_attr =
	__ATTR(scaling_max_gps_freq, 0666, show_scaling_max_gps_freq,
			store_scaling_max_gps_freq);

static struct attribute *cpufreq_limit_manager_attrs[] = {
	&scaling_min_suspend_freq_attr.attr,
	&scaling_max_suspend_freq_attr.attr,
	&scaling_max_oncall_freq_attr.attr,
	&scaling_max_gps_freq_attr.attr,
	NULL,
};

static struct attribute_group cpufreq_limit_manager_attr_group = {
	.attrs = cpufreq_limit_manager_attrs,
	.name = "cpufreq_limit_manager",
};

static int __init cpufreq_limit_manager_init(void)
{
	int rc;

	rc = sysfs_create_group(kernel_kobj, &cpufreq_limit_manager_attr_group);

	if (rc) {
		pr_info("%s sysfs create failed!\n", __FUNCTION__);
		return -EFAULT;
	}

#if defined(CONFIG_POWERSUSPEND)
	register_power_suspend(&cpufreq_limit_suspend_driver);
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	register_early_suspend(&cpufreq_limit_suspend_driver);
#endif  /* CONFIG_POWERSUSPEND || CONFIG_HAS_EARLYSUSPEND */

	return (rc);
}

static int __exit cpufreq_limit_manager_exit(void)
{
#if defined(CONFIG_POWERSUSPEND)
	unregister_power_suspend(&cpufreq_limit_suspend_driver);
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	unregister_early_suspend(&cpufreq_limit_suspend_driver);
#endif  /* CONFIG_POWERSUSPEND || CONFIG_HAS_EARLYSUSPEND */

	sysfs_remove_group(kernel_kobj, &cpufreq_limit_manager_attr_group);

	return 0;
}

MODULE_AUTHOR("Alucard_24 XDA");
MODULE_DESCRIPTION("CPUFREQ Limit manager");
MODULE_LICENSE("GPL");
late_initcall(cpufreq_limit_manager_init);
late_initexit(cpufreq_limit_manager_exit);
