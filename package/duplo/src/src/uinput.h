/* by xerpi (c) 2013 */

#ifndef _UINPUT_H_
#define _UINPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdint.h>
#include <time.h>

int uinput_init();
int uinput_exit();
int send_event(uint16_t type, uint16_t code, int32_t value);
int send_report();
int send_rel_mouse(int32_t x, int32_t y);
int send_abs_mouse(int32_t x, int32_t y);

#define release    0
#define press      1
#define autorepeat 2

#define send_key_macro(x) \
    static inline int send_key_##x(uint16_t key) {  \
        return send_event(EV_KEY, key, x);   \
    }
    
#define uinput_return_not_initiated() \
	if(!ui_initiated) {    \
		return -2;             \
	}

send_key_macro(press);
send_key_macro(release);
send_key_macro(autorepeat);

#endif
