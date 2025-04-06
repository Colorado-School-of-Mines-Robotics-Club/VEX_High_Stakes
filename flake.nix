{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        raspberry-pi-nix.url = "github:nix-community/raspberry-pi-nix";
        colmena.url = "github:zhaofengli/colmena";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = inputs@{ self, nixpkgs, pros-nix, raspberry-pi-nix, colmena, ... }: let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; config.allowUnsupportedSystem = true; };
        pkgsCross = pkgs.pkgsCross.arm-embedded;
    in  {
        # Create a development shell containing the PROS CLI & colmena for RPI deployment
        devShells.${system}.default = pkgsCross.mkShell {
            packages = [
                pros-nix.packages.${system}.pros-cli
                colmena.packages."${system}".colmena
            ];
        };

        # RPI NixOS configs
        nixosConfigurations = {
            rpi-1 = nixpkgs.lib.nixosSystem {
                system = "aarch64-linux";
                modules = [
                    raspberry-pi-nix.nixosModules.raspberry-pi
                    raspberry-pi-nix.nixosModules.sd-image

                    ./raspberry-pi/nixos/entrypoint.nix
                ];

                specialArgs = { inherit inputs; };
            };
        };

        colmena = let
            conf = self.nixosConfigurations;
            deploymentOptions = {
                rpi-1 = {
                    targetUser = "pi";
                    targetHost = "10.25.236.177";
                    tags = [ "otos" ];
                };
            };
        in {
            meta = {
                nixpkgs = pkgs;
                nodeNixpkgs = builtins.mapAttrs (name: value: value.pkgs) conf;
                nodeSpecialArgs = builtins.mapAttrs (name: value: value._module.specialArgs) conf;
            };

            # rpi-2.deployment = {
            #     targetUser = "pi";
            #     targetHost = "10.0.0.202";
            #     tags = [ "otos" ];
            # };
        } // builtins.mapAttrs (name: value: { imports = value._module.args.modules; deployment = deploymentOptions.${name}; }) conf;
        colmenaHive = colmena.lib.makeHive self.colmena;
    };

    nixConfig = {
        extra-substituters = [
            "https://nix-community.cachix.org"
            "https://colmena.cachix.org"
        ];
        extra-trusted-public-keys = [
            "nix-community.cachix.org-1:mB9FSh9qf2dCimDSUo8Zy7bkq5CX+/rkCWyvRCYg3Fs="
            "colmena.cachix.org-1:7BzpDnjjH8ki2CT3f6GdOk7QAzPOl+1t3LvTLXqYcSg="
        ];
    };
}
