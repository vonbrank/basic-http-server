@echo off

set DCMAKE_TOOLCHAIN_FILE=.\generators\conan_toolchain.cmake

if "%1"=="start" (
    cd .\build\Debug\
    cmake ..\..\ -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%DCMAKE_TOOLCHAIN_FILE% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build .
    .\basic_http_server.exe
) else if "%1"=="build" (
    cd .\build\Release\
    cmake ..\..\ -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%DCMAKE_TOOLCHAIN_FILE% -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build .
) else if "%1"=="install-debug" (
    conan install . --build=missing --profile=mingw64 --settings=build_type=Debug
) else if "%1"=="install-release" (
    conan install . --build=missing --profile=mingw64
) else if "%1"=="clean" (
    call :DeleteFileIfExists "CMakeUserPresets.json"
    call :DeleteFolderIfExists "build"
) else (
    echo Invalid command.
)
goto :eof

:DeleteFolderIfExists
IF EXIST %1 (
    rd /s /q %1
)
goto :eof

:DeleteFileIfExists
IF EXIST %1 (
    del %1
)
goto :eof