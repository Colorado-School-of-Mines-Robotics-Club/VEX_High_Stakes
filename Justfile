_default:
    @just --list

# Upload the YIN bot's code
[group("pros")]
upload-yin qualifier="competition":
    pros mu --name "YIN {{qualifier}}" --slot 1 --project ./YIN

# Upload the YANG bot's code
[group("pros")]
upload-yang qualifier="competition":
    pros mu --name "YANG {{qualifier}}" --slot 1 --project ./YANG

# Start YIN's code for testing
[group("pros")]
run-yin: (upload-yin "testing")
    -sleep 2
    pros v5 run 1

# Start YANG's code for testing
[group("pros")]
run-yang: (upload-yang "testing")
    -sleep 2
    pros v5 run 1

# Setup raspberry pi using ethernet gadget, to be used after first boot from SD
[group("rpi")]
setup-rpi number="1" sops-key-path="sops-key.txt":
    #!/usr/bin/env bash

    echo "Waiting ssh to start..."

    while ! timeout 1 bash -c "echo > /dev/tcp/10.0.0.2/22"; do
      sleep 1
    done

    echo "Copying sops-nix key to /tmp..."

    scp \
        -o BindAddress=10.0.0.100 \
        -o StrictHostKeyChecking=no \
        "{{sops-key-path}}" \
        pi@10.0.0.2:/tmp/key.txt

    echo "Moving sops-nix key to correct location..."
    ssh \
        -o BindAddress=10.0.0.100 \
        -o StrictHostKeyChecking=no \
        -t \
        pi@10.0.0.2 \
        'sudo mv /tmp/key.txt /var/lib/sops-nix/key.txt && sudo chown -R root:root /var/lib/sops-nix'

    echo "Sending new configuration to pi for next boot..."

    export NIX_SSHOPTS="-o BindAddress=10.0.0.100 -o StrictHostKeyChecking=no"
    nixos-rebuild \
        --flake '.#rpi{{number}}' \
        --accept-flake-config \
        --use-remote-sudo \
        --target-host "pi@10.0.0.2" \
        boot

    echo "Raspberry pi initialized, next boot will be set up"

    read -p "Reboot now [yN]? " -n 1 -r
    echo

    if [[ "$REPLY" =~ ^[Yy]$ ]]
    then
        ssh \
            -o BindAddress=10.0.0.100 \
            -o StrictHostKeyChecking=no \
            -t \
            pi@10.0.0.2 \
            'sudo systemctl reboot'
    fi

# Push new NixOS configuration to raspberry pi
[group("rpi")]
upload-rpi number="1" ssh-host-prefix="pi@mines-rpi0":
    NIX_SSHOPTS="${NIX_SSHOPTS:-} -o StrictHostKeyChecking=no" \
    nixos-rebuild \
        --flake '.#rpi{{number}}' \
        --accept-flake-config \
        --use-remote-sudo \
        --target-host "{{ssh-host-prefix}}-{{number}}" \
        switch

# Build the SD card image for a raspberry pi
[group("rpi")]
build-sd number="1" build-sd-locally="true":
    # Build the top level (potentially using a remote builder)
    nix build \
        --accept-flake-config \
        --out-link "./.gcroots/rpi{{number}}-sd" \
        --print-out-paths \
        '.#nixosConfigurations.rpi{{number}}-sd.config.system.build.toplevel'
    # Build the SD image (only locally)
    nix build \
        --accept-flake-config \
        --out-link "./.gcroots/rpi{{number}}-sd" \
        --print-out-paths \
        {{if build-sd-locally == "true" { "--builders ''" } else { "" } }} \
        '.#rpi{{number}}-sd'

# Write the SD card image for a raspberry pi onto an SD card. The number argument is used only if the default image path is used
[group("rpi"), positional-arguments]
write-sd device number="1" image-path="./.gcroots/rpi${2}-sd":
    run0 sh -c "$(which pv) -Y  < {{image-path}}/sd-image/nixos-sd-image-rpi02-uboot.img > /dev/{{device}}"
