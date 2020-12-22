//
// Created by dotty on 12/9/20.
//

#ifndef TELE_COORDINATE_HH
#define TELE_COORDINATE_HH

template<typename T>
class Coordinate {
public:
    Coordinate() : x_(0), y_(0) {}

    Coordinate(T x, T y) {
        x_ = x;
        y_ = y;
    }

    T getX() {
        return x_;
    }

    T getY() {
        return y_;
    }

    T x_;
    T y_;
};


#endif //TELE_COORDINATE_HH
