{ pkgs, ... }:
{
    environment.systemPackages = with pkgs; [
        fastfetch
        btop

        python313Packages.pyserial
        python313Packages.cobs
        # TODO Package sparkfun-qwiic-otos from wheel
    ];

    security.sudo = {
        enable = true;
        wheelNeedsPassword = false; # Colmena requires passwordless sudo
        execWheelOnly = true;
    };
}