# Common-cpp

LOCAL_PATH := $(call my-dir)

lib_suffix := ${APP_OPTIM}_android_${APP_ABI}

lib_common_cpp_static_name := common-cpp-static_${lib_suffix}

include $(CLEAR_VARS)
LOCAL_MODULE := common-cpp-static-prebuilt
LOCAL_SRC_FILES := libcommon-cpp-static_${APP_OPTIM}_android_$(TARGET_ARCH_ABI).a
LOCAL_EXPORT_C_INCLUDES := ../../../..
include $(PREBUILT_STATIC_LIBRARY)
