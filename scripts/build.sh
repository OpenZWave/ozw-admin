#!/bin/bash
#Build File for Docker Image. 
#Run docker in this directory with: docker run -it --rm --device /dev/fuse --privileged -v`pwd`:/opt/buildfiles -e DOCKERUSERID=`id -u` -e DOCKERGROUPID=`id -g` <docker image>
cd /opt
git clone https://github.com/OpenZWave/open-zwave.git && cd open-zwave && make -j4 && make install
cd /opt
git clone https://github.com/OpenZWave/qt-openzwave.git && cd qt-openzwave && /opt/qt512/bin/qmake -r CONFIG+=release && make -j4 && make install
cd /opt
git clone https://github.com/OpenZWave/ozw-admin.git && cd ozw-admin && /opt/qt512/bin/qmake -r CONFIG+=release && make -j4 && make install
cd /opt

mkdir -p /opt/AppDir/usr/bin
cp /opt/ozw-admin/ozwadmin /opt/AppDir/usr/bin/
mkdir -p /opt/AppDir/usr/share/OpenZWave/
cp /opt/qt-openzwave/qt-openzwavedatabase/*.rcc /opt/AppDir/usr/share/OpenZWave/
mkdir -p /opt/AppDir/usr/plugins/platforms/
cp /opt/qt512/plugins/platforms/libqvnc.so /opt/AppDir/usr/plugins/platforms/
mkdir -p /opt/AppDir/usr/share/metainfo/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export QMAKE=/opt/qt512/bin/qmake
export VERSION=$(cat /opt/ozw-admin/scripts/version)

cd /opt && ./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage -d ozw-admin/scripts/ozwadmin.desktop -i ozw-admin/scripts/ozwadmin.png

cp /opt/OZWAdmin-*.AppImage /opt/buildfiles/
cp /opt/ozw-admin/scripts/version /opt/buildfiles/

chown $DOCKERUSERID:$DOCKERGROUPID /opt/buildfiles/OZWAdmin-*.AppImage
chown $DOCKERUSERID:$DOCKERGROUPID /opt/buildfiles/version