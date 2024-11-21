#!/usr/bin/env bash

arduino_renesas_core_rules () {
    echo ""
    echo "# Renesas based Arduino Santiago/Portenta H33 bootloader mode UDEV rules"
    echo ""
cat <<EOF
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", MODE:="0666"
EOF
}

OS="$(uname -s)"
case "$OS" in
Linux*)
  if [ "$EUID" -ne 0 ]; then
    if [ -e "${PWD}/post_install.sh" ]; then
      echo
      echo "You might need to configure permissions for uploading."
      echo "To do so, run the following command from the terminal:"
      echo "sudo \"${PWD}/post_install.sh\""
      echo
    else
      # Script was executed from another path. It is assumed this will only occur when user is executing script directly.
      # So it is not necessary to provide the command line.
      echo "Please run as root"
    fi

    exit
  fi

  arduino_renesas_core_rules > /etc/udev/rules.d/60-arduino-renesas.rules

  # reload udev rules
  echo "Reload rules..."
  udevadm trigger
  udevadm control --reload-rules

  ;;
esac
