################################################################################
#
# duplo
#
################################################################################

DUPLO_VERSION = 1.0.0
#DUPLO_SOURCE = duplo-$(DUPLO_VERSION).tar.bz2
DUPLO_SITE = $(TOPDIR)/package/duplo/src/
DUPLO_SITE_METHOD = local
DUPLO_LICENSE = GPLv2+
DUPLO_LICENSE_FILES = COPYING
DUPLO_DEPENDENCIES = wiringpi

DUPLO_INSTALL_TARGET = YES
DUPLO_AUTORECONF = YES

ifeq ($(BR2_PACKAGE_DUPLO),y)
TARGETS+=duplo
endif
$(eval $(autotools-package))
