#pragma once

#include "Command.h"
#include "intake.h"

class SetIntaking : public Command {
private:
    Intake* intake;
public:
    void run() {
        intake->setIntaking();
    }
    bool finished() {
        return true;
    }
};
    
