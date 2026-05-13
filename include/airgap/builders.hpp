# pragma once

# include <string>

# include "params.hpp"



class AirGapBuilders {

    int _s_airgap = 0;

    const StatorParams&  _s;
    const AirGapParams&  _a;
    const RotorParams&   _r;

    public:
        explicit AirGapBuilders(const StatorParams& s, const AirGapParams& a, const RotorParams& r);
        void build();
        std::string field() const;

};


