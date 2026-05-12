# include "stator/derived_params.hpp"

# include <cmath>
# include <iostream>



DerivedStatorParams::DerivedStatorParams() {

    compute();

}



void DerivedStatorParams::compute_theta_p() {

    const double theta_p = (2.0 * std::numbers::pi) / n_s;
    std::cout << "Pitch angle (theta_p): " << theta_p << '\n';

    _theta_p = theta_p;

}



void DerivedStatorParams::compute_theta_o() {

    const double theta_o = 2.0 * asin(w_s / (2.0 * r_so));
    std::cout << "Slot opening angle (theta_o): " << theta_o << '\n';

    _theta_o = theta_o;

}



void DerivedStatorParams::compute_theta_s() {

    const double theta_s = _theta_p - _theta_o;
    std::cout << "Tooth tip angle (theta_s): " << theta_s << '\n';

    _theta_s = theta_s;

}



void DerivedStatorParams::compute_r_sm() {

    const double r_sm = r_so - d1;
    std::cout << "Tooth mid radius (r_sm): " << r_sm << '\n';

    _r_sm = r_sm;

}



void DerivedStatorParams::compute_c_sm() {

    const double c_sm = 2.0 * _r_sm * sin(_theta_s / 2.0);
    std::cout << "Tooth mid chord (c_sm): " << c_sm << '\n';

    _c_sm = c_sm;

}



void DerivedStatorParams::compute_delta_x() {

    const double delta_x = 0.5 * (_c_sm - w_tb);
    std::cout << "Horizontal dist b/w edge of tooth body to edge of tooth mid (delta_x): " << delta_x << '\n';

    _delta_x = delta_x;

}



void DerivedStatorParams::compute_delta_y() {

    const double delta_y = _delta_x * tan(d2);
    std::cout << "Verical dist b/w stator neck to tooth mid (delta_y): " << delta_y << '\n';

    _delta_y = delta_y;

}



void DerivedStatorParams::compute_r_sn() {

    const double r_sn = r_so - d1 - _delta_y;
    std::cout << "Stator neck radius (r_sn): " << r_sn << '\n';

    _r_sn = r_sn;

}



void DerivedStatorParams::compute_theta_sn() {

    const double theta_sn = 2.0 * asin(w_tb / (2.0 * _r_sn));
    std::cout << "Stator neck angle (theta_sn): " << theta_sn << '\n';

    _theta_sn = theta_sn;

}



void DerivedStatorParams::compute_r_sb() {

    const double r_sb = w_tb + r_si;
    std::cout << "Stator base radius (r_sb): " << r_sb << '\n';

    _r_sb = r_sb;

}



void DerivedStatorParams::compute_theta_sb() {

    const double theta_sb = 2.0 * asin(w_tb / (2.0 * _r_sb));
    std::cout << "Stator base angle (theta_sb): " << theta_sb << '\n';

    _theta_sb = theta_sb;

}



void DerivedStatorParams::compute() {

    compute_theta_p();
    compute_theta_o();
    compute_theta_s();
    compute_r_sm();
    compute_c_sm();
    compute_delta_x();
    compute_delta_y();
    compute_r_sn();
    compute_theta_sn();
    compute_r_sb();
    compute_theta_sb();

}



double DerivedStatorParams::theta_p() const {
    return _theta_p;
};



double DerivedStatorParams::theta_o() const {
    return _theta_o;
};



double DerivedStatorParams::theta_s() const {
    return _theta_s;
};



double DerivedStatorParams::r_sm() const {
    return _r_sm;
};



double DerivedStatorParams::c_sm() const {
    return _c_sm;
};



double DerivedStatorParams::delta_x() const {
    return _delta_x;
};



double DerivedStatorParams::delta_y() const {
    return _delta_y;
};



double DerivedStatorParams::r_sn() const {
    return _r_sn;
};



double DerivedStatorParams::theta_sn() const {
    return _theta_sn;
};



double DerivedStatorParams::r_sb() const {
    return _r_sb;
};



double DerivedStatorParams::theta_sb() const {
    return _theta_sb;
};


