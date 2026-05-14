# pragma once

# include <array>
# include <string>

# include <gmsh.h>
# include <utility>

# include "params.hpp"
# include "stator/derived_params.hpp"
# include "origin/builders.hpp"



class StatorBuilders {

    const DerivedStatorParams&  _s;
    const params::OriginParams& _o;
    const OriginBuilder& _ob;

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

        explicit StatorBuilders(const DerivedStatorParams& s, const params::OriginParams& o, const OriginBuilder& ob);
        void build();
        std::string field() const;

        std::vector<std::pair<int,int>> s_stator_slotted() const;

};


