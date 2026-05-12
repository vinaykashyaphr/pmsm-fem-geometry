# pragma once

# include "stator/params.hpp"



class DerivedStatorParams : public StatorParams {

    double _theta_p    = 0.0;
    double _theta_o    = 0.0;
    double _theta_s    = 0.0;
    double _r_sm       = 0.0;
    double _c_sm       = 0.0;
    double _delta_x    = 0.0;
    double _delta_y    = 0.0;
    double _r_sn       = 0.0;
    double _theta_sn   = 0.0;
    double _r_sb       = 0.0;
    double _theta_sb   = 0.0;

    void compute_theta_p();
    void compute_theta_o();
    void compute_theta_s();
    void compute_r_sm();
    void compute_c_sm();
    void compute_delta_x();
    void compute_delta_y();
    void compute_r_sn();
    void compute_theta_sn();
    void compute_r_sb();
    void compute_theta_sb();

    void compute();


    public:

        explicit DerivedStatorParams();

        double theta_p()    const;
        double theta_o()    const;
        double theta_s()    const;
        double r_sm()       const;
        double c_sm()       const;
        double delta_x()    const;
        double delta_y()    const;
        double r_sn()       const;
        double theta_sn()   const;
        double r_sb()       const;
        double theta_sb()   const;

};
