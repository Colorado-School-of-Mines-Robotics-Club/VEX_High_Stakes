{ config, pkgs, lib, piNumber, ... }:
{
    networking = {
        hostName = "raspberry-pi-${builtins.toString piNumber}";

        firewall.enable = false; # TODO Maybe?

        # Configure wifi
        wireless = {
            enable = true;
            userControlled.enable = true;
            networks = {
                # Mines unauthenticated network
                "Mines-Legacy" = {
                    priority = 100;
                    auth = ''
                        key_mgmt=NONE
                    '';
                };
                # Tyler hotspot
                "typixel" = {
                    priority = 50;
                    psk = "hhhhhhhh";
                };
            };
        };

        # networking is managed by systemd-networkd and wpa_supplicant, disable everything else
        networkmanager.enable = false;
        useDHCP = false;
    };

    systemd.network = {
        enable = true;

        networks."10-wireless" = {
            matchConfig = {
                Name = "wlan0";
            };

            linkConfig.RequiredForOnline = "yes";

            networkConfig = {
                DHCP = "yes";
                IPv6AcceptRA = "yes";

                IgnoreCarrierLoss = "3s";
            };
        };
    };

    services.openssh.enable = true;
    users.users.pi.openssh.authorizedKeys.keys = [
        "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIH/2p94CtvFXI/CXLudCu5KYFf8Afes1o3G7T1wLFeGP openpgp:0x0D009703"
    ];

    services.tailscale = {
        enable = true;
        useRoutingFeatures = "both";
        openFirewall = true;

        authKeyFile = "${config.sops.secrets."tailscale/preauth_key".path}";

        extraUpFlags = [
            "--reset"
            "--ssh"
            "--accept-dns"
            "--accept-routes"
            "--advertise-tags=tag:rpi"
            "--login-server=https://ts.myriation.xyz"
            "--operator=ty"
        ];
    };
    systemd.services.tailscaled.environment = { "TS_DEBUG_MTU" = "1350"; }; # https://github.com/tailscale/tailscale/issues/2633
    networking.networkmanager.dispatcherScripts = [{
        type = "pre-up";
        source = lib.getExe (pkgs.writeShellScriptBin "tailscale-optimizations" ''
            ${lib.getExe pkgs.ethtool} -K $DEVICE_IP_IFACE rx-udp-gro-forwarding on rx-gro-list off
        '');
    }];
    services.networkd-dispatcher = {
        enable = true;
        rules = {
            tailscale-optimizations = {
                onState = [ "routable" ];
                script = ''
                    ${lib.getExe pkgs.ethtool} -K $IFACE rx-udp-gro-forwarding on rx-gro-list off
                '';
            };
        };
    };
}
