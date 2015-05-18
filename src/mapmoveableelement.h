#ifndef MAPMOVEABLEELEMENT_H
#define MAPMOVEABLEELEMENT_H

#include <QtCore/QString>
#include "global.h"

struct MapMoveableElement
{
    enum class ElementKind
    {
        AVATAR,
        BOMB
    };

    double xWithinTile() const
    {
        return (is_left ? -1. : 1.)
                * double{filled_max() - x_filled} / double{filled_max() + 1};
    }

    double yWithinTile() const
    {
        return (is_up ? -1. : 1.)
            * double{filled_max() - y_filled} / double{filled_max() + 1};
    }


    /**
     * Returns the number of moved tiles.
     */
    unsigned moveUp(double seconds)
    {
        auto ret = full_steps(seconds);

        /* To ease implementation, we displace the element to the center and
           then we compensate later. I want to believe that good code will be
           generated from this mess. */
        int error = filled_max() - y_filled;
        bool previous_is_up = is_up;
        y_filled = filled_max();

        // Actual movement
        auto steps = full_steps_remainder(seconds);
        if (steps > filled_max()) {
            is_up = true;
            y_filled = steps - filled_max() - 1;
            ++ret;
        } else {
            is_up = false;
            y_filled -= steps;
        }

        // Compensating the error...
        if (error != 0) {
            if (previous_is_up && is_up) {
                if (error > y_filled) {
                    y_filled = error - (y_filled + 1);
                    is_up = false;
                    --ret;
                } else {
                    y_filled -= error;
                }
            } else if (previous_is_up && !is_up) {
                if (y_filled + error > filled_max()) {
                    y_filled = 2 * filled_max() - (y_filled + error);
                    is_up = true;
                } else {
                    y_filled += error;
                }
            } else if (!previous_is_up && is_up) {
                if (y_filled + error > filled_max()) {
                    y_filled = 2 * filled_max() - (y_filled + error);
                    is_up = false;
                } else {
                    y_filled += error;
                }
            } else { // !previous_is_up && !is_up
                if (error > y_filled) {
                    y_filled = error - (y_filled + 1);
                    is_up = true;
                    --ret;
                } else {
                    y_filled -= error;
                }
            }
        }

        is_up = !is_up;
        return ret;
    }

    /**
     * Returns the number of moved tiles.
     */
    unsigned moveRight(double seconds)
    {
        auto ret = full_steps(seconds);

        /* To ease implementation, we displace the element to the center and
           then we compensate later. I want to believe that good code will be
           generated from this mess. */
        int error = filled_max() - x_filled;
        bool previous_is_left = is_left;
        x_filled = filled_max();

        // Actual movement
        auto steps = full_steps_remainder(seconds);
        if (steps > filled_max()) {
            is_left = true;
            x_filled = steps - filled_max() - 1;
            ++ret;
        } else {
            is_left = false;
            x_filled -= steps;
        }

        // Compensating the error...
        if (error != 0) {
            if (previous_is_left && is_left) {
                if (error > x_filled) {
                    x_filled = error - (x_filled + 1);
                    is_left = false;
                    --ret;
                } else {
                    x_filled -= error;
                }
            } else if (previous_is_left && !is_left) {
                if (x_filled + error > filled_max()) {
                    x_filled = 2 * filled_max() - (x_filled + error);
                    is_left = true;
                } else {
                    x_filled += error;
                }
            } else if (!previous_is_left && is_left) {
                if (x_filled + error > filled_max()) {
                    x_filled = 2 * filled_max() - (x_filled + error);
                    is_left = false;
                } else {
                    x_filled += error;
                }
            } else { // !previous_is_up && !is_up
                if (error > x_filled) {
                    x_filled = error - (x_filled + 1);
                    is_left = true;
                    --ret;
                } else {
                    x_filled -= error;
                }
            }
        }

        return ret;
    }

