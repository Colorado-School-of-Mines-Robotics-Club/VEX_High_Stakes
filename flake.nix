{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = { nixpkgs, pros-nix, ... }: let
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
    };
}
