{ pkgs, lib, ... }:
{
    systemd.services = {
        otos-sensor = {
            description = "OTOS Sensor";
            environment.PYTHONUNBUFFERED = "1";

            serviceConfig = {
                Type = "notify";
                Restart = "always";
                ExecStart = "${lib.getExe pkgs.python313} ${../otos-sensor.py}";
            };

            wantedBy = [ "multi-user.target" ];
        };
    };
}