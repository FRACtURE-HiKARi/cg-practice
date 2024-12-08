#include "objects.h"

Grid standardCube() {
    Grid m;
    m.V.reserve(8);
    m.E.reserve(12);
    m.V = {
            {0, 0, 0, 1}, // 0
            {1, 0, 0, 1}, // 1
            {1, 1, 0, 1}, // 2
            {0, 1, 0, 1}, // 3
            {0, 1, 1, 1}, // 4
            {1, 1, 1, 1}, // 5
            {1, 0, 1, 1}, // 6
            {0, 0, 1, 1}, // 7
    };
    m.E = {
            // bottom
            std::make_pair(0, 1),
            std::make_pair(1, 2),
            std::make_pair(2, 3),
            std::make_pair(3, 0),
            // side
            std::make_pair(0, 7),
            std::make_pair(1, 6),
            std::make_pair(2, 5),
            std::make_pair(3, 4),
            // top
            std::make_pair(4, 5),
            std::make_pair(5, 6),
            std::make_pair(6, 7),
            std::make_pair(7, 4),
    };
    return m;
}

Mesh simpleTetra() {
    Mesh t;
    t.V.reserve(4);
    t.S.reserve(4);
    t.V = {
            {0, 0, 0, 1},
            {1, 1, 0, 1},
            {1, 0, 1, 1},
            {0, 1, 1, 1}
    };
    t.S = {
            {0, 1, 2},
            {0, 1, 3},
            {0, 2, 3},
            {1, 2, 3}
    };
    return t;
}
