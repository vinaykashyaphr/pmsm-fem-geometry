# pragma once

# include <array>
# include <numbers>




class StatorParams {

    public:

        /// Origin for cartesian coordinate
        const std::array<double, 3> origin  = {0.0, 0.0, 0.0};

        // Stator outer radius
        const double r_so                   = 80.0;

        // Stator inner radius
        const double r_si                   = 27.0;
        
        // Stator tooth width
        const double w_tb                   = 11.0;
        
        // Slot opening width
        const double w_s                    = 2.8;
        
        // Slot shoe depth
        const double d1                     = 5.0;
        
        // Slot shoe ramp angle 
        const double d2                     = deg2rad(15);
        
        // Number of Slots
        const int n_s                       = 18;


    constexpr double deg2rad(double degrees) {
        return degrees * (std::numbers::pi / 180.0);
    }

};
