#!/bin/bash
#Build File for Docker Image. 
#Run docker in this directory with: docker run -it --rm --device /dev/fuse --privileged -v`pwd`:/opt/buildfiles -e DOCKERUSERID=`id -u` -e DOCKERGROUPID=`id -g` <docker image>
cd /opt
git clone https://github.com/OpenZWave/open-zwave.git && cd open-zwave && make -j4 && make install
cd /opt
git clone https://github.com/OpenZWave/qt-openzwave.git && cd qt-openzwave && /opt/qt512/bin/qmake -r && make -j4 && make install
cd /opt
git clone https://github.com/OpenZWave/ozw-admin.git && cd ozw-admin && /opt/qt512/bin/qmake -r && make -j4 && make install
cd /opt
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export QMAKE=/opt/qt512/bin/qmake
export VERSION=0.1
cd ozw-admin && ../linuxdeploy-x86_64.AppImage --appdir AppDir -e ozwadmin --plugin qt --output appimage -d scripts/ozwadmin.desktop -i scripts/ozwadmin.png
cp /opt/ozw-admin/OZWAdmin-*.AppImage /opt/buildfiles/
chown $DOCKERUSERID:$DOCKERGROUPID /opt/buildfiles/OZWAdmin-*.AppImage
