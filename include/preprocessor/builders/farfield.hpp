# pragma once

# include <string>
# include <utility>

# include "preprocessor/params/config.hpp"



class FarFieldBuilder {

    int _s_farfield = 0;

    const Config& _cfg;

    public:

        explicit FarFieldBuilder(const Config& rcfg);

        void build();
        std::string field() const;

        std::pair<int, int> s_farfield() const;

};

