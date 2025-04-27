{ lib, self, system, ... }:
{
    systemd.services.otos-sensor = {
        description = "matrix authentication server";
        wantedBy = [ "multi-user.target" ];

        serviceConfig = {
            DynamicUser = true;
            User = "otos";
            Group = "otos";

            ExecStart = "${lib.getExe self.packages.${system}.otos-script}";
        };
    };
}
