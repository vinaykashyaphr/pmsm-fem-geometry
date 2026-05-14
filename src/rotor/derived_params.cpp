# include <iostream>

# include "rotor/derived_params.hpp"



DerivedRotorParams::DerivedRotorParams() {

    compute();

}



void DerivedRotorParams::compute_theta_p() {

    const double theta_p = (2.0 * std::numbers::pi) / n_m;
    std::cout << "Pitch angle (theta_p): " << theta_p << '\n';

    _theta_p = theta_p;

}



void DerivedRotorParams::compute_r_mo() {

    const double r_mo = r_ri + l_m;
    std::cout << "Rotor pole outer radius (r_mo)" << r_mo << '\n';

    _r_mo = r_mo;

}



void DerivedRotorParams::compute() {

    compute_theta_p();
    compute_r_mo();

}



double DerivedRotorParams::theta_p() const {
    return _theta_p;
}



double DerivedRotorParams::r_mo() const {
    return _r_mo;
}


