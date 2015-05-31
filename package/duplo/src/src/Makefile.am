bin_PROGRAMS = duplo

duplo_SOURCES = nunchuk.c nunchuk.h uinput.c uinput.h log.c log.h nunchuk2pi.c
duplo_CFLAGS = @LIRC_CFLAGS@
duplo_LDADD = @LIRC_LIBS@ -llirc_client -lwiringPi -lm -lpthread
