{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        flake-utils.url = "github:numtide/flake-utils";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
        nixos-raspberrypi = {
            url = "github:nvmd/nixos-raspberrypi";
            # inputs.nixpkgs.follows = "nixpkgs";
        };
        disko = {
            url = "github:nix-community/disko";
            inputs.nixpkgs.follows = "nixpkgs";
        };
        nixos-facter = {
            url = "github:nix-community/nixos-facter";
            inputs.nixpkgs.follows = "nixpkgs";
        };
        nixos-facter-modules.url = "github:nix-community/nixos-facter-modules";
        sops-nix = {
            url = "github:Mic92/sops-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = inputs@{ self, nixpkgs, pros-nix, flake-utils, nixos-raspberrypi, ... }:
        (flake-utils.lib.eachDefaultSystem (system: let
            pkgs = nixpkgs.legacyPackages.${system};
            pkgsCross = pkgs.pkgsCross.arm-embedded;
        in  {
            # Create a development shell containing the PROS CLI
            devShells.default = pkgsCross.mkShell {
                packages = [
                    pros-nix.packages.${system}.pros-cli
                ];
            };

            # Create a package to generate the SD
            packages = {
                rpi1-sd = self.nixosConfigurations.rpi1-sd.config.system.build.sdImage;
                rpi2-sd = self.nixosConfigurations.rpi1-sd.config.system.build.sdImage;
            };
        })) // {
            nixosConfigurations = let
                rpi-nixpkgs = nixos-raspberrypi.inputs.nixpkgs;
                mkNixos = isSd: piNumber: rpi-nixpkgs.lib.nixosSystem {
                    system = "aarch64-linux";
                    pkgs = rpi-nixpkgs.legacyPackages."aarch64-linux";

                    specialArgs = { inherit inputs piNumber isSd; system = "aarch64-linux"; };

                    modules = [
                        # Inherit main configuration for ease
                        ./rpi-nix
                    ] ++ (if isSd then [
                        # Add SD-related options
                        nixos-raspberrypi.nixosModules.sd-image-uboot
                        nixos-raspberrypi.nixosModules.usb-gadget-ethernet
                        ({ lib, modulesPath, ... }: {
                            imports = [
                                (modulesPath + "/profiles/installation-device.nix")
                            ];

                            boot.swraid.enable = lib.mkForce false;
                            installer.cloneConfig = false;
                            sdImage.compressImage = false;

                            users.users = let
                                keys = [
                                    "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIH/2p94CtvFXI/CXLudCu5KYFf8Afes1o3G7T1wLFeGP openpgp:0x0D009703"
                                ];
                            in {
                                root.openssh.authorizedKeys.keys = keys;
                                nixos.openssh.authorizedKeys.keys = keys;
                            };
                        })
                    ] else []);
                };
                mkConfiguration = mkNixos false;
                mkSd = mkNixos true;
            in {
                # Add the main configuration
                rpi1 = mkConfiguration 1;
                rpi2 = mkConfiguration 2;

                # Add a configuration for an sd version
                rpi1-sd = mkSd 1;
                rpi2-sd = mkSd 2;
            };
        };

    nixConfig = {
        extra-substituters = [
            "https://nixos-raspberrypi.cachix.org"
            "https://numtide.cachix.org"
        ];
        extra-trusted-public-keys = [
            "nixos-raspberrypi.cachix.org-1:4iMO9LXa8BqhU+Rpg6LQKiGa2lsNh/j2oiYLNOQ5sPI="
            "numtide.cachix.org-1:2ps1kLBUWjxIneOy1Ik6cQjb41X0iXVXeHigGmycPPE="
        ];
    };
}
