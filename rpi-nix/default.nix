{ inputs, lib, piNumber, isSd, ... }:
{
    system.stateVersion = "25.05";

    imports = let
        rpiModules = inputs.nixos-raspberrypi.nixosModules;
        rpiLib = inputs.nixos-raspberrypi.lib;
    in [
        # Base for the Pi Zero
        rpiModules.raspberry-pi-02.base

        # Add raspberry pi overlays to nixpkgs
        rpiModules.nixpkgs-rpi
        rpiLib.inject-overlays

        # Secrets
        ./secrets.nix

        ./config
    ] ++ (lib.optionals (!isSd) [
        # Disko and facter
        inputs.disko.nixosModules.disko
        ./disk-configuration.nix

        # inputs.nixos-facter-modules.nixosModules.facter
        # { facter.reportPath = ./facter.json; }
        ./hardware-configuration.nix
    ]);

    # Enable binary caches on system
    nix.settings = {
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
    boot.kernelParams =
        lib.optionals (piNumber == 1) [ "g_ether.dev_addr=f2:5d:47:5e:e5:05" "g_ether.host_addr=f2:5d:47:5e:e5:06" ] ++
        lib.optionals (piNumber == 2) [ "g_ether.dev_addr=f2:5d:47:5e:e5:07" "g_ether.host_addr=f2:5d:47:5e:e5:08" ];
}
