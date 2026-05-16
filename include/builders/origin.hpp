# pragma once

# include <utility>

# include "config/config.hpp"



class OriginBuilder {

    const OriginConfig& _cfg;

    int _p_origin = 0;

    public:
        explicit OriginBuilder(const OriginConfig& cfg);
        void build();
        std::pair<int, int> p_origin() const;

};


