#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

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
    std::vector<struct vertex*> vertices = buildTestTree();

    std::set<int16_t> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.insert(4);
    a.insert(5);

    std::set<int16_t> b;
    b.insert(1);
    b.insert(2);
    b.insert(3);

    //do {
    //    print(b.begin(), b.end());
    //} while(next_combination(a.begin(), a.end(), b.begin(), b.end()));

    std::set<int16_t> c = complement(b, 6);
    print(c.begin(), c.end());



    double arr[5];
    arr[0] = 1.323;
    arr[1] = 0;
    arr[2] = 0;
    arr[3] = 4.323;
    arr[4] = 0.00001;

    std::set<int16_t> z = toZSet(arr, 5, 1e-7);
    print(z.begin(), z.end());

    std::cout << "end" << std::endl;
}
