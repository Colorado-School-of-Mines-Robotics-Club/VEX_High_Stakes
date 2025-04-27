{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        flake-utils.url = "github:numtide/flake-utils";
        colmena.url = "github:zhaofengli/colmena";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
        nixos-raspberrypi = {
            url = "github:nvmd/nixos-raspberrypi";
            # inputs.nixpkgs.follows = "nixpkgs";
        };
        sops-nix = {
            url = "github:Mic92/sops-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = inputs@{ self, nixpkgs, pros-nix, flake-utils, nixos-raspberrypi, colmena, ... }:
        (flake-utils.lib.eachDefaultSystem (system: let
            pkgs = nixpkgs.legacyPackages.${system};
            pkgsCross = pkgs.pkgsCross.arm-embedded;
        in  {
            # Create a development shell containing the PROS CLI
            devShells.default = pkgsCross.mkShell {
                packages = [
                    pros-nix.packages.${system}.pros-cli
                    self.packages.${system}.qwiic-otos
                    colmena.packages.${system}.colmena
                ] ++ (with pkgs.python314Packages; [
                    cobs
                    pyserial
                ]);
            };

            packages = {
                # Packages for generating the SD image to flash
                rpi1-sd = self.nixosConfigurations.rpi1-sd.config.system.build.sdImage;
                rpi2-sd = self.nixosConfigurations.rpi2-sd.config.system.build.sdImage;

                # Packaged python libraries
                qwiic-i2c = pkgs.callPackage ./rpi-nix/config/otos/packages/qwiic-i2c.nix {};
                qwiic-otos = pkgs.callPackage ./rpi-nix/config/otos/packages/qwiic-otos.nix {
                    inherit (self.packages.${system}) qwiic-i2c;
                };
                otos-script = pkgs.callPackage ./rpi-nix/config/otos/packages/otos-script.nix {
                    inherit (self.packages.${system}) qwiic-otos;
                };
            };
        })) // {
            # NixOS configurations
            nixosConfigurations = let
                rpi-nixpkgs = nixos-raspberrypi.inputs.nixpkgs;
                mkNixos = piNumber: rpi-nixpkgs.lib.nixosSystem {
                    system = "aarch64-linux";
                    pkgs = rpi-nixpkgs.legacyPackages."aarch64-linux";

                    specialArgs = { inherit inputs piNumber self; system = "aarch64-linux"; };

                    modules = [ ./rpi-nix ];
                };
                mkEtherGadget = config: config.extendModules {
                    modules = [ nixos-raspberrypi.nixosModules.usb-gadget-ethernet ];
                };
            in rec {
                # Add the main configuration
                rpi1 = mkNixos 1;
                rpi2 = mkNixos 2;

                rpi1-sd = mkEtherGadget rpi1;
                rpi2-sd = mkEtherGadget rpi2;
            };

            # Colmena configuration
            colmena = let
                mkColmena = piNumber: {
                    deployment = {
                        targetHost = "raspberry-pi-${builtins.toString piNumber}";
                        targetUser = "root";
                        targetPort = 22;
                    };

                    imports = [ ./rpi-nix ];
                };
            in {
                meta = {
                    nixpkgs = nixos-raspberrypi.inputs.nixpkgs.legacyPackages."aarch64-linux";

                    specialArgs = { inherit inputs self; system = "aarch64-linux"; };
                    nodeSpecialArgs = {
                        rpi1.piNumber = 1;
                        rpi2.piNumber = 2;
                    };
                };

                rpi1 = mkColmena 1;
                rpi2 = mkColmena 2;
            };
            colmenaHive = colmena.lib.makeHive self.outputs.colmena;
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
