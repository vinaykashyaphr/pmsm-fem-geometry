# include <gmsh.h>
# include <iostream>

# include "mesh_builder.hpp"


namespace mesh = gmsh::model::mesh;
namespace option = gmsh::option;



MeshBuilder::MeshBuilder() {

    std::cout << "Meshing ..." << '\n';

}; 



void MeshBuilder::add_region(const std::string expr) {

    int f = mesh::field::add("MathEval");
    mesh::field::setString(f, "F", expr);

    _field_ids.push_back(f);

}



void MeshBuilder::generate() {

    option::setNumber("Mesh.CharacteristicLengthFromPoints", 0);
    option::setNumber("Mesh.CharacteristicLengthFromCurvature", 0);
    option::setNumber("Mesh.CharacteristicLengthExtendFromBoundary", 0);
    option::setNumber("Mesh.CharacteristicLengthMin", 0.05);
    option::setNumber("Mesh.CharacteristicLengthMax", 3.0);

    int f_min = mesh::field::add("Min");

    std::vector<double> ids;
    for (int id : _field_ids) ids.push_back(static_cast<double>(id));

    mesh::field::setNumbers(f_min, "FieldsList", ids);
    mesh::field::setAsBackgroundMesh(f_min);

    mesh::generate(2);

}


