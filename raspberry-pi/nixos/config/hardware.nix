{
    raspberry-pi-nix = {
        board = "bcm2711";
        # kernel-version = "v6_6_51";
        pin-inputs.enable = true;
        uboot.enable = false;
        libcamera-overlay.enable = false;
    };

    hardware = {
        raspberry-pi = {
            config = {
                all = {
                    base-dt-params = {
                        BOOT_UART = {
                            value = 1;
                            enable = true;
                        };
                        uart_2ndstage = {
                            value = 1;
                            enable = true;
                        };
                    };
                    dt-overlays = {
                        disable-bt = {
                            enable = true;
                            params = { };
                        };
                    };
                };
            };
        };
    };

    boot.kernelParams = [
        # "modules-load=dwc2,g_ether"

        # "g_ether.dev_addr=f2:5d:47:5e:e5:05"
        # "g_ether.host_addr=f2:5d:47:5e:e5:06"

        "g_ether.dev_addr=f2:5d:47:5e:e5:07"
        "g_ether.host_addr=f2:w5d:47:5e:e5:08"
    ];

    security.rtkit.enable = true;
}