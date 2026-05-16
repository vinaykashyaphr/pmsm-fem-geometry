# pragma once

# include "config/config.hpp"
# include "params/stator.hpp"
# include "params/rotor.hpp"
# include "builders/stator.hpp"
# include "builders/airgap.hpp"
# include "builders/rotor.hpp"



struct Params {

    const Config              cfg;
    const DerivedStatorConfig scfg;
    const DerivedRotorConfig  rcfg;

};



struct Builders {

    const OriginBuilder obuilder;
    const StatorBuilder sbuilder;
    const AirGapBuilder abuilder;
    const RotorBuilder  rbuilder;

};


