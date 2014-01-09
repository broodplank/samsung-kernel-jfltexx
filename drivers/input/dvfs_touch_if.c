/*
 * Author: Chad Froebel <chadfroebel@gmail.com>
 *
 * Simple port to Nexus 4 : motley <motley.slate@gmail.com>
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
#include <linux/dvfs_touch_if.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>

atomic_t dvfs_min_touch_limit = ATOMIC_INIT(DVFS_MIN_TOUCH_LIMIT);
atomic_t dvfs_min_touch_limit_second = ATOMIC_INIT(DVFS_MIN_TOUCH_LIMIT_SECOND);
atomic_t syn_touch_booster_off_time = ATOMIC_INIT(SYN_TOUCH_BOOSTER_OFF_TIME);
atomic_t syn_touch_booster_chg_time = ATOMIC_INIT(SYN_TOUCH_BOOSTER_CHG_TIME);
atomic_t mxts_touch_booster_off_time = ATOMIC_INIT(MXTS_TOUCH_BOOSTER_OFF_TIME);
atomic_t cyp_touch_booster_off_time = ATOMIC_INIT(CYP_TOUCH_BOOSTER_OFF_TIME);
atomic_t cyp_touch_booster_chg_time = ATOMIC_INIT(CYP_TOUCH_BOOSTER_CHG_TIME);
atomic_t gpio_key_booster_off_time = ATOMIC_INIT(GPIO_KEY_BOOSTER_OFF_TIME);
atomic_t gpio_key_booster_chg_time = ATOMIC_INIT(GPIO_KEY_BOOSTER_CHG_TIME);

static ssize_t show_dvfs_min_touch_limit(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&dvfs_min_touch_limit));
}

static ssize_t show_dvfs_min_touch_limit_second(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&dvfs_min_touch_limit_second));
}

static ssize_t show_syn_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&syn_touch_booster_off_time));
}

static ssize_t show_syn_touch_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&syn_touch_booster_chg_time));
}

static ssize_t show_mxts_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&mxts_touch_booster_off_time));
}

static ssize_t show_cyp_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&cyp_touch_booster_off_time));
}

static ssize_t show_cyp_touch_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&cyp_touch_booster_chg_time));
}

static ssize_t show_gpio_key_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&gpio_key_booster_off_time));
}

static ssize_t show_gpio_key_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", atomic_read(&gpio_key_booster_chg_time));
}

static ssize_t store_dvfs_min_touch_limit(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1890000),810000);

	if (input != atomic_read(&dvfs_min_touch_limit)) {
		/* update only if valid value provided */
		atomic_set(&dvfs_min_touch_limit,input);
	}

	return count;
}

static ssize_t store_dvfs_min_touch_limit_second(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1350000),486000);

	if (input != atomic_read(&dvfs_min_touch_limit_second)) {
		/* update only if valid value provided */
		atomic_set(&dvfs_min_touch_limit_second,input);
	}

	return count;
}

static ssize_t store_syn_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 5000),200);

	if (input !=  atomic_read(&syn_touch_booster_off_time)) {
		/* update only if valid value provided */
		atomic_set(&syn_touch_booster_off_time,input);
	}

	return count;
}

static ssize_t store_syn_touch_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1000),100);

	if (input != atomic_read(&syn_touch_booster_chg_time)) {
		/* update only if valid value provided */
		atomic_set(&syn_touch_booster_chg_time, input);
	}

	return count;
}

static ssize_t store_mxts_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1000),100);

	if (input !=  atomic_read(&mxts_touch_booster_off_time)) {
		/* update only if valid value provided */
		atomic_set(&mxts_touch_booster_off_time,input);
	}

	return count;
}

static ssize_t store_cyp_touch_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 5000),200);

	if (input !=  atomic_read(&cyp_touch_booster_off_time)) {
		/* update only if valid value provided */
		atomic_set(&cyp_touch_booster_off_time,input);
	}

	return count;
}

static ssize_t store_cyp_touch_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1000),100);

	if (input != atomic_read(&cyp_touch_booster_chg_time)) {
		/* update only if valid value provided */
		atomic_set(&cyp_touch_booster_chg_time, input);
	}

	return count;
}

