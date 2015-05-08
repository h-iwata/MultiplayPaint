# Chat-cpp

LOCAL_PATH := $(call my-dir)

all_static_libraries = common-cpp-static-prebuilt \
                       photon-cpp-static-prebuilt

include $(CLEAR_VARS)
LOCAL_MODULE            := chat-cpp-static-prebuilt
LOCAL_SRC_FILES         := libchat-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI).a
LOCAL_STATIC_LIBRARIES  := $(all_static_libraries)
include $(PREBUILT_STATIC_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../Common-cpp/lib)
$(call import-add-path, $(LOCAL_PATH)/../../../Photon-cpp/lib)

$(call import-module,common-cpp-prebuilt)
$(call import-module,photon-cpp-prebuilt)


