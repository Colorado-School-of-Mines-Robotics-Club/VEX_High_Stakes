{
    python314Packages,
    fetchPypi,
    ...
}:
python314Packages.buildPythonPackage rec {
    pname = "qwiic-i2c";
    version = "2.0.0";
    pyproject = true;

    src = fetchPypi {
        inherit version;
        pname = "sparkfun_qwiic_i2c";
        hash = "sha256-HpVngVwcekwD+sDZygGiS33rE2jTGbAU2qRdllgp+rY=";
    };

    build-system = with python314Packages; [
        setuptools
        setuptools-scm
    ];

    dependencies = with python314Packages; [
        smbus2
    ];

    meta = {
        # ...
    };
}
