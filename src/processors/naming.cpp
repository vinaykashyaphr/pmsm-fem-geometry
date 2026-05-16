# include <gmsh.h>
# include "processors/naming.hpp"
# include "config/config.hpp"
# include "builders/rotor.hpp"


namespace model = gmsh::model;


GroupNamer::GroupNamer(const Builders& builders): 
    _builders(builders), _names {
        {Config::tag.origin, {builders.obuilder.p_origin()}},
        {Config::tag.stator, {builders.sbuilder.s_stator_slotted()}},
        {Config::tag.airgap, {builders.abuilder.s_airgap()}},
        {Config::tag.pole,   {builders.rbuilder.s_rotor_cut()}},
        {Config::tag.yoke,   {builders.rbuilder.s_rotor_back()}}
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


