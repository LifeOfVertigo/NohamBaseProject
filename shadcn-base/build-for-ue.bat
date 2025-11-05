@echo off
echo ========================================
echo Building React App for Unreal Engine
echo ========================================

echo.
echo Step 1: Building production bundle...
call npm run build

if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Step 2: Copying to NEON Content folder...
xcopy /E /Y /I "dist\*" "..\Noham_Base_Proj_Cpp\Content\UI\NEON\MainMenu\dist\"

echo.
echo ========================================
echo Build Complete!
echo ========================================
echo.
echo The bundled app is now at:
echo   Noham_Base_Proj_Cpp\Content\UI\NEON\MainMenu\dist\
echo.
echo In Unreal Engine, set your NEON widget URL to:
echo   /Content/UI/NEON/MainMenu/dist/index.html
echo.
pause
