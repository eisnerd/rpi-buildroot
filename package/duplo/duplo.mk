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
DUPLO_DEPENDENCIES = wiringpi i2c-tools host-pkgconf lirc-tools

define DUPLO_BOOTSTRAP
	(cd "$(@D)"; sh autogen.sh)
endef

DUPLO_PRE_CONFIGURE_HOOKS += DUPLO_BOOTSTRAP
DUPLO_INSTALL_STAGING = YES
DUPLO_INSTALL_TARGET = YES
DUPLO_AUTORECONF = YES

DUPLO_CONF_OPTS = \
	CFLAGS="-I$(STAGING_DIR)/usr/include/wiringPi"

ifeq ($(BR2_PACKAGE_DUPLO),y)
TARGETS+=duplo
endif
$(eval $(autotools-package))
