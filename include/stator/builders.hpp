# pragma once

# include <array>
# include <gmsh.h>

# include "stator/derived_params.hpp"



class StatorBuilders {


    DerivedStatorParams _params;

    std::array<int, 8> _slot_vertices{};
    std::vector<std::pair<int, int>> _all_slots{};

    int _p_origin           = 0;
    int _s_stator           = 0;
    int _s_slot             = 0;
    int _s_stator_slotted   = 0;

    void build_origin();
    void build_stator_annulus();
    void build_slot_vertices();
    void build_slot_profile();
    void replicate_slots();
    void cut_slot_profiles();

    void build();


    public:
        explicit StatorBuilders();

};