# include <gmsh.h>
# include "naming.hpp"
#include "rotor/builders.hpp"


namespace model = gmsh::model;


GroupNamer::GroupNamer(const OriginBuilder& ob, const StatorBuilders& sb, const AirGapBuilders& ab, const RotorBuilders& rb): 
    _ob(ob), _sb(sb), _ab(ab), _rb(rb), _names {

        {"origin", {_ob.p_origin()}},
        {"stator", _sb.s_stator_slotted()},
        {"airgap", {_ab.s_airgap()}},
        {"poles", {_rb.s_rotor_cut()}},
        {"yoke", {_rb.s_rotor_back()}}

    }
{}



void GroupNamer::start_naming() {

    for (const auto& [name, regions] : _names) {

        std::unordered_map<int, std::vector<int>> by_dim;
        for (const auto& [dim, tag] : regions) {
            by_dim[dim].push_back(tag);
        }

        for (const auto& [dim, tags] : by_dim) {
            int group = model::addPhysicalGroup(dim, tags);
            model::setPhysicalName(dim, group, name);
        }

    }

}


