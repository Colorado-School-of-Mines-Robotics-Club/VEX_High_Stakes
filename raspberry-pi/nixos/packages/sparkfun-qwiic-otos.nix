{ lib
, python312Packages
, fetchPypi
}:

python312Packages.buildPythonApplication rec {
    pname = "sparkfun-qwiic-otos";
    version = "0.0.1";
    format = "wheel";

    src = fetchPypi {
        pname = "sparkfun-qwiic-otos";
        inherit version format;
        # python = "py3";
        # dist = "py3";
        hash = "sha256-7oVNI8huosw8qmdKaKXimSAzZtcSNKZwZ1Vc5SLbaws=";
    };

    dependencies = with python312Packages; [
        
    ];

    meta = {
        changelog = "https://github.com/purduesigbots/pros-cli/releases/tag/${version}";
        description = "Command Line Interface for managing PROS projects. Works with V5 and the Cortex";
        homepage = "https://github.com/purduesigbots/pros-cli";
        license = lib.licenses.mpl20;
    };
}