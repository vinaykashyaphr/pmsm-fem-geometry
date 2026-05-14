# pragma once

# include <string>
# include <utility>

# include "params.hpp"



class RotorBuilders {

    int _s_airgap = 0;

    const params::RotorParams& _r;

    public:
        explicit RotorBuilders(const params::RotorParams& r);
        void build();
        std::string field() const;

        std::pair<int, int> s_airgap() const;

};


