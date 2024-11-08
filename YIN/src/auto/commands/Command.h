#pragma once

class Command {
    public:
        virtual void run() = 0;
        virtual bool finished() = 0;
};