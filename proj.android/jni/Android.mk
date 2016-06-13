LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := Butcher_shared

LOCAL_MODULE_FILENAME := libButcher

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/butcher.cpp \
../../Classes/hud_layer.cpp \
../../Classes/actors/actor.cpp \
../../Classes/actors/character.cpp \
../../Classes/actors/player.cpp \
../../Classes/actors/monster.cpp \
../../Classes/data/actors_database.cpp \
../../Classes/dungeon/dungeon_layer.cpp \
../../Classes/dungeon/dungeon_state.cpp \
../../Classes/actors/actions/move_action.cpp \
../../Classes/actors/actions/attack_action.cpp \
../../Classes/actors/actions/die_action.cpp \
../../Classes/utils/utils.cpp \
../../Classes/utils/target.cpp \
../../Classes/actors/ai/ai.cpp \
../../Classes/actors/ai/fsm/fsm.cpp \
../../Classes/actors/ai/fsm/states/fsm_state.cpp \
../../Classes/actors/ai/fsm/states/idle_state.cpp \
../../Classes/actors/ai/fsm/states/wandering_state.cpp \
../../Classes/actors/ai/fsm/states/move_to_target_state.cpp \
../../Classes/actors/ai/fsm/states/melee_attack_state.cpp \
../../Classes/fov/permissive-fov.cc \
../../Classes/dungeon/generators/cave_grid_generator.cpp \
../../Classes/dungeon/generators/tmx_builder.cpp \
../../Classes/dungeon/generators/DungeonMaker.cpp \
../../Classes/dungeon/generators/dungeon_maker_generator.cpp \
../../Classes/utils/profiler.cpp \
../../Classes/loading_scene.cpp  \
../../Classes/actors/object.cpp  \
../../Classes/actors/instances/stairs_down.cpp  \
../../Classes/actors/instances/stairs_up.cpp  \
../../Classes/dungeon/level_manager.cpp  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/dungeon

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
