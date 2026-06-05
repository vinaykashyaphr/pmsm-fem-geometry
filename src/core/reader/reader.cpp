# include <cstddef>
# include <string>
# include <unordered_map>
# include <vector>
# include <gmsh.h>
# include <iostream>

# include "core/reader/reader.hpp"
# include "core/reader/mesh.hpp"


namespace model = gmsh::model;



Region MeshReader::region_from_name(const std::string& name) {
    if (name == "stator")   return Region::Stator;
    if (name == "airgap")   return Region::Airgap;
    if (name == "rotor")    return Region::Rotor;
    if (name == "pole")     return Region::Pole;
    if (name == "yoke")     return Region::Yoke;
    if (name == "farfield") return Region::Farfield;
    throw std::runtime_error("Unknown region: " + name);
}



Mesh MeshReader::read() {

    Mesh mesh;

    std::vector<std::size_t> node_tags;
    std::vector<double> coords, param_coords;
    model::mesh::getNodes(node_tags, coords, param_coords);

    // {tag, node}
    std::unordered_map<std::size_t, std::size_t> tag_to_idx;
    for (std::size_t i = 0; i < node_tags.size(); ++i) {
        tag_to_idx[node_tags.at(i)] = static_cast<std::size_t>(i);
        mesh.nodes.push_back({coords.at(3*i), coords.at(3*i + 1)});
    }

    std::cout << "Nodes: " << mesh.nodes.size() << '\n';

    std::vector<std::pair<int, int>> phys_groups;
    model::getPhysicalGroups(phys_groups, 2);

    for (const auto& [dim, phys_group] : phys_groups) {
        std::string group_name;
        model::getPhysicalName(dim, phys_group, group_name);
        Region region = region_from_name(group_name);

        std::vector<int> entities;
        model::getEntitiesForPhysicalGroup(dim, phys_group, entities);

        for (int entity : entities) {

            std::vector<std::size_t> elem_tags, elem_node_tags;

            // 2 = triangle
            model::mesh::getElementsByType(2, elem_tags, elem_node_tags);

            for (std::size_t j = 0; j < elem_tags.size(); ++j) {

                Element elem;
                elem.region = region;

                elem.nodes.at(0) = tag_to_idx.at(elem_node_tags.at(3 * j));
                elem.nodes.at(1) = tag_to_idx.at(elem_node_tags.at(3 * j + 1));
                elem.nodes.at(2) = tag_to_idx.at(elem_node_tags.at(3 * j + 2));

                mesh.elements.push_back(elem);

            }

        }

    }

    std::cout << "Elements: " << mesh.elements.size() << '\n';

    std::vector<std::pair<int,int>> boundary_groups;
    gmsh::model::getPhysicalGroups(boundary_groups, 1);

    for (const auto& [dim, phys_tag] : boundary_groups) {

        std::string name;
        gmsh::model::getPhysicalName(dim, phys_tag, name);

        if (name == "farfield") {

            std::vector<int> entities;
            gmsh::model::getEntitiesForPhysicalGroup(dim, phys_tag, entities);

            for (int entity : entities) {
                std::vector<std::size_t> b_node_tags;
                std::vector<double> b_coords;
                gmsh::model::mesh::getNodes(b_node_tags, b_coords, param_coords, 1, entity);

                for (std::size_t t : b_node_tags) {
                    mesh.boundary_nodes.push_back(tag_to_idx.at(t));
                }
            }
        }
    }

    std::cout << "Boundary nodes: " << mesh.boundary_nodes.size() << '\n';

    return mesh;

}