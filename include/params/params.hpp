# pragma once

# include "params/config.hpp"
# include "params/stator.hpp"
# include "params/rotor.hpp"



struct Params {

    const Config              cfg;
    const DerivedStatorConfig scfg;
    const DerivedRotorConfig  rcfg;

};


