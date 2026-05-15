# pragma once

# include <unordered_map>
# include <string>
# include <utility>
# include <vector>

# include "origin/builders.hpp"
#include "rotor/builders.hpp"
# include "stator/builders.hpp"
# include "airgap/builders.hpp"



class GroupNamer {

    const OriginBuilder& _ob;
    const StatorBuilders& _sb;
    const AirGapBuilders& _ab;
    const RotorBuilders& _rb;

    const std::unordered_map<std::string, std::vector<std::pair<int, int>>> _names; 

    public:
        GroupNamer(const OriginBuilder& ob, const StatorBuilders& sb, const AirGapBuilders& ab, const RotorBuilders& rb);
        void start_naming();

};