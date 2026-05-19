# pragma once

# include "preprocessor/params/config.hpp"
# include "preprocessor/params/stator.hpp"
# include "preprocessor/params/rotor.hpp"



struct Params {

    const Config cfg;
    const DerivedStatorConfig scfg;
    const DerivedRotorConfig  rcfg;

    Params(const Config& c, const DerivedStatorConfig& s, const DerivedRotorConfig& r):
        cfg(c), scfg(s), rcfg(r)
    {}

};


