{
    # Configure wpa_supplicant to connect to wifi
    networking.wireless = {
        enable = true;
        networks = {
            "Mines-Legacy" = {
                extraConfig = "key_mgmt=NONE";
            };
        };
    };

    # Setup DHCP on wifi and statically-configured usb networking
    systemd.network = {
        networks = {
            "10-wireless" = {
                matchConfig.Name = "wlan0";
                networkConfig.DHCP = "ipv4";

                linkConfig.RequiredForOnline = "routable";
            };

            "10-usb" = {
                matchConfig.Name = "usb0";
                
                address = [
                    "10.0.0.101/32"
                ];
                routes = [
                    { Gateway = "10.0.0.1"; }
                ];
            };
        };
    };

    # Enable remote login
    services.openssh = {
        enable = true;
        settings = {
            PasswordAuthentication = true;
        };
    };
}