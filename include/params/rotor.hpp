# pragma once

# include "config/config.hpp"



class DerivedRotorConfig {

    const RotorConfig& _cfg;

    double _theta_p    = 0.0;
    double _r_mo       = 0.0;

    void compute_theta_p();
    void compute_r_mo();
    void compute();

    public:

        explicit DerivedRotorConfig(const RotorConfig& cfg);

        double theta_p()    const;
        double r_mo()       const;

};


