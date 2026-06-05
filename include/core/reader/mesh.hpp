# pragma once

# include <array>
# include <vector>



enum class Region {
    Origin,
    Stator,
    Airgap,
    Pole,
    Rotor,
    Yoke,
    Farfield
};


struct Node {
    double x, y;
};



struct Element {
    std::array<int, 3> nodes;
    Region region;
};



struct Mesh {
    std::vector<Node> nodes;
    std::vector<Element> elements;
    std::vector<int> boundary_nodes;
};
