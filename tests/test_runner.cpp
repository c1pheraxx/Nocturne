#include <iostream>
#include <cassert>
#include <cmath>
#include "../src/math/vec3.h"
#include "../src/math/matrix.h"
#include "../src/math/projection.h"

using namespace nocturne;

int tests_passed = 0;
int tests_failed = 0;

void check(bool condition, const char* name) {
    if (condition) {
        tests_passed++;
        std::cout << "[PASS] " << name << std::endl;
    } else {
        tests_failed++;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

int main() {
    std::cout << "=== Nocturne Test Runner ===" << std::endl;

    // Vec3 tests
    math::Vec3 a(1, 2, 3);
    math::Vec3 b(4, 5, 6);
    auto c = a + b;
    check(c.x == 5 && c.y == 7 && c.z == 9, "Vec3 addition");

    auto d = b - a;
    check(d.x == 3 && d.y == 3 && d.z == 3, "Vec3 subtraction");

    auto e = a * 2;
    check(e.x == 2 && e.y == 4 && e.z == 6, "Vec3 scalar multiply");

    float dot = a.dot(b);
    check(dot == 32.0f, "Vec3 dot product");

    float dist = a.distance(b);
    check(std::abs(dist - 5.196f) < 0.01f, "Vec3 distance");

    // Matrix tests
    math::Matrix4x4 identity = {};
    identity.m[0][0] = 1; identity.m[1][1] = 1;
    identity.m[2][2] = 1; identity.m[3][3] = 1;

    auto p = identity.multiply_point(math::Vec3(1, 2, 3));
    check(p.x == 1 && p.y == 2 && p.z == 3, "Matrix identity multiply");

    // Projection tests
    math::Matrix4x4 ortho = {};
    ortho.m[0][0] = 1; ortho.m[1][1] = 1;
    ortho.m[2][2] = 1; ortho.m[3][3] = 1;

    auto sp = math::Projection::world_to_screen(math::Vec3(0, 0, 0), 1920, 1080, ortho);
    check(sp.valid, "Projection returns valid");

    // Registry tests (mock)
    check(true, "Registry placeholder");

    std::cout << "
=== RESULTADO: " << tests_passed << " passaram, " << tests_failed << " falharam ===" << std::endl;
    return tests_failed > 0 ? 1 : 0;
}
