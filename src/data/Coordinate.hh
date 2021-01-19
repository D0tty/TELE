//
// Created by Thomas Michelot on 12/9/20.
//

#ifndef TELE_COORDINATE_HH
#define TELE_COORDINATE_HH

template<typename T>
class Coordinate {
public:
    Coordinate() : x_(0), y_(0) {}


    /**
     * @param x = Longitude
     * @param y = Latitude
     */
    /*
    Coordinate(T x_long, T y_lat) {
        x_ = x_long;
        y_ = y_lat;
    }
    */

    /**
     * @brief Returns X
     * @return Returns X
     */
    T getX() const {
        return x_;
    }

    /**
     * @brief Returns Longitude
     * @return Returns Longitude
     */
    /*
    T getLongitude() {
        return x_;
    }
    */

    /**
     * @brief Returns Y
     * @return Returns Y
     */
    T getY() const {
        return y_;
    }

    /**
     * @brief Returns Latitude
     * @return Returns Latitude
     */
    /*
    T getLatitude() {
        return y_;
    }
    */

    /**
     * @brief Sets X
     * @param x = Longitude
     */
    void setX(T x) {
        this->x_ = x;
    }

    /**
     * @brief Sets Longitude
     * @param x = Longitude
     */
    /*
    void setLongitude(T longitude) {
        this->x_ = longitude;
    }
    */

    /**
     * @brief Sets Y or Latitude
     * @param y = Latitude
     */
    void setY(T y) {
        this->y_ = y;
    }

    /**
     * @brief Sets Latitude
     * @param y = Latitude
     */
    /*
    void setLatitude(T latitude) {
        this->y_ = latitude;
    }
    */

    T x_;
    T y_;
};


#endif //TELE_COORDINATE_HH
