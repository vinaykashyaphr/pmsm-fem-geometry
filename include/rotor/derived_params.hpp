# pragma once

# include "params.hpp"



class DerivedRotorParams : public params::RotorParams {

    double _theta_p    = 0.0;
    double _r_mo       = 0.0;

    void compute_theta_p();
    void compute_r_mo();
    void compute();

    public:

        explicit DerivedRotorParams();

        double theta_p()    const;
        double r_mo()       const;

};


