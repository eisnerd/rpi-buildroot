/* by xerpi (c) 2013 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <linux/uinput.h>
#include <lirc_client.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wiringPi.h>
#include "uinput.h"
#include "nunchuk.h"

struct nunchuk nun, last_nun;

void nunchuk2pi_init();
void nunchuk2pi_exit();
void catch_signal(int signal);
void nunchuk_print_data(struct nunchuk* n);
void read_callback(int read_success);
void lirc_send(const char *key);
const char *brake = "BB";

int main(int argc, char *argv[])
{
  wiringPiSetupGpio();
	nunchuk2pi_init();
	nunchuk_init_nunchuk();

	while(1) {
		usleep(1000 * 1000);
	}
	
	nunchuk2pi_exit();
	return 0;
}

void read_callback(int read_success)
{
	if(read_success) {
		//nunchuk_print_data(&nun);
		int x = nun.X-128;
		int y = nun.Y-128;
		if(abs(x) < 15) x = 0;
		if(abs(y) < 15) y = 0;
		//x = (int)((float)x/7.5f);
		//y = (int)((float)y/7.5f);
		send_rel_mouse(x, -y);
		if(nun.C && !last_nun.C) {
			send_key_press(BTN_LEFT);
		}
		if(!nun.C && last_nun.C) {
			send_key_release(BTN_LEFT);
		}
		if(nun.Z && !last_nun.Z) {
			send_key_press(BTN_RIGHT);
		}
		if(!nun.Z && last_nun.Z) {
			send_key_release(BTN_RIGHT);
		}
		send_report();
		last_nun = nun;

    if (y != 0 || x != 0) {
      double r = x*x + y*y, p = atan2(y, x) * 4 / M_PI - 0.5;
      const char *d = "RF";
      if (p > -4) d = "XR";
      if (p > -3) d = "RR";
      if (p > -2) d = "RX";
      if (p > -1) d = "FR";
      if (p >  0) d = "FX";
      if (p >  1) d = "FF";
      if (p >  2) d = "XF";
      if (p >  3) d = "RF";
      if (r > 6400) {
        lirc_send(d);
        return;
      }
    }
    lirc_send(brake);
	}
  else
    lirc_send(NULL);
}

int lirc = -1;
unsigned int last = 0;
const char *last_key;

void lirc_send(const char *key) {
  if (key != NULL) {
    last_key = key;
    if (key != brake)
      last = millis();
  }
  if (last + 2000 < millis())
    return;
  printf("%s\n", last_key);
  if (lirc < 0)
    lirc = lirc_get_local_socket("/var/run/lirc/lircd", 0);
  if (lirc < 0)
    printf("Could not init lirc.\n");
  else if (lirc_send_one(lirc, "duplo", last_key) == -1) {
    close(lirc);
    lirc = -1;
  }
}

void nunchuk_print_data(struct nunchuk* n)
{
 	printf("joyX: %d  joyY: %d  Z: %d  C: %d\n", n->X, n->Y, n->Z, n->C);
 	printf("aX: %d  aY: %d  aZ: %d\n", n->aX, n->aY, n->aZ);
}

void nunchuk2pi_init()
{
	printf("nunchuk2pi init:\n");
	
	signal(SIGTERM, catch_signal);
	signal(SIGKILL, catch_signal);
	signal(SIGINT, catch_signal);
	
	if(uinput_init() < 0) {
		printf("Could not open uinput.\n");
		//exit(EXIT_FAILURE);
	}
	if(nunchuk_init(&nun) < 0) {
		printf("Could not init nunchuk.\n");
		exit(EXIT_FAILURE);
	}
	nunchuk_set_read_callback(read_callback);
}

void nunchuk2pi_exit()
{
	uinput_exit();
	nunchuk_exit();
	printf("\nn2pi exit\n");	
}

void catch_signal(int signal)
{
	printf("Signal %i caught.\n", signal);
	nunchuk2pi_exit();
	exit(-1);
}

