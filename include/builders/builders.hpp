# pragma once

# include "builders/stator.hpp"
# include "builders/airgap.hpp"
# include "builders/rotor.hpp"



struct Builders {

    const OriginBuilder obuilder;
    const StatorBuilder sbuilder;
    const AirGapBuilder abuilder;
    const RotorBuilder  rbuilder;

};


