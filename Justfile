upload-yin:
    pros mu --name "YIN" --slot 2 --project ./YIN

initial-gadget number="1":
    #!/usr/bin/env bash

    echo "Waiting ssh to start..."

    while ! timeout 1 bash -c "echo > /dev/tcp/10.0.0.2/22"; do
      sleep 1
    done

    echo "Copying sops-nix key to /tmp..."

    scp \
        -o BindAddress=10.0.0.100 \
        -o StrictHostKeyChecking=no \
        nixos-anywhere-extras/var/lib/sops-nix/key.txt \
        pi@10.0.0.2:/tmp/key.txt

    echo "Moving sops-nix key to correct location..."
    ssh \
        -o BindAddress=10.0.0.100 \
        -o StrictHostKeyChecking=no \
        -t \
        pi@10.0.0.2 \
        'sudo mv /tmp/key.txt /var/lib/sops-nix/key.txt && sudo chown -R root:root /var/lib/sops-nix'

    echo "Sending new configuration to pi for next boot..."

    nixos-rebuild \
        --flake '.#rpi{{number}}' \
        --accept-flake-config \
        --use-remote-sudo \
        --target-host "pi@10.0.0.2" \
        boot

    echo "Raspberry pi initialized, next boot will be set up"

upload-rpi number="1" ssh-host-prefix="pi@mines-rpi0":
    nixos-rebuild \
        --flake '.#rpi{{number}}' \
        --accept-flake-config \
        --use-remote-sudo \
        --target-host "{{ssh-host-prefix}}-{{number}}" \
        switch

qemu-build-sd number="1":
    #!/usr/bin/env bash

    nix build \
        --accept-flake-config \
        --out-link "./.gcroots/rpi{{number}}-sd" \
        --print-out-paths \
        --builders '' \
        '.#rpi{{number}}-sd'

write-sd path="result" device="sdb":
    run0 sh -c "$(which pv) -Y  < {{path}}/sd-image/nixos-sd-image-rpi02-uboot.img > /dev/{{device}}"
