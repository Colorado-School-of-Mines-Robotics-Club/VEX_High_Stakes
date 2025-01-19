{
    description = "Colorado School of Mines VEX High Stakes Code";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
        flake-utils.url = "github:numtide/flake-utils";
        pros-nix = {
            url = "git+https://codeberg.org/tyy/pros-nix";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = { nixpkgs, flake-utils, pros-nix, ... }:
        # For each default supported system (amd/arm linux/mac)
        flake-utils.lib.eachDefaultSystem (system: let
            pkgs = nixpkgs.legacyPackages.${system};
        in {
            # Create a development shell containing the PROS CLI
            devShells.default = pkgs.mkShell {
                packages = [
                    pros-nix.packages.${system}.pros-cli
                ];
            };
        });
}
