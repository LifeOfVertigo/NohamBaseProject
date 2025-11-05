@echo off
echo ========================================
echo Building Noham Base Project C++
echo ========================================
echo.

"C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\Build.bat" Noham_Base_Proj_CppEditor Win64 Development "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject\Noham_Base_Proj_Cpp\Noham_Base_Proj_Cpp.uproject" -WaitMutex

if errorlevel 1 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    pause
    exit /b 1
) else (
    echo.
    echo ========================================
    echo BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo You can now open the project in Unreal Engine.
    echo The MainMenuWidget class is ready to use!
    pause
)
