# pragma once

# include <string>
# include <utility>

# include "config/config.hpp"



class AirGapBuilder {

    int _s_airgap = 0;

    const Config& _cfg;

    public:

        explicit AirGapBuilder(const Config& cfg);

        void build();
        std::string field() const;

        std::pair<int, int> s_airgap() const;

};


