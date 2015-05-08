LOCAL_PATH := $(call my-dir)
LOCAL_PHOTON_ROOT := $(LOCAL_PATH)/../Photon-AndroidNDK_SDK

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES_JNI_PREFIXED := \
    $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp) \
    $(wildcard $(LOCAL_PATH)/../../Classes/**/*.c*)

LOCAL_SRC_FILES_JNI_UNPREFIXED := $(subst jni/,, $(LOCAL_SRC_FILES_JNI_PREFIXED))

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(LOCAL_SRC_FILES_JNI_UNPREFIXED)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Photon-AndroidNDK_SDK

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_CFLAGS := -DEG_DEBUGGER -D__STDINT_LIMITS -D_EG_ANDROID_PLATFORM

LOCAL_STATIC_LIBRARIES := common-cpp-static-prebuilt
LOCAL_STATIC_LIBRARIES += photon-cpp-static-prebuilt
LOCAL_STATIC_LIBRARIES += loadbalancing-cpp-static-prebuilt

LOCAL_LDLIBS := -llog

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)

$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-cpp/lib)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Common-cpp)
$(call import-module,common-cpp-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-cpp/lib)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/Photon-cpp)
$(call import-module,photon-cpp-prebuilt)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/LoadBalancing-cpp/lib)
$(call import-add-path-optional, $(LOCAL_PHOTON_ROOT)/LoadBalancing-cpp)
$(call import-module,loadbalancing-cpp-prebuilt)%