#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME MyRollBall)

project(${APP_NAME})

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

if(NOT DEFINED BUILD_ENGINE_DONE) # to test MyRollBall into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
Classes/AppDelegate.cpp
Classes/Level.cpp
Classes/Player.cpp
Classes/Infos/GameInfo.cpp
Classes/LevelComponents/CoinCreator.cpp
Classes/LevelComponents/CoinLevelComponent.cpp
Classes/LevelComponents/Crossbow.cpp
Classes/LevelComponents/KeyLevelComponent.cpp
Classes/LevelComponents/LevelComponentTemplate.cpp
Classes/Managers/AccelerationHandler.cpp
Classes/Managers/ContactHandler.cpp
Classes/Managers/HandlerManager.cpp
Classes/Managers/KeyboardHandler.cpp
Classes/Scenes/ActionCreator.cpp
Classes/Scenes/ComingSoonScene.cpp
Classes/Scenes/InGameScene.cpp
Classes/Scenes/IntroLevelScene.cpp
Classes/Scenes/MainMenuScene.cpp
Classes/Scenes/MyCustomGUI.inl
Classes/Scenes/SplashScreenScene.cpp
Classes/Scenes/TouchSwallower.inl
Classes/Scenes/Dialogs/CreditsDialog.cpp
Classes/Scenes/Dialogs/ExitGameDialog.cpp
Classes/Scenes/Dialogs/GameOverDialog.cpp
Classes/Scenes/Dialogs/MenuDialog.cpp
Classes/Scenes/Dialogs/MyDialog.cpp
Classes/Scenes/Dialogs/OptionsDialog.cpp
Classes/Scenes/Dialogs/PausingGameDialog.cpp
Classes/Scenes/Dialogs/VictoryDialog.cpp
Classes/Scenes/Layers/ChoosingLevelLayer.cpp
Classes/Scenes/Layers/MyLayer.cpp
Classes/Scenes/Layers/ScreenInfoLayer.cpp
Classes/Scenes/MyComponents/ActionHolder.inl
Classes/Scenes/MyComponents/MyButton.cpp
Classes/Scenes/MyComponents/MyImageView.cpp
Classes/Scenes/MyComponents/MyText.cpp
Classes/ScreenLog/ScopeLock.cpp
Classes/ScreenLog/ScopeLog.cpp
Classes/ScreenLog/ScreenLog.cpp
Classes/ScreenLog/ScreenLogMessage.cpp
Classes/Utils/StaticMethods.cpp
Classes/Utils/TMXUtil.cpp
)

list(APPEND GAME_HEADER
Classes/AppDelegate.h
Classes/Bitmask.h
Classes/IPlayer.h
Classes/Level.h
Classes/Player.h
Classes/Infos/GameInfo.h
Classes/Infos/PlayerInfo.h
Classes/Infos/ScreenLogInfo.h
Classes/LevelComponents/CoinCreator.h
Classes/LevelComponents/CoinLevelComponent.h
Classes/LevelComponents/Crossbow.h
Classes/LevelComponents/KeyLevelComponent.h
Classes/LevelComponents/LevelComponentTemplate.h
Classes/Managers/AccelerationHandler.h
Classes/Managers/ContactHandler.h
Classes/Managers/HandlerManager.h
Classes/Managers/KeyboardHandler.h
Classes/Scenes/ActionCreator.h
Classes/Scenes/ComingSoonScene.h
Classes/Scenes/IInGameScene.h
Classes/Scenes/InGameScene.h
Classes/Scenes/IntroLevelScene.h
Classes/Scenes/MainMenuScene.h
Classes/Scenes/MyCustomGUI.h
Classes/Scenes/SplashScreenScene.h
Classes/Scenes/TouchSwallower.h
Classes/Scenes/Dialogs/CreditsDialog.h
Classes/Scenes/Dialogs/ExitGameDialog.h
Classes/Scenes/Dialogs/GameOverDialog.h
Classes/Scenes/Dialogs/MenuDialog.h
Classes/Scenes/Dialogs/MyDialog.h
Classes/Scenes/Dialogs/OptionsDialog.h
Classes/Scenes/Dialogs/PausingGameDialog.h
Classes/Scenes/Dialogs/VictoryDialog.h
Classes/Scenes/Layers/ChoosingLevelLayer.h
Classes/Scenes/Layers/MyLayer.h
Classes/Scenes/Layers/ScreenInfoLayer.h
Classes/Scenes/MyComponents/ActionHolder.h
Classes/Scenes/MyComponents/MyButton.h
Classes/Scenes/MyComponents/MyImageView.h
Classes/Scenes/MyComponents/MyText.h
Classes/ScreenLog/ScopeLock.h
Classes/ScreenLog/ScopeLog.h
Classes/ScreenLog/ScreenLog.h
Classes/ScreenLog/ScreenLogMessage.h
Classes/Utils/Cocos2dCreator.h
Classes/Utils/StaticMethods.h
Classes/Utils/TMXUtil.h
)

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
    target_compile_features(${APP_NAME} PUBLIC cxx_std_20)
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)


# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")

    if(MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
