# ozw-admin
This is a Gui for OpenZWave, intended to replace the open-zwave-control-panel eventually.

Latest Installer Images/AppImages can be found here:
https://github.com/OpenZWave/ozw-admin/releases

Work in Progress. Don't expect much right now :)

If you are using the ozwdaemon docker image, you need to allow remote network access to the Docker Image:

```docker run -it --security-opt seccomp=unconfined --device=/dev/ttyUSB0 -v /tmp/ozw:/opt/ozw/config -e MQTT_SERVER="10.100.200.102" -e USB_PATH=/dev/ttyUSB0 --network host openzwave/ozwdaemon:latest```

Then in the OZWAdmin gui, when opening a "Remote Connection" specify the IP address of the Host the Docker Image is running on. 
