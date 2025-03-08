# Credit https://github.com/cyber-murmel/nixos-rpi-zero-w/blob/main/minification.nix
{ lib, config, ... }:
{
    # don't build documentation
    documentation.info.enable = lib.mkDefault false;
    documentation.man.enable = lib.mkDefault false;

    # don't include a 'command not found' helper
    programs.command-not-found.enable = lib.mkDefault false;

    # disable firewall (needs iptables)
    networking.firewall.enable = lib.mkDefault false;

    # disable polkit
    security.polkit.enable = lib.mkDefault false;

    # disable audit
    security.audit.enable = lib.mkDefault false;

    # disable udisks
    services.udisks2.enable = lib.mkDefault false;

    # disable containers
    boot.enableContainers = lib.mkDefault false;

    # build less locales
    i18n.supportedLocales = [ (config.i18n.defaultLocale + "/UTF-8") ];
}