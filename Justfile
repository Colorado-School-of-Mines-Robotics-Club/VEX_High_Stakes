upload-yin:
    pros mu --name "YIN" --slot 2 --project ./YIN

# TODO :c
install-rpi1 ssh-host:
    @echo "Using nixos-anywhere w/ latest stable kexec image..."
    nix run github:nix-community/nixos-anywhere -- \
        --flake .#rpi1 \
        --generate-hardware-config nixos-generate-config ./rpi-nix/hardware-configuration.nix \
        --extra-files ./nixos-anywhere-extras \
        --target-host "{{ssh-host}}"
