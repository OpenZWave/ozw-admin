#!/bin/bash
if [ $# -lt 3 ]; then
	echo "Please Provide the path to the App Bundle and OZW Config Directory and QT Directories"
	exit 1
fi
if [ ! -f $1/Contents/Frameworks/qt-openzwave.framework/qt-openzwave ]; then
	echo "$1/Contents/Frameworks/qt-openzwave.framework/qt-openzwave doens't exist"
	exit 1
fi
if [ ! -f $2/manufacturer_specific.xml ]; then
	echo "$2/manufacturer_specific.xml doesn't exist"
	exit 1
fi
if [ ! -f $3/bin/macdeployqt ]; then
	echo "$3/bin/macdeployqt  does't exist"
	exit 1
fi
PATH=`otool -L $1/Contents/Frameworks/qt-openzwave.framework/qt-openzwave  | grep libopenzwave | awk '{print $1}'`
/usr/bin/install_name_tool -change $PATH "@rpath/${PATH##*/}" $1/Contents/Frameworks/qt-openzwave.framework/qt-openzwave 
/bin/cp -r $2 $1/Contents/Resources/config/
echo "To Finish Please Run '$3/bin/macdeployqt $1 -verbose=2 -always-overwrite'"