    /**
     * Returns the number of moved tiles.
     */
    unsigned moveDown(double seconds)
    {
        auto ret = full_steps(seconds);

        /* To ease implementation, we displace the element to the center and
           then we compensate later. I want to believe that good code will be
           generated from this mess. */
        int error = filled_max() - y_filled;
        bool previous_is_up = is_up;
        y_filled = filled_max();

        // Actual movement
        auto steps = full_steps_remainder(seconds);
        if (steps > filled_max()) {
            is_up = true;
            y_filled = steps - filled_max() - 1;
            ++ret;
        } else {
            is_up = false;
            y_filled -= steps;
        }

        // Compensating the error...
        if (error != 0) {
            if (previous_is_up && is_up) {
                if (error > y_filled) {
                    y_filled = error - (y_filled + 1);
                    is_up = false;
                    --ret;
                } else {
                    y_filled -= error;
                }
            } else if (previous_is_up && !is_up) {
                if (y_filled + error > filled_max()) {
                    y_filled = 2 * filled_max() - (y_filled + error);
                    is_up = true;
                } else {
                    y_filled += error;
                }
            } else if (!previous_is_up && is_up) {
                if (y_filled + error > filled_max()) {
                    y_filled = 2 * filled_max() - (y_filled + error);
                    is_up = false;
                } else {
                    y_filled += error;
                }
            } else { // !previous_is_up && !is_up
                if (error > y_filled) {
                    y_filled = error - (y_filled + 1);
                    is_up = true;
                    --ret;
                } else {
                    y_filled -= error;
                }
            }
        }

        return ret;
    }

    bool moveLeft(double seconds)
    {
        auto ret = full_steps(seconds);

        /* To ease implementation, we displace the element to the center and
           then we compensate later. I want to believe that good code will be
           generated from this mess. */
        int error = filled_max() - x_filled;
        bool previous_is_left = is_left;
        x_filled = filled_max();

        // Actual movement
        auto steps = full_steps_remainder(seconds);
        if (steps > filled_max()) {
            is_left = true;
            x_filled = steps - filled_max() - 1;
            ++ret;
        } else {
            is_left = false;
            x_filled -= steps;
        }

        // Compensating the error...
        if (error != 0) {
            if (previous_is_left && is_left) {
                if (error > x_filled) {
                    x_filled = error - (x_filled + 1);
                    is_left = false;
                    --ret;
                } else {
                    x_filled -= error;
                }
            } else if (previous_is_left && !is_left) {
                if (x_filled + error > filled_max()) {
                    x_filled = 2 * filled_max() - (x_filled + error);
                    is_left = true;
                } else {
                    x_filled += error;
                }
            } else if (!previous_is_left && is_left) {
                if (x_filled + error > filled_max()) {
                    x_filled = 2 * filled_max() - (x_filled + error);
                    is_left = false;
                } else {
                    x_filled += error;
                }
            } else { // !previous_is_up && !is_up
                if (error > x_filled) {
                    x_filled = error - (x_filled + 1);
                    is_left = true;
                    --ret;
                } else {
                    x_filled -= error;
                }
            }
        }

        is_left = !is_left;
        return ret;
    }

    static constexpr unsigned filled_min()
    {
        return 0;
    }

    static constexpr unsigned filled_max()
    {
        return 7;
    }

private:
    static constexpr unsigned FILLED_MIN = 0;
    static constexpr unsigned FILLED_MAX = 7;

public:
    // TODO: needs a better name (range_size, interval_size, states_count?)
    /* (filled_max() - filled_min()) possible states at each side and the
       centered state */
    static constexpr unsigned filled_granularity
        = 2 * (FILLED_MAX - FILLED_MIN) + 1;

    unsigned is_up: 1;
    unsigned y_filled: 3; // [0,8)
    unsigned is_left: 1;
    unsigned x_filled: 3; // [0,8)

    QString id;
    ElementKind kind;

private:
    static unsigned full_steps(double seconds)
    {
        int ms = seconds * 1000.;
        int nfills = ms / MS_PER_FILL_UNIT;
        return nfills / filled_granularity;
    }

    static unsigned full_steps_remainder(double seconds)
    {
        int ms = seconds * 1000.;
        int nfills = ms / MS_PER_FILL_UNIT;
        return nfills % filled_granularity;
    }
};

#endif // MAPMOVEABLEELEMENT_H
