{ inputs, lib, piNumber, ... }:
let
    variants = {
        wifiMacs = {
            "1" = "FE:64:28:75:5A:87";
            "2" = "FE:30:EC:DB:0D:BA";
        };
        etherKernelParams = {
            "1" = [ "g_ether.dev_addr=f2:5d:47:5e:e5:05" "g_ether.host_addr=f2:5d:47:5e:e5:06" ];
            "2" = [ "g_ether.dev_addr=f2:5d:47:5e:e5:07" "g_ether.host_addr=f2:5d:47:5e:e5:08" ];
        };
    };
in {
    system.stateVersion = "25.05";

    imports = let
        rpiModules = inputs.nixos-raspberrypi.nixosModules;
        rpiLib = inputs.nixos-raspberrypi.lib;
    in [
        # Base for the Pi Zero
        rpiModules.raspberry-pi-02.base

        # Allow building an SD card from this config
        rpiModules.sd-image
        { sdImage.compressImage = false; }

        inputs.nixos-facter-modules.nixosModules.facter
        { config.facter.reportPath = ./facter.json; }

        # Add raspberry pi overlays to nixpkgs
        rpiModules.nixpkgs-rpi
        rpiLib.inject-overlays
        # rpiLib.inject-overlays-global

        # Secrets
        ./secrets.nix

        ./config
    ];

    # Enable binary caches on system
    nix.settings = {
        extra-experimental-features = [ "nix-command" "flakes" ];
        substituters = [
            "https://nixos-raspberrypi.cachix.org"
            "https://numtide.cachix.org"
        ];
        trusted-substituters = [
            "https://nixos-raspberrypi.cachix.org"
            "https://numtide.cachix.org"
        ];
        trusted-public-keys = [
            "nixos-raspberrypi.cachix.org-1:4iMO9LXa8BqhU+Rpg6LQKiGa2lsNh/j2oiYLNOQ5sPI="
            "numtide.cachix.org-1:2ps1kLBUWjxIneOy1Ik6cQjb41X0iXVXeHigGmycPPE="
        ];
    };

    # Configure static MAC address for ethernet gadget
    boot.kernelParams = variants.etherKernelParams.${builtins.toString piNumber};

    # Configure hardcoded MAC address for Wi-Fi
    hardware.raspberry-pi.config.all.base-dt-params.wifiaddr = {
        enable = true;
        value = variants.wifiMacs.${builtins.toString piNumber};
    };
}
