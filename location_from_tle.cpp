#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <ctime>

#define mu 2.975537e15
#define PI 3.141592653589793
#define EARTH_RADI 6378.137

double kepler(double e, double M, double E);
double kepler_dif(double e, double E);
double Newton_Raphson(double ini, double e, double M);

int main(void)
{

    // TLE data

    int linenum1;
    int catalog_num1;
    char satclass;
    int designator;
    char designator_piece;
    double epoch;
    int epoch_year;
    double epoch_day;
    double first_deriv;
    double second_deriv;
    float second_deriv_coef;
    char second_deriv_sign;
    int second_deriv_index;
    double bstar;
    float bstar_coef;
    char bstar_sign;
    int bstar_index;
    int ephemeris;
    int element_check;
    int element;
    int check1;
    int linenum2;
    int catalog_num2;
    double inclination;
    double inclination_rad;
    double ascension;
    double ascension_now_deg;
    double ascension_now_rad;
    double eccentricity;
    double eccentric_anomaly_rad;
    double eccentric_anomaly_deg;
    double perigee;
    double perigee_now_deg;
    double perigee_now_rad;
    double anomaly;
    double anomaly_now;
    double motion;
    double motion_now;
    int revolutions_check;
    int revolutions;
    int check2;
    double coordinate_U;
    double coordinate_V;
    double coordinate_x;
    double coordinate_y;
    double coordinate_z;
    double coordinate_lX;
    double coordinate_lY;
    double coordinate_lZ;
    double JD;
    int JD_year;
    int JD_mon;
    double TJD;
    double sidereal;
    double latitude;
    double longitude;

    double long_radius;

    double now_day = 0;
    float month_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    std::ifstream TLEfile("QZS-2_TLE.txt");

    if (!TLEfile.is_open()) {
        std::cout << "Error! Can't open the file.\n";
        return 0;
    }

    TLEfile >> linenum1;
    TLEfile >> catalog_num1;
    TLEfile >> satclass;
    TLEfile >> designator;
    TLEfile >> designator_piece;
    TLEfile >> epoch;
    TLEfile >> first_deriv;
    TLEfile >> second_deriv_coef;
    TLEfile >> second_deriv_sign;
    TLEfile >> second_deriv_index;
    TLEfile >> bstar_coef;
    TLEfile >> bstar_sign;
    TLEfile >> bstar_index;
    TLEfile >> ephemeris;
    TLEfile >> element_check;
    TLEfile >> linenum2;
    TLEfile >> catalog_num2;
    TLEfile >> inclination;
    TLEfile >> ascension;
    TLEfile >> eccentricity;
    TLEfile >> perigee;
    TLEfile >> anomaly;
    TLEfile >> motion;
    TLEfile >> revolutions_check;

    epoch_year = floor(epoch/1000 + 2000);
    epoch_day = epoch - floor(epoch/1000)*1000;

    first_deriv *= 2;

    second_deriv = second_deriv_coef;

    for (int i = 0; i < second_deriv_index; i++) {
        second_deriv /= second_deriv;
    }

    if (second_deriv_sign == '-') {
        second_deriv *= -1;
    }

    second_deriv *= 6;
    
    bstar = bstar_coef;

    for (int i = 0; i < bstar_index; i++) {
        bstar /= bstar;
    }

    if (bstar_sign == '-') {
        bstar *= -1;
    }

    element = floor(element_check/10);

    check1= element_check - floor(element_check/10)*10;

    eccentricity /= 10000000;

    revolutions = floor(revolutions_check/10);

    check2 = revolutions_check - floor(revolutions_check/10)*10;

//*************************************************************************

    time_t current = time(NULL);
    struct tm* timer = localtime(&current);
    struct tm* UT = gmtime(&current);

    for (int i = 0; i < timer->tm_mon; i++){
        now_day += (double)month_day[i];
    }

    now_day += (double)timer->tm_mday -1.0 + (double)timer->tm_hour /24 + (double)timer->tm_min / 24 / 60 + (double)timer->tm_sec / 24 / 60 / 60; 

    if ((timer->tm_year +1900)%4 == 0 && (timer->tm_year +1900)%100 != 0 || (timer->tm_year +1900)%400 == 0) {
        now_day += 1;
    }

    //********************* for validation *******************************

    // epoch_day = 120.72277529;
    // perigee = 14.7699;
    // inclination = 98.2104;
    // ascension = 195.1270;
    // eccentricity = 0.0001679;
    // anomaly = 345.3549;
    // motion = 14.59544429;
    // first_deriv = 0.00000232;
    // now_day = 135 + 2.0/24;

    //********************* for validation end ***************************

    motion_now = motion + first_deriv * (now_day - epoch_day);

    long_radius = cbrt(mu / (4 * pow(PI, 2) * pow(motion_now, 2)));

    anomaly_now = anomaly/360.0 + motion*(now_day - epoch_day) + first_deriv/2.0*(now_day - epoch_day)*(now_day - epoch_day);
    anomaly_now = (anomaly_now - floor(anomaly_now)) * 360.0;     // rev -> deg conversion

    eccentric_anomaly_rad = Newton_Raphson(100.0, eccentricity, anomaly_now);
    eccentric_anomaly_deg = eccentric_anomaly_rad / (2 * PI) * 360;

    coordinate_U = long_radius * cos(eccentric_anomaly_rad) - long_radius * eccentricity;
    coordinate_V = long_radius * sqrt(1 - eccentricity * eccentricity) * sin(eccentric_anomaly_rad);

    inclination_rad = inclination/360*2*PI;

    perigee_now_deg = perigee + (180.0 * 0.174 * (2.0-2.5*sin(inclination_rad)*sin(inclination_rad)))/(PI*pow(long_radius/EARTH_RADI, 3.5)) * (now_day - epoch_day);
    ascension_now_deg = ascension - (180.0 * 0.174 * cos(inclination_rad))/(PI * pow(long_radius/EARTH_RADI, 3.5)) * (now_day - epoch_day);

    perigee_now_rad = perigee_now_deg/360*2*PI;
    ascension_now_rad = ascension_now_deg/360*2*PI;

    coordinate_x = (cos(ascension_now_rad) * cos(perigee_now_rad) - sin(ascension_now_rad) * cos(inclination_rad) * sin(perigee_now_rad))*coordinate_U + (-cos(ascension_now_rad) * sin(perigee_now_rad) - sin(ascension_now_rad) * cos(inclination_rad) * cos(perigee_now_rad))*coordinate_V;
    coordinate_y = (sin(ascension_now_rad) * cos(perigee_now_rad) + cos(ascension_now_rad) * cos(inclination_rad) * sin(perigee_now_rad))*coordinate_U + (-sin(ascension_now_rad) * sin(perigee_now_rad) + cos(ascension_now_rad) * cos(inclination_rad) * cos(perigee_now_rad))*coordinate_V;
    coordinate_z = (sin(inclination_rad) * sin(perigee_now_rad))*coordinate_U + (sin(inclination_rad) * cos(perigee_now_rad))*coordinate_V;

    JD_year = (UT->tm_year) + 1900;
    JD_mon = (UT->tm_mon) + 1;

    if (JD_mon == 1) {
        JD_year--;
        JD_mon = 13;
    } else if (JD_mon == 2) {
        JD_year--;
        JD_mon = 14;
    }

    JD = floor(365.25*JD_year) + floor(JD_year/400) - floor(JD_year/100) + floor(30.59*(JD_mon-2)) + UT->tm_mday + 1721088.5 + (UT->tm_hour)/24.0 + (UT->tm_min)/1440.0 + (UT->tm_sec)/86400.0;
    //JD = floor(365.25*(2006)) + floor(2006/400) - floor(2006/100) + floor(30.59*(5-2)) + 15 + 1721088.5 + (2)/24.0 + (0)/1440.0 + (0)/86400.0;
    TJD = JD - 2440000.5;
    sidereal = 0.671262 + 1.0027379094 * TJD;
    sidereal = sidereal - floor(sidereal);

    coordinate_lX = cos(-2*PI*sidereal)*coordinate_x - sin(-2*PI*sidereal)*coordinate_y;
    coordinate_lY = sin(-2*PI*sidereal)*coordinate_x + cos(-2*PI*sidereal)*coordinate_y;
    coordinate_lZ = coordinate_z;

    latitude = asin(coordinate_lZ/sqrt(coordinate_lX*coordinate_lX + coordinate_lY*coordinate_lY + coordinate_lZ*coordinate_lZ)) * 360/(2*PI);
    longitude = atan2(coordinate_lY, coordinate_lX) * 360/(2*PI);

    std::cout << latitude << '\n' << longitude << '\n';

    return 0;
}

double kepler(double e, double M, double E)
{
    double M_rad;
    M_rad = M/360*2*PI;

    return E - e*sin(E) - M_rad;
}

double kepler_dif(double e, double E)
{
    return 1 - e*cos(E);
}

double Newton_Raphson(double ini, double e, double M)
{
    double E;

    E = ini;

    while (fabs(kepler(e, M, E)) > 0.00000000001) {
        E = E - kepler(e, M, E) / kepler_dif(e, E);
    }

    return E;
}
