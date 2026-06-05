# pragma once

# include "preprocessor/params/params.hpp"
# include "preprocessor/builders/origin.hpp"
# include "preprocessor/builders/stator.hpp"
# include "preprocessor/builders/airgap.hpp"
# include "preprocessor/builders/rotor.hpp"
# include "preprocessor/builders/farfield.hpp"



struct Builders {

    const OriginBuilder obuilder;
    const StatorBuilder sbuilder;
    const AirGapBuilder abuilder;
    const RotorBuilder  rbuilder;
    const FarFieldBuilder fbuilder;

    Builders(const Params& params):
        obuilder(params.cfg.origin),
        sbuilder(params.cfg, params.scfg, obuilder.p_origin().second),
        abuilder(params.cfg),
        rbuilder(params.cfg, params.rcfg, obuilder.p_origin().second),
        fbuilder(params.cfg)
    {}

};


