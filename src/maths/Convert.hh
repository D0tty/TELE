//
// Created by dotty on 1/3/21.
//

#ifndef TELE_CONVERT_HH
#define TELE_CONVERT_HH

#include <cmath>
#include <data/Coordinate.hh>
#include <metadata/GeoTaggedImage.hh>

class Convert {
public:
    static double getLongitude(const GeoTaggedImage &image);

    static double getLatitude(const GeoTaggedImage &image);

    static double getSpeed(const GeoTaggedImage &image0, const GeoTaggedImage &image1);

    static double getOrientation(const GeoTaggedImage &image0, const GeoTaggedImage &image1);

private:
    static constexpr double CFAC = -781648343.0;
    static constexpr double LFAC = -781648343.0;
    static constexpr double COFF = 1856.0;
    static constexpr double LOFF = 1856.0;
    static constexpr double FLAT_DEG_ANGLE = 180.0;
    static constexpr double SUB_LON = 0;
    static constexpr double COORD_ERROR = -999.999;
    /*
     * Those constants are explained here
     * https://github.com/pytroll/pyresample/issues/64
     */
    static constexpr double SQUARED_ELLIPSOID_FLATNESS = 1.006739501;
    static constexpr double DISTANCE_EARTH_SATELLITE = 42164.0;
    /*
     * At page 11 there is this constant is declared
     * https://www.data.jma.go.jp/mscweb/en/himawari89/space_segment/hsd_sample/HS_D_users_guide_en_v13.pdf
     */
    static constexpr double WGS84_CTE = 1737122264.0;
    static constexpr double EARTH_RADIUS = 6367.3764;
    static constexpr double DELTA_TIME = 10.0;

    static inline double degreeToRadian(double degrees) {
        static constexpr double tmp = M_PI / FLAT_DEG_ANGLE;
        return degrees * tmp;
    }

    static inline double radianToDegree(double radians) {
        static constexpr double tmp = FLAT_DEG_ANGLE / M_PI;
        return radians * tmp;
    }

    static double intermediateXFromPixelX(int x);

    static double intermediateYFromPixelY(int y);

    static double longitudeFromIntermediate(double x, double y);

    static double latitudeFromIntermediate(double x, double y);

    static double getSA(double x, double y);

    static double getSN(double sd, double x, double y);

    static double getS3(double sn, double y);

    static double getSXY(double sn, double x, double y);

    static double getS1(double sn, double x, double y);

    static double getS2(double sn, double x, double y);

    static double getAngularGap(double longitude, double latitude);

    static double getDistance(double angle);

    static double getSpeed(double diffLongitude, double diffLatitude);

    static double getOrientation(double diffLongitude, double diffLatitude);
};

#endif //TELE_CONVERT_HH
