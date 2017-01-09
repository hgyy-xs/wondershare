#
# hua.shao@mediatek.com
#
# MTK Property Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=test_device
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/test_device
  SECTION:=MTK Properties
  CATEGORY:=MTK Properties
  TITLE:=Smartconfig device test
  SUBMENU:=Applications
  DEPENDS:=+lua
endef

define Package/test_device/description
  An  device program to test Smartconfig.
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

define Package/test_device/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_device $(1)/usr/bin
endef


$(eval $(call BuildPackage,test_device))

