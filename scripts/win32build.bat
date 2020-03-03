@echo off
cd ..\open-zwave
msbuild /p:Configuration=ReleaseDLL /p:Platform=Win32 cpp\build\windows\vs2010\OpenZWave.sln
msbuild /p:Configuration=DebugDLL /p:Platform=Win32 cpp\build\windows\vs2010\OpenZWave.sln
cd ..\qt-openzwave
qmake -r
#msbuild /p:Configuration=Release /p:Platform=Win32 qt-openzwave.sln
nmake
cd ..\ozw-admin
qmake -r 
#msbuild /p:Configuration=Release /p:Platform=Win32 ozwadmin.sln
nmake
rmdir /S /Q package
mkdir package
cd package
copy ..\..\open-zwave\cpp\build\windows\vs2010\ReleaseDLL\*.dll .
copy ..\..\qt-openzwave\qt-openzwave\release\*.dll .
copy ..\..\qt-openzwave\qt-openzwavedatabase\release\*.dll .
copy ..\..\qt-openzwave\qt-openzwavedatabase\*.rcc .
copy ..\ozwadmin-main\*.exe .
windeployqt qt-openzwave1.dll --verbose 2 -dir .
windeployqt qt-openzwavedatabase1.dll --verbose 2 dir .
windeployqt ozwadmin.exe --verbose 2 -dir .
"c:\Program Files (x86)\NSIS\makensis.exe" /NOCD /V4 ..\scripts\package.nsis
copy "OpenZwave Admin-*-installer.exe" ..
cd ..

