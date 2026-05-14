# include <gmsh.h>
# include "naming.hpp"


namespace model = gmsh::model;


GroupNamer::GroupNamer(StatorBuilders& sb, AirGapBuilders& ab): 
    _sb(sb), _ab(ab), _names {

        {"origin", _sb.p_origin()},
        {"stator", _sb.s_stator_slotted()},
        {"airgap", _ab.s_airgap()}

    }
{}



void GroupNamer::start_naming() {

    for (const auto& [name, attrib] : _names) {

        int group = model::addPhysicalGroup(attrib.first, {attrib.second});
        model::setPhysicalName(attrib.first, group, name);

    }

}


