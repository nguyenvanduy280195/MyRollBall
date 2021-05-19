MyRollBall
==========

- [MyRollBall](#myrollball)
  - [1. Create solution in proj.win32 folder](#1-create-solution-in-projwin32-folder)
  - [2. Add a new class to CMakeLists.txt](#2-add-a-new-class-to-cmakeliststxt)
  - [3. Replace GAME_SOURCE in `CMakeLists.txt` by content of `fileCpp.txt`](#3-replace-game_source-in-cmakeliststxt-by-content-of-filecpptxt)
  - [4. Replace GAME_HEADER in `CMakeLists.txt` by content of `fileh.txt`](#4-replace-game_header-in-cmakeliststxt-by-content-of-filehtxt)

These are steps to run the project with cmake

## 1. Create solution in proj.win32 folder

In `proj.win32`
```
cmake .. -G "Visual Studio 16 2019" -AWin32
```

Enter `MyRollBall.sln`, Remove `ALL_BUILD` Project. Run it

## 2. Add a new class to CMakeLists.txt

Run `tools/list-cpp-h-files.bat`

In `fileCpp.txt`
```
AppDelegate.cpp
HelloWorldScene.cpp
```

In `fileh.txt`
```
AppDelegate.h
HelloWorldScene.h
```

## 3. Replace GAME_SOURCE in `CMakeLists.txt` by content of `fileCpp.txt`

  ```
    list(APPEND GAME_SOURCE
         Classes/*.cpp
         )
  ```
  to
  ```
  list(APPEND GAME_SOURCE
       Classes/AppDelegate.cpp
       Classes/HelloWorldScene.cpp
       )
  ```

## 4. Replace GAME_HEADER in `CMakeLists.txt` by content of `fileh.txt`

```
    list(APPEND GAME_HEADER
         Classes/*.h
         )
  ```
  to
  ```
  list(APPEND GAME_HEADER
       Classes/AppDelegate.h
       Classes/HelloWorldScene.h
       )
  ```