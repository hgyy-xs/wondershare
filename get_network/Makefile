#
# hua.shao@mediatek.com
#
# MTK Property Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=get_network
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/get_network
  SECTION:=MTK Properties
  CATEGORY:=MTK Properties
  TITLE:=get network status
  SUBMENU:=Applications
  DEPENDS:=+libc
endef

define Package/get_network/description
  An  test program to get network mask and ip .
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += \
	-I$(LINUX_DIR)/arch/$(LINUX_KARCH)/include \
	-I$(LINUX_DIR)/drivers/net/rt_rdm

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS)"


define Build/Configure
endef

define Package/get_network/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) /var/tmp/mkinitramfs_aNetnq/lib/x86_64-linux-gnu/libc.so.6 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/get_network $(1)/usr/bin
endef


$(eval $(call BuildPackage,get_network))

