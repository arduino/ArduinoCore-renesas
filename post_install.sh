#!/usr/bin/env bash

arduino_renesas_core_rules () {
    echo ""
    echo "# Renesas based Arduino Santiago/Portenta H33 bootloader mode UDEV rules"
    echo ""
cat <<EOF
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", MODE:="0666"
EOF
}

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

arduino_renesas_core_rules > /etc/udev/rules.d/60-arduino-renesas.rules

# reload udev rules
echo "Reload rules..."
udevadm trigger
udevadm control --reload-rules
