# pragma once

# include "preprocessor/builders/stator.hpp"
# include "preprocessor/builders/airgap.hpp"
# include "preprocessor/builders/rotor.hpp"



struct Builders {

    const OriginBuilder obuilder;
    const StatorBuilder sbuilder;
    const AirGapBuilder abuilder;
    const RotorBuilder  rbuilder;

};