static ssize_t store_gpio_key_booster_off_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1000),200);

	if (input !=  atomic_read(&gpio_key_booster_off_time)) {
		/* update only if valid value provided */
		atomic_set(&gpio_key_booster_off_time,input);
	}

	return count;
}

static ssize_t store_gpio_key_booster_chg_time(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = max(min(input, 1000),100);

	if (input !=  atomic_read(&gpio_key_booster_chg_time)) {
		/* update only if valid value provided */
		atomic_set(&gpio_key_booster_chg_time,input);
	}

	return count;
}

static struct kobj_attribute dvfs_min_touch_limit_attr =
	__ATTR(dvfs_min_touch_limit, 0666, show_dvfs_min_touch_limit,
			store_dvfs_min_touch_limit);

static struct kobj_attribute dvfs_min_touch_limit_second_attr =
	__ATTR(dvfs_min_touch_limit_second, 0666, show_dvfs_min_touch_limit_second,
			store_dvfs_min_touch_limit_second);

static struct kobj_attribute syn_touch_booster_off_time_attr =
	__ATTR(syn_touch_booster_off_time, 0666, show_syn_touch_booster_off_time,
			store_syn_touch_booster_off_time);

static struct kobj_attribute syn_touch_booster_chg_time_attr =
	__ATTR(syn_touch_booster_chg_time, 0666, show_syn_touch_booster_chg_time,
			store_syn_touch_booster_chg_time);

static struct kobj_attribute mxts_touch_booster_off_time_attr =
	__ATTR(mxts_touch_booster_off_time, 0666, show_mxts_touch_booster_off_time,
			store_mxts_touch_booster_off_time);

static struct kobj_attribute cyp_touch_booster_off_time_attr =
	__ATTR(cyp_touch_booster_off_time, 0666, show_cyp_touch_booster_off_time,
			store_cyp_touch_booster_off_time);

static struct kobj_attribute cyp_touch_booster_chg_time_attr =
	__ATTR(cyp_touch_booster_chg_time, 0666, show_cyp_touch_booster_chg_time,
			store_cyp_touch_booster_chg_time);

static struct kobj_attribute gpio_key_booster_off_time_attr =
	__ATTR(gpio_key_booster_off_time, 0666, show_gpio_key_booster_off_time,
			store_gpio_key_booster_off_time);

static struct kobj_attribute gpio_key_booster_chg_time_attr =
	__ATTR(gpio_key_booster_chg_time, 0666, show_gpio_key_booster_chg_time,
			store_gpio_key_booster_chg_time);

static struct attribute *dvfs_touch_if_attrs[] = {
	&dvfs_min_touch_limit_attr.attr,
	&dvfs_min_touch_limit_second_attr.attr,
	&syn_touch_booster_off_time_attr.attr,
	&syn_touch_booster_chg_time_attr.attr,
	&mxts_touch_booster_off_time_attr.attr,
	&cyp_touch_booster_off_time_attr.attr,
	&cyp_touch_booster_chg_time_attr.attr,
	&gpio_key_booster_off_time_attr.attr,
	&gpio_key_booster_chg_time_attr.attr,
	NULL,
};

static struct attribute_group dvfs_touch_if_attr_group = {
	.attrs = dvfs_touch_if_attrs,
};

/* Initialize dvfs_touch_if sysfs folder */
static struct kobject *dvfs_touch_if_kobj;


int dvfs_touch_if_init(void)
{
	int rc;

	dvfs_touch_if_kobj = kobject_create_and_add("dvfs_touch_if", kernel_kobj);
	if (!dvfs_touch_if_kobj) {
			return -ENOMEM;
	}

	rc = sysfs_create_group(dvfs_touch_if_kobj, &dvfs_touch_if_attr_group);

	if (rc)
		kobject_put(dvfs_touch_if_kobj);

	return (rc);
}

void dvfs_touch_if_exit(void)
{
	kobject_put(dvfs_touch_if_kobj);
}

module_init(dvfs_touch_if_init);
module_exit(dvfs_touch_if_exit);
