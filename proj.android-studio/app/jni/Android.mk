LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/DataManager.cpp \
                   ../../../Classes/BottomMenu.cpp \
                   ../../../Classes/Button_Highlight.cpp \
                   ../../../Classes/Cell_HighScore.cpp \
                   ../../../Classes/Cell_Setting.cpp \
                   ../../../Classes/Cell_Statistics.cpp \
                   ../../../Classes/HighScoreManager.cpp \
                   ../../../Classes/HighScoreModel.cpp \
                   ../../../Classes/Layer_HighScore.cpp \
                   ../../../Classes/Layer_Setting.cpp \
                   ../../../Classes/Layer_Statistics.cpp\
                   ../../../Classes/Audio.cpp \
                   ../../../Classes/KLUtils.cpp \
                   ../../../Classes/md5.cpp \
                   ../../../Classes/BlankCard.cpp \
                   ../../../Classes/Button_Daily.cpp \
                   ../../../Classes/DayModel.cpp \
                   ../../../Classes/Global.cpp \
                   ../../../Classes/HLAdManagerWrapper-android.cpp \
                   ../../../Classes/HLAnalsytWrapper-android.cpp \
                   ../../../Classes/HttpManager.cpp \
                   ../../../Classes/iOSWrapper-android.cpp \
                   ../../../Classes/KLColor.cpp \
                   ../../../Classes/KLPopupLayer.cpp \
                   ../../../Classes/KTDate.cpp \
                   ../../../Classes/KTFactory.cpp \
                   ../../../Classes/KTLayer.cpp \
                   ../../../Classes/KTParticleManager.cpp \
                   ../../../Classes/KTPauseLayer.cpp \
                   ../../../Classes/KTUtils.cpp \
                   ../../../Classes/Layer_Daily.cpp \
                   ../../../Classes/Layer_NavigationBar.cpp \
                   ../../../Classes/Localization.cpp \
                   ../../../Classes/MonthModel.cpp \
                   ../../../Classes/SolitaireKlondikeScene.cpp \
                   ../../../Classes/SolitaireKlondikeViewModel.cpp \
                   ../../../Classes/SpritePool.cpp \
                   ../../../Classes/TestScene.cpp \
                   ../../../Classes/Trump.cpp \
                   ../../../Classes/TrumpData.cpp \
                   ../../../Classes/TrumpModel.cpp \
                   ../../../Classes/Layer_Leaderboard.cpp \
                   ../../../Classes/VisibleRect.cpp \
                   ../../../Classes/AdmobWrapper.cpp \
                   ../../../Classes/FBWrapper.cpp \
                   ../../../Classes/HLAnalystAndroid.cpp \
                   ../../../Classes/UnityADWrapper.cpp \
                   ../../../Classes/HLOnlineConfigAndroid.cpp \
                   ../../../Classes/VungleWrapper.cpp \
                   ../../../Classes/HLNotificationAndroid.cpp \
                   ../../../Classes/Item_Setting.cpp \
                   ../../../Classes/HLInterfaceAndroid.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
