{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        raspberry-pi-nix.url = "github:nix-community/raspberry-pi-nix";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = inputs@{ nixpkgs, pros-nix, raspberry-pi-nix, ... }: let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; config.allowUnsupportedSystem = true; };
        pkgsCross = pkgs.pkgsCross.arm-embedded;
    in  {
        # Create a development shell containing the PROS CLI
        devShells.${system}.default = pkgsCross.mkShell {
            packages = [
                pros-nix.packages.${system}.pros-cli
            ];
        };

        # Create NixOS config for the OTOS pi
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
    };

    nixConfig = {
        extra-substituters = [ "https://nix-community.cachix.org" ];
        extra-trusted-public-keys = [ "nix-community.cachix.org-1:mB9FSh9qf2dCimDSUo8Zy7bkq5CX+/rkCWyvRCYg3Fs=" ];
    };
}
