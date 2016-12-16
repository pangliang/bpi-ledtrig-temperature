/*
 * LED Heartbeat Trigger
 *
 * Copyright (C) 2006 Atsushi Nemoto <anemo@mba.ocn.ne.jp>
 *
 * Based on Richard Purdie's ledtrig-timer.c and some arch's
 * CONFIG_HEARTBEAT code.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/leds.h>
#include "leds.h"

struct temperature_trig_data {
	unsigned int phase;
	unsigned int t;
	struct timer_list timer;
};

extern int ths_read_data(int value);

static void led_temperature_function(unsigned long data)
{
	struct led_classdev *led_cdev = (struct led_classdev *) data;
	struct temperature_trig_data *temperature_data = led_cdev->trigger_data;
	unsigned long brightness = LED_OFF;
	unsigned long delay = 0;

	switch (temperature_data->phase) {
	case 0:
		delay = msecs_to_jiffies(100);
		temperature_data->phase++;
		brightness = led_cdev->max_brightness;
		break;
	default:
		temperature_data->t = (temperature_data->t * 5 + ths_read_data(4)) / 6;
		//pr_info("%d, %s\n", temperature_data->t, __FUNCTION__);
		
		delay = (temperature_data->t > 80 ? 0 : (80 - temperature_data->t) * 50) + 10;
		delay = msecs_to_jiffies(delay);
		temperature_data->phase = 0;
		break;
	}

	led_set_brightness(led_cdev, brightness);
	mod_timer(&temperature_data->timer, jiffies + delay);
}

static void temperature_trig_activate(struct led_classdev *led_cdev)
{
	struct temperature_trig_data *temperature_data;

	temperature_data = kzalloc(sizeof(*temperature_data), GFP_KERNEL);
	if (!temperature_data)
		return;

	led_cdev->trigger_data = temperature_data;
	setup_timer(&temperature_data->timer,
		    led_temperature_function, (unsigned long) led_cdev);
	temperature_data->phase = 0;
	led_temperature_function(temperature_data->timer.data);
}

static void temperature_trig_deactivate(struct led_classdev *led_cdev)
{
	struct temperature_trig_data *temperature_data = led_cdev->trigger_data;

	if (temperature_data) {
		del_timer_sync(&temperature_data->timer);
		kfree(temperature_data);
	}
}

static struct led_trigger temperature_led_trigger = {
	.name     = "cpu_temp",
	.activate = temperature_trig_activate,
	.deactivate = temperature_trig_deactivate,
};

static int __init temperature_trig_init(void)
{
	return led_trigger_register(&temperature_led_trigger);
}

static void __exit temperature_trig_exit(void)
{
	led_trigger_unregister(&temperature_led_trigger);
}

module_init(temperature_trig_init);
module_exit(temperature_trig_exit);

MODULE_AUTHOR("pangliang<418094911@qq.com>");
MODULE_DESCRIPTION("Cpu Temperature LED trigger");
MODULE_LICENSE("GPL");
