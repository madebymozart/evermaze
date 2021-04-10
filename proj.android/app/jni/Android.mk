LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := evermaze

LOCAL_MODULE_FILENAME := libevermaze

LOCAL_SRC_FILES := $(LOCAL_PATH)/android/main.cxx \
$(LOCAL_PATH)/../../../Classes/app/app_delegate.cxx \
$(LOCAL_PATH)/../../../Classes/engines/fmod/fmod_engine.cxx \
$(LOCAL_PATH)/../../../Classes/engines/firebase/firebase_engine.cxx \
$(LOCAL_PATH)/../../../Classes/engines/language/language_engine.cxx \
$(LOCAL_PATH)/../../../Classes/engines/tmx/tmx_engine.cxx \
$(LOCAL_PATH)/../../../Classes/entities/llumas/ivory/ivory_lluma.cxx \
$(LOCAL_PATH)/../../../Classes/entities/llumas/base_lluma.cxx \
$(LOCAL_PATH)/../../../Classes/entities/llurkers/ximois/ximois_llurker.cxx \
$(LOCAL_PATH)/../../../Classes/entities/llurkers/base_llurker.cxx \
$(LOCAL_PATH)/../../../Classes/entities/base_entity.cxx \
$(LOCAL_PATH)/../../../Classes/gestures/tap/tap_gesture.cxx \
$(LOCAL_PATH)/../../../Classes/gestures/swipe/swipe_gesture.cxx \
$(LOCAL_PATH)/../../../Classes/gestures/base_gesture.cxx \
$(LOCAL_PATH)/../../../Classes/layers/background/background_layer.cxx \
$(LOCAL_PATH)/../../../Classes/managers/menu/menu_manager.cxx \
$(LOCAL_PATH)/../../../Classes/managers/sprite/sprite_manager.cxx \
$(LOCAL_PATH)/../../../Classes/managers/text/text_manager.cxx \
$(LOCAL_PATH)/../../../Classes/managers/base_manager.cxx \
$(LOCAL_PATH)/../../../Classes/objects/tmx/tmx_object.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/game/controls/controls_game_scene.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/game/base_game_scene.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/intro/intro_scene.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/levels/levels_scene.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/modes/modes_scene.cxx \
$(LOCAL_PATH)/../../../Classes/scenes/base_scene.cxx \
$(LOCAL_PATH)/../../../Classes/sqleet/sqleet.c \
$(LOCAL_PATH)/../../../Classes/utils/action/action_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/archive/archive_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/data/data_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/particle/particle_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/path/path_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/scene/scene_utils.cxx \
$(LOCAL_PATH)/../../../Classes/utils/thread/thread_utils.cxx

LOCAL_CFLAGS := -DSQLITE_HAS_CODEC

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_SHORT_COMMANDS=true

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += firebase_admob
LOCAL_STATIC_LIBRARIES += firebase_analytics
LOCAL_STATIC_LIBRARIES += firebase_auth
LOCAL_STATIC_LIBRARIES += firebase_database
LOCAL_STATIC_LIBRARIES += firebase_remote_config
LOCAL_STATIC_LIBRARIES += firebase_storage
LOCAL_STATIC_LIBRARIES += firebase_app

ifeq ($(NDK_DEBUG),1)
LOCAL_SHARED_LIBRARIES := fmodL
LOCAL_SHARED_LIBRARIES += fmodstudioL
else
LOCAL_SHARED_LIBRARIES := fmod
LOCAL_SHARED_LIBRARIES += fmodstudio
endif

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../xLibraries)

$(call import-module, cocos)
$(call import-module, fmod/android/prebuilt)
$(call import-module, firebase/prebuilt/android)