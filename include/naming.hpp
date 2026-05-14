# pragma once

# include <unordered_map>
# include <string>
# include <utility>

# include "stator/builders.hpp"
# include "airgap/builders.hpp"



class GroupNamer {

    StatorBuilders& _sb;
    AirGapBuilders& _ab;

    const std::unordered_map<std::string, std::pair<int, int>> _names; 

    public:
        GroupNamer(StatorBuilders& sb, AirGapBuilders& ab);
        void start_naming();

};