# USB communication with the DMX chain

debian pakage:

libusb-1.0-0-dev/stable 2:1.0.22-2 armhf
  userspace USB programming library development files

Compile lib parameter: -lusb-1.0

doc:
[http://libusb.sourceforge.net/api-1.0/group__libusb__syncio.html](sync doc)
[http://libusb.sourceforge.net/api-1.0/group__libusb__asyncio.html](async doc)

# Wifi hotspot setup
[https://www.raspberrypi.org/documentation/configuration/wireless/access-point-routed.md](rpi doc wifi)
ignore:
**Enable routing and IP masquerading**
to
**Configure the DHCP and DNS services for the wireless network**
since we don't need to forward packets on a existing router.
