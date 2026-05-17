# pragma once

# include <unordered_map>
# include <string>
# include <utility>
# include <vector>

# include "builders/builders.hpp"



class GroupNamer {

    const Builders& _builders;


    const std::unordered_map<std::string, std::vector<std::pair<int, int>>> _names; 

    public:
        GroupNamer(const Builders& builders);
        void start_naming();

};