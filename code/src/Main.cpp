#include <vector>
#include <iostream>
#include <algorithm>

#include "slp/Tree.hpp"
#include "slp/Good.hpp"



std::vector<struct vertex*> buildTestTree();
std::vector<struct vertex*> buildTestTree() {
    struct vertex* v0 = new struct vertex;
    struct vertex* v2 = new struct vertex;
    struct vertex* v8 = new struct vertex;
    struct vertex* v16 = new struct vertex;
    struct vertex* v10 = new struct vertex;
    struct vertex* v3 = new struct vertex;
    struct vertex* v13 = new struct vertex;
    struct vertex* v11 = new struct vertex;
    struct vertex* v7 = new struct vertex;
    struct vertex* v28 = new struct vertex;
    struct vertex* v29 = new struct vertex;

    /* Connect the vertices */
    v0->children.push_back(v2);
    v0->children.push_back(v8);
    v0->children.push_back(v16);
    v0->children.push_back(v10);

    v2->children.push_back(v3);
    v3->children.push_back(v7);

    v8->children.push_back(v13);
    v13->children.push_back(v28);
    v28->children.push_back(v29);

    v10->children.push_back(v11);

    /* Add variables to the sets */
    v0->z.push_back(1);
    v0->z.push_back(3);

    v2->m.push_back(2);
    v2->z.push_back(2);
    v2->z.push_back(3);
    v2->z.push_back(5);

    v8->m.push_back(4);
    v8->z.push_back(4);
    v8->z.push_back(1);
    v8->z.push_back(5);

    v16->m.push_back(5);
    v16->z.push_back(5);
    v16->z.push_back(1);
    v16->z.push_back(3);

    v10->m.push_back(2);
    v10->m.push_back(4);
    v10->z.push_back(2);
    v10->z.push_back(4);
    v10->z.push_back(1);
    v10->z.push_back(3);

    v3->m.push_back(1);
    v3->m.push_back(2);
    v3->z.push_back(1);
    v3->z.push_back(2);
    v3->z.push_back(4);
    v3->z.push_back(5);

    v13->m.push_back(3);
    v13->m.push_back(4);
    v13->z.push_back(3);
    v13->z.push_back(4);
    v13->z.push_back(1);

    v11->m.push_back(1);
    v11->m.push_back(2);
    v11->m.push_back(4);
    v11->z.push_back(1);
    v11->z.push_back(2);
    v11->z.push_back(4);
    v11->z.push_back(3);

    v7->m.push_back(1);
    v7->m.push_back(2);
    v7->m.push_back(3);
    v7->z.push_back(1);
    v7->z.push_back(2);
    v7->z.push_back(3);
    v7->z.push_back(5);

    v28->m.push_back(3);
    v28->m.push_back(4);
    v28->m.push_back(5);
    v28->z.push_back(3);
    v28->z.push_back(4);
    v28->z.push_back(5);
    v28->z.push_back(2);

    v29->m.push_back(1);
    v29->m.push_back(3);
    v29->m.push_back(4);
    v29->m.push_back(5);
    v29->z.push_back(1);
    v29->z.push_back(3);
    v29->z.push_back(4);
    v29->z.push_back(5);
    v29->z.push_back(2);

    std::vector<struct vertex*> vertices;
    vertices.push_back(v0);
    vertices.push_back(v2);
    vertices.push_back(v8);
    vertices.push_back(v16);
    vertices.push_back(v10);
    vertices.push_back(v3);
    vertices.push_back(v13);
    vertices.push_back(v11);
    vertices.push_back(v7);
    vertices.push_back(v28);
    vertices.push_back(v29); 

    return vertices;
}

int main() {
    ClpModel quad = clpFromTxt();

    struct vertex* root = construct(quad, 1, 10000, 1e-7);
    delete root;
}
