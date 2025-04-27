{
    python314,
    python314Packages,
    qwiic-otos,
    ...
}:
python314Packages.buildPythonApplication {
    pname = "otos-script";
    version = "0.0.1";
    pyproject = false;
    dontUnpack = true;

    propagatedBuildInputs = [
        (python314.withPackages (pythonPackages: with pythonPackages; [
            cobs
            pyserial
        ] ++ [ qwiic-otos ]))
    ];

    installPhase = ''
        install -Dm755 "${../otos-script.py}" "$out/bin/otos-script"
    '';

    meta = {
        mainProgram = "otos-script";
    };
}
