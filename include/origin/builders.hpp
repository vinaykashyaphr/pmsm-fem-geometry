# pragma once

# include <utility>

# include "params.hpp"



class OriginBuilder {

    const params::OriginParams& _o;

    int _p_origin = 0;

    public:
        explicit OriginBuilder(const params::OriginParams& o);
        void build();
        std::pair<int, int> p_origin() const;

};


