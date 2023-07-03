`Arduino_CAN`
=============
### How-to-`SocketCAN`
* Connect your [SocketCAN](https://en.wikipedia.org/wiki/SocketCAN) capable CAN/USB dongle to the PC and configure it.
```bash
sudo ip link set can0 type can bitrate 250000
sudo ip link set up can0
```
* Install can-utils for cansend/candump:
```bash
sudo apt-get install can-utils
```
* Transmit CAN frames via [`cansend`](https://manpages.ubuntu.com/manpages/lunar/man1/cansend.1.html):
```bash
cansend can0 1F334455#1122334455667788
```
* Display received CAN frames via [`candump`](https://manpages.ubuntu.com/manpages/lunar/man1/candump.1.html):
```bash
candump can0
```
