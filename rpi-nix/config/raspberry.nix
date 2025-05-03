{
    boot = {
        kernelModules = [ "i2c_dev" ];
    };

    hardware.enableAllFirmware = true;
    hardware.raspberry-pi.config = {
        all = {
            base-dt-params = {
                i2c_arm = {
                    enable = true;
                    value = "on";
                };
            };
        };
    };
}
