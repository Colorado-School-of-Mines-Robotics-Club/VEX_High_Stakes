{
    # Setup user account(s)
    users = {
        mutableUsers = false;
        
        groups = {
            gpio = {};
        };
        users.pi = {
            isNormalUser = true;
            hashedPassword = "$y$j9T$/s0PS18ny1uY/a2lvLLEN.$yBUrCqyJFJ7hVpBZZnd82iwI/zQME.t156UNg3AvGG/";
            extraGroups = [ "wheel" "dialout" "gpio" "i2c" ];
        };
    };
}