# Photon-cpp

LOCAL_PATH := $(call my-dir)

all_static_libraries = common-cpp-static-prebuilt

include $(CLEAR_VARS)
LOCAL_MODULE            := photon-cpp-static-prebuilt
LOCAL_SRC_FILES         := libphoton-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI).a
LOCAL_STATIC_LIBRARIES  := $(all_static_libraries)
include $(PREBUILT_STATIC_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../Common-cpp/lib)

$(call import-module,common-cpp-prebuilt)
