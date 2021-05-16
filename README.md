CMake
=======

These are steps to run the project with cmake

## 1. Run `tools/list-cpp-h-files.bat`


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

## 2. Replace GAME_SOURCE in `CMakeLists.txt` by content of `fileCpp.txt`
----------------------------------------------------------

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

## 3. Replace GAME_HEADER in `CMakeLists.txt` by content of `fileh.txt`

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

## 4. Run cmake

In proj.win32, run:
```
cmake .
```