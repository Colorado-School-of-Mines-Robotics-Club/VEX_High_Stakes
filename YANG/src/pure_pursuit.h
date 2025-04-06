#ifndef PURE_PURSUIT
#define PURE_PURSUIT

#include "otos.h"
#include <cstddef>
#include <optional>
#include <utility>

#define LOOK_AHEAD_DISTANCE 12. // 12 inches, or 1ft, purdue recommends 0.7ft-1.5ft
#define LINEAR_SCALAR 1.
#define ANGULAR_SCALAR 1.

struct Point {
    double x;
    double y;
};

class PurePursuit {
    private:
        OtosSensor otos;
        double** path;
        size_t pathSize;

        uint8_t currentGoal = 0;

        std::optional<Point> getIntersections(OtosData pos, double x1, double y1, double x2, double y2);
        std::optional<Point> getNextLookahead(OtosData pos);
    public:
        PurePursuit(OtosSensor otos, double** path, size_t pathSize);

        void setMotors();
};


#endif // PURE_PURSUIT
