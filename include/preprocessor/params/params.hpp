# pragma once

# include "preprocessor/params/config.hpp"
# include "preprocessor/params/stator.hpp"
# include "preprocessor/params/rotor.hpp"



struct Params {

    const Config cfg;
    const DerivedStatorConfig scfg;
    const DerivedRotorConfig  rcfg;
    const FarFieldConfig fcfg;

    Params(
        const Config& c, 
        const DerivedStatorConfig& s, 
        const DerivedRotorConfig& r,
        const FarFieldConfig& f
    ):
        cfg(c), scfg(s), rcfg(r), fcfg(f)
    {}

};


