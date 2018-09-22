LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_app
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_app.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_admob
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_admob.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_analytics
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_analytics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_auth
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_auth.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_database
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_database.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_remote_config
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_remote_config.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := firebase_storage
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libfirebase_storage.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)