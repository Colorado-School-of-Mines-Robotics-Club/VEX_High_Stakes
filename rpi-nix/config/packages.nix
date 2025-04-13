{ system, inputs, ... }:
{
    environment.systemPackages = [
        inputs.nixos-facter.packages.${system}.nixos-facter
    ];
}
