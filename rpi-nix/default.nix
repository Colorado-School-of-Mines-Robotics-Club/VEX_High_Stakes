{ inputs, ... }:
{
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

        # Secrets
        ./secrets.nix

        ./config
    ];
}
