#!/bin/bash

cd "$(dirname "$0")"

interrupted=false
trap interrupted=true SIGINT

script_name="flight.sh"
script_path="$(pwd)/$script_name"

service_name="CARL-Air-Brake.service"
service_path="/etc/systemd/system/$service_name"

if [ "$(whoami)" != "root" ] ; then
  echo "This script requires superuser privileges" 1>&2
  exit 1
fi

echo "[Unit]"                                            >  $service_path
echo "Description: CARL Air Brake"                       >> $service_path
echo ""                                                  >> $service_path
echo "[Service]"                                         >> $service_path
echo "ExecStartPre=/bin/systemctl disable $service_name" >> $service_path
echo "ExecStartPre=/bin/rm $service_path"                >> $service_path
echo "ExecStart=/bin/bash $script_path"                  >> $service_path
echo ""                                                  >> $service_path
echo "[Install]"                                         >> $service_path
echo "WantedBy=multi-user.target"                        >> $service_path

systemctl enable $service_name

clear

echo "System is now armed and will automatically start on next boot. Press Ctrl+C to power off."

while ! $interrupted ; do
  :
done

shutdown now
