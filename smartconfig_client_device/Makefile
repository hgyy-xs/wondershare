#
# hua.shao@mediatek.com
#
# MTK Property Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=test_client
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/test_client
  SECTION:=MTK Properties
  CATEGORY:=MTK Properties
  TITLE:=Read/Write registers from user space
  SUBMENU:=Applications
  DEPENDS:=+lua
endef

define Package/test_client/description
  An program to read/write ralink register.
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

define Package/test_client/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_client $(1)/usr/bin
endef


$(eval $(call BuildPackage,test_client))

