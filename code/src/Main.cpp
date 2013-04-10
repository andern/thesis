#include <vector>
#include <iostream>

#include "slp/Tree.hpp"

void printset(const std::set<short>& s);

void printset(const std::set<short>& s) {
    for (std::set<short>::iterator it=s.begin(); it!=s.end(); ++it)
        std::cout << *it;
    std::cout << "\n";
}

//void buildTestTree(std::vector<struct vertex*>& vertices);
//void buildTestTree(std::vector<struct vertex*>& vertices) {
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

    std::cout << "end of declarations" << std::endl;

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
    v0->z.insert(1);
    v0->z.insert(3);

    v2->m.insert(2);
    v2->z.insert(2);
    v2->z.insert(3);
    v2->z.insert(5);

    v8->m.insert(4);
    v8->z.insert(4);
    v8->z.insert(1);
    v8->z.insert(5);

    v16->m.insert(5);
    v16->z.insert(5);
    v16->z.insert(1);
    v16->z.insert(3);

    v10->m.insert(2);
    v10->m.insert(4);
    v10->z.insert(2);
    v10->z.insert(4);
    v10->z.insert(1);
    v10->z.insert(3);

    v3->m.insert(1);
    v3->m.insert(2);
    v3->z.insert(1);
    v3->z.insert(2);
    v3->z.insert(4);
    v3->z.insert(5);

    v13->m.insert(3);
    v13->m.insert(4);
    v13->z.insert(3);
    v13->z.insert(4);
    v13->z.insert(1);

    v11->m.insert(1);
    v11->m.insert(2);
    v11->m.insert(4);
    v11->z.insert(1);
    v11->z.insert(2);
    v11->z.insert(4);
    v11->z.insert(3);

    v7->m.insert(1);
    v7->m.insert(2);
    v7->m.insert(3);
    v7->z.insert(1);
    v7->z.insert(2);
    v7->z.insert(3);
    v7->z.insert(5);

    v28->m.insert(3);
    v28->m.insert(4);
    v28->m.insert(5);
    v28->z.insert(3);
    v28->z.insert(4);
    v28->z.insert(5);
    v28->z.insert(2);

    v29->m.insert(1);
    v29->m.insert(3);
    v29->m.insert(4);
    v29->m.insert(5);
    v29->z.insert(1);
    v29->z.insert(3);
    v29->z.insert(4);
    v29->z.insert(5);
    v29->z.insert(2);

    std::cout << "before vertices" << std::endl;

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

    std::cout << "end of build" << std::endl;
    return vertices;
}

int main() {
    std::vector<struct vertex*> vertices = buildTestTree();
    struct vertex* v = vertices[1];

    printset(v->z);

    std::set<short> ml;
    ml.insert(1);
    ml.insert(2);
    ml.insert(3);
    ml.insert(4);
    std::cout << "End of main" << std::endl;
}
