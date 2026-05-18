# pragma once

# include <array>
# include <string>

# include <gmsh.h>
# include <utility>

# include "preprocessor/params/config.hpp"
# include "preprocessor/params/stator.hpp"
# include "preprocessor/builders/origin.hpp"



class StatorBuilder {

    const Config& _cfg;
    const DerivedStatorConfig&  _scfg;
    const OriginBuilder& _obuilder;

    std::array<int, 8> _slot_vertices{};
    std::vector<std::pair<int, int>> _all_slots{};

    std::vector<std::pair<int,int>> _s_stator_slotted{};
    int _s_stator           = 0;
    int _s_slot             = 0;

    void build_stator_annulus();
    void build_slot_vertices();
    void build_slot_profile();
    void replicate_slots();
    void cut_slot_profiles();

    public:

        explicit StatorBuilder(const Config& cfg, const DerivedStatorConfig& scfg, const OriginBuilder& ob);
        void build();
        std::string field() const;

        std::vector<std::pair<int,int>> s_stator_slotted() const;

};


