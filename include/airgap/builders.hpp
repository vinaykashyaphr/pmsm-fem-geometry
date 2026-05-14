# pragma once

# include <string>
# include <utility>

# include "params.hpp"



class AirGapBuilders {

    int _s_airgap = 0;

    const params::StatorParams&  _s;
    const params::AirGapParams&  _a;
    const params::RotorParams&   _r;

    public:

        explicit AirGapBuilders(
            const params::StatorParams& s, 
            const params::AirGapParams& a, 
            const params::RotorParams& r
        );

        void build();
        std::string field() const;

        std::pair<int, int> s_airgap() const;

};


