# pragma once

# include <string>
# include <utility>
# include <array>
# include <vector>

# include "params.hpp"
# include "rotor/derived_params.hpp"
# include "origin/builders.hpp"



class RotorBuilders {

    int _s_rotor_back = 0;
    int _s_rotor      = 0;
    int _s_polegap    = 0;
    std::vector<std::pair<int,int>> _s_rotor_cut{};

    std::array<int, 4> _polegap_vertices{};
    std::vector<std::pair<int, int>> _all_polegaps{};

    const params::OriginParams& _o;
    const OriginBuilder& _ob;
    const DerivedRotorParams&  _r;

    public:

        explicit RotorBuilders(const params::OriginParams& o, const OriginBuilder& ob, const DerivedRotorParams& r);

        void build_rotor_back_annulus();
        void build_rotor_pm_annulus();
        void build_polegap_vertices();
        void build_polegap_profile();
        void replicate_polegaps();
        void cut_polegap_profiles();
        void build();

        std::string field_yoke() const;
        std::string field_pm() const;

        std::vector<std::pair<int,int>> s_rotor_cut() const;
        std::pair<int, int> s_rotor_back() const;

};


