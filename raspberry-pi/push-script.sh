#!/usr/bin/env bash

SSH_CONNECTION="${1:-"pi@10.0.0.101"}"

rsync -aPz --delete ./script.py requirements.txt "$SSH_CONNECTION:/home/pi/code"

if [ "${2:-}" != "nopip" ]; then
    ssh "$SSH_CONNECTION" 'bash -s' << EOF
        cd /home/pi/code
        python3 -m venv venv
        . venv/bin/activate
        pip install -r requirements.txt
EOF
fi