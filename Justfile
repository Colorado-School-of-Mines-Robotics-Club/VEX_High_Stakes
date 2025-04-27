upload-yin:
    pros mu --name "YIN" --slot 2 --project ./YIN

ssh-gadget:
    ssh -b 10.0.0.101 pi@10.0.0.2

upload-rpi ssh-host="pi@raspberry-pi-1":
    nixos-rebuild \
        --flake .#rpi1 \
        --accept-flake-config \
        --use-remote-sudo \
        --target-host "{{ssh-host}}" \
        switch
