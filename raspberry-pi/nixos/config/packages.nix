{ pkgs, ... }:
{
    environment.systemPackages = with pkgs; [
        # Python w/ dependencies used in script
        (python313.withPackages(pyPkgs: with pyPkgs; [
            pyserial
            cobs
            # TODO Package sparkfun-qwiic-otos from wheel
        ]))
    ];
}