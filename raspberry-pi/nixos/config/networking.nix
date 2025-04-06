{ pkgs, lib, ... }:
{
    services.networkd-dispatcher = {
        enable = true;
        rules."discord-webhook" = {
            onState = ["configured"];
            script = let
                ip = lib.getExe' pkgs.iproute2 "ip";
                grep = lib.getExe pkgs.gnugrep;
                curl = lib.getExe pkgs.curl;
            in ''
                #!${pkgs.runtimeShell}
                DISCORD_WEBHOOK_URL="https://discord.com/api/webhooks/1338251821499551888/0AC-b47l1DS2eR9JI3NFC7uMFnKxvgzq8WPLJ--xE1ApAwe6hMtfqgfTDsuhxr2dTXMf"
                MESSAGE_ID="1338279003236208722"
                MESSAGE_PREFIX="RPI 2 (nixos): "

                if [[ $IFACE == "wlan0" && $AdministrativeState == "configured" ]]; then
                    IP_ADDR="$(${ip} addr | ${grep} -P '.+\d+\.\d+\.\d+\.\d.+wlan0' | ${grep} -Po '\d+\.\d+\.\d+\.\d+' | head -n 1)"
                    ${curl} --interface wlan0 "$DISCORD_WEBHOOK_URL/messages/$MESSAGE_ID" -X PATCH -H "Content-Type: application/json" --data "{\"content\":\"''${MESSAGE_PREFIX}''${IP_ADDR}\"}" || exit 0
                fi
                exit 0
            '';
        };
    };

    # Configure wpa_supplicant to connect to wifi
    networking.wireless = {
        enable = true;
        networks = {
            "Mines-Legacy" = {
                extraConfig = "key_mgmt=NONE";
            };
            "ty-pixel" = {
                psk = "hhhhhhhh";
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