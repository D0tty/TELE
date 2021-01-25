//
// Created by Thomas Michelot on 1/3/21.
//

#include "Convert.hh"

double Convert::intermediateXFromPixelX(int x) {
    return (x - COFF) / (pow(2, -16) * CFAC);
}

double Convert::intermediateYFromPixelY(int y) {
    return (y - LOFF) / (pow(2, -16) * LFAC);
}

double Convert::latitudeFromIntermediate(double x, double y) {
    double radX = degreeToRadian(x);
    double radY = degreeToRadian(y);
    double sa = getSA(radX, radY);

    if (sa < 0)
        return COORD_ERROR;

    double sn = getSN(sqrt(sa), radX, radY);
    double lat = SQUARED_ELLIPSOID_FLATNESS * (getS3(sn, radY) / getSXY(sn, radX, radY));

    return radianToDegree(atan(lat));
}

double Convert::longitudeFromIntermediate(double x, double y) {
    double radX = degreeToRadian(x);
    double radY = degreeToRadian(y);
    double sa = getSA(radX, radY);

    if (sa < 0)
        return COORD_ERROR;

    double sd = sqrt(sa);
    double sn = getSN(sd, x, y);
    double s1 = getS1(sn, x, y);
    double s2 = getS2(sn, x, y);

    return radianToDegree(atan(s2 / s1) + SUB_LON);
}

double Convert::getSA(double x, double y) {
    const double cosY = cos(y);

    const double tmp = (pow(cosY, 2.0) + SQUARED_ELLIPSOID_FLATNESS * pow(sin(y), 2.0)) * WGS84_CTE;

    return pow(DISTANCE_EARTH_SATELLITE * cos(x) * cosY, 2.0) - tmp;
}

double Convert::getSN(double sd, double x, double y) {
    double cosy = cos(y);

    return ((DISTANCE_EARTH_SATELLITE * cos(x) * cosy) - sd) /
           (pow(cosy, 2.0) + (SQUARED_ELLIPSOID_FLATNESS * pow(sin(y), 2.0)));
}

double Convert::getS3(double sn, double y) {
    return sn * sin(y);
}

double Convert::getSXY(double sn, double x, double y) {
    double s1 = getS1(sn, x, y);
    double s2 = getS2(sn, x, y);

    return sqrt(pow(s1, 2.0) + pow(s2, 2.0));
}

double Convert::getS1(double sn, double x, double y) {
    return DISTANCE_EARTH_SATELLITE - sn * cos(x) * cos(y);
}

double Convert::getS2(double sn, double x, double y) {
    return sn * sin(x) * cos(y);
}

double Convert::getAngularGap(double longitude, double latitude) {
    return acos(cos(longitude) * cos(latitude) +
                sin(longitude) * sin(latitude) *
                cos(M_PI_2));
}

double Convert::getDistance(double angle) {
    return angle * EARTH_RADIUS;
}

double Convert::getSpeed(double diffLongitude, double diffLatitude) {
    double angular = getAngularGap(diffLongitude, diffLatitude);

    return getDistance(angular) / DELTA_TIME;
}

double Convert::getOrientation(double diffLongitude, double diffLatitude) {
    double angular = getAngularGap(diffLongitude, diffLatitude);
    double a = cos(diffLatitude) - cos(diffLongitude) * cos(angular);
    double b = sin(diffLongitude) * sin(angular);

    return radianToDegree(acos(a / b));
}

double Convert::getSpeed(const GeoTaggedImage &image0, const GeoTaggedImage &image1) {
    auto longitude0 = getLongitude(image0);
    auto latitude0 = getLatitude(image0);
    auto longitude1 = getLongitude(image1);
    auto latitude1 = getLatitude(image1);

    return getSpeed(longitude1 - longitude0, latitude1 - latitude0);
}

double Convert::getOrientation(const GeoTaggedImage &image0, const GeoTaggedImage &image1) {
    auto longitude0 = getLongitude(image0);
    auto latitude0 = getLatitude(image0);
    auto longitude1 = getLongitude(image1);
    auto latitude1 = getLatitude(image1);

    return getOrientation(longitude1 - longitude0, latitude1 - latitude0);
}

double Convert::getLongitude(const GeoTaggedImage &image) {
    auto interX = Convert::intermediateXFromPixelX(image.coordinateXY_.getX());
    auto interY = Convert::intermediateYFromPixelY(image.coordinateXY_.getY());
    auto longitude = Convert::longitudeFromIntermediate(interX, interY);

    return longitude;
}

double Convert::getLatitude(const GeoTaggedImage &image) {
    auto interX = Convert::intermediateXFromPixelX(image.coordinateXY_.getX());
    auto interY = Convert::intermediateYFromPixelY(image.coordinateXY_.getY());
    auto latitude = Convert::latitudeFromIntermediate(interX, interY);

    return latitude;
}
