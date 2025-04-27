#include "pure_pursuit.h"
#include "drive.h"
#include "pros/llemu.hpp"
#include <cmath>
#include <optional>

#define SQR(x) std::pow(x, 2)

std::optional<Point> getIntersection(OtosData pos, double x1, double y1, double x2, double y2) {
    double x1_offset = x1 - pos.x;
    double y1_offset = y1 - pos.y;
    double x2_offset = x2 - pos.x;
    double y2_offset = y2 - pos.y;

    double d_x = x2_offset - x1_offset;
    double d_y = y2_offset - y1_offset;
    double d_r_sqr = SQR(d_x) + SQR(d_y);
    double D = x1 * y2 - x2 * y1;
    double discriminant = SQR(LOOK_AHEAD_DISTANCE) * d_r_sqr - SQR(D);

    if (discriminant >= 0) {
        Point first;
        first.x = D * d_y + (d_y < 0 ? -1 : 1) * d_x * std::sqrt(discriminant) / d_r_sqr;
        first.y = - D * d_x + std::fabs(d_y) * std::sqrt(discriminant) / d_r_sqr;
        
        Point second;
        second.x = D * d_y - (d_y < 0 ? -1 : 1) * d_x * std::sqrt(discriminant);
        second.y = - D * d_x - std::fabs(d_y) * std::sqrt(discriminant) / d_r_sqr;

        bool firstXValid = first.x >= std::min(x1, x2) && first.x <= std::max(x1, x2);
        bool firstYValid = first.y >= std::min(y1, y2) && first.y <= std::max(y1, y2);
        bool secondXValid = second.x >= std::min(x1, x2) && first.x <= std::max(x1, x2);
        bool secondYValid = second.y >= std::min(y1, y2) && first.y <= std::max(y1, y2);
        if (firstXValid && firstYValid && secondXValid && secondYValid) {
            // Both points are valid, return the furthest along one
            return
                std::sqrt(SQR(first.x - x2) + SQR(first.y - y2)) < std::sqrt(SQR(second.x - x2) + SQR(second.y - y2))
                ? std::optional(first)
                : std::optional(second);
        } else if (firstXValid && firstYValid) {
            // Only first is valid, return that
            return std::optional(first);
        } else if (secondXValid && secondYValid) {
            // Only second is valid, return that
            return std::optional(second);
        } else {
            // None are valid, return nullopt
            return std::nullopt;
        }
    } else {
        return std::nullopt;
    }
}

PurePursuit::PurePursuit(OtosSensor otos, double** path, size_t pathSize) : otos(otos), path(path), pathSize(pathSize) {}

std::optional<Point> PurePursuit::getNextLookahead(OtosData pos) {
    auto intersection = getIntersections(
        pos,
        this->path[currentGoal][0],
        this->path[currentGoal][1],
        this->path[currentGoal + 1][0],
        this->path[currentGoal][1]
    );

    if (!intersection.has_value()) {
        pros::lcd::set_text(4, "LOST LINE :c");
    }

    return intersection;
}

void PurePursuit::setMotors() {
    OtosData pos = this->otos.get_latest_position();
    auto currentYaw = Drive::getYaw();
    if (currentYaw < 0) currentYaw += 360; // Correct range from [-180, 180] -> [0, 360]

    auto nextLookahead = this->getNextLookahead(pos);

    // Exit early on lost line
    if (!nextLookahead.has_value()) {
        return;
    }

    // Calculate linear error
    double linearError = std::sqrt(SQR(nextLookahead->x - pos.x) + SQR(nextLookahead->y - pos.y));

    // Calculate angular error
    double lookaheadAngle = std::atan2(nextLookahead->y - pos.y, nextLookahead->x - pos.x);
    lookaheadAngle = lookaheadAngle / (2*M_PI) * 360; // Convert from radians to degrees
    if (lookaheadAngle < 0) lookaheadAngle += 360; // Correct range from [-180, 180] -> [0, 360]
    double angularError = lookaheadAngle - currentYaw;

    // Calculate minimum turning angle
    double minAngle = angularError;
    if (minAngle > 180 || minAngle < -180) {
        minAngle = -1 * (minAngle >= 0 ? 1 : 0) * (360 - std::fabs(minAngle));
    }

    // Calculate motor velocities
    double leftSpeed = linearError - angularError;
    double rightSpeed = linearError + angularError;

    Drive::move(leftSpeed, rightSpeed);
}