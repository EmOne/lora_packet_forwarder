#!/bin/bash

if [[ -z "$1" ]] ; then
    # No binary specified
    echo "$0: No binary specified."
    exit 1
fi

reset_gw="bb-reset-lgw.sh"

binary="$1"
binary_name=`basename "$binary"`
binary_directory=`dirname "$binary"`
pushd "$binary_directory"
absolute_binary_directory="$(pwd)"
absolute_binary_path="$absolute_binary_directory/$binary_name"
popd

echo "[Unit]
Description=Packet Forwarder Service

[Install]
WantedBy=multi-user.target

[Service]
PIDFile=/var/spool/lora/pid/pkt-fwd.pid
TimeoutStartSec=infinity
Type=simple
TimeoutSec=infinity
RestartSec=10
WorkingDirectory=$absolute_binary_directory
ExecStartPre=$reset_gw start
ExecStart=$absolute_binary_path
Restart=always
BusName=org.thethingsnetwork.pkt-fwd" > /etc/systemd/system/pkt-fwd.service

echo "$0: Installation of the systemd service complete."
