#include "pch.h"
#include "Cylinder.h"
#include "Sphere.h"

// -----------------------------------------------------------------------------
// Sphere-Cylinder Intersection Tests
// -----------------------------------------------------------------------------

// Case: Sphere is inside cylinder intersecting at the middle of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(0,5,0), Radius 1
// Result:   Distance to axis is 0. SumRadii is 3. 0 <= 3 -> Intersect.
TEST(Intersects_SphereCylinder, Middle_Inside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 0.0, 5.0, 0.0 }, 1.0f);
    EXPECT_TRUE(c.Intersects(s));
}

// Case: Sphere is outside cylinder at the middle of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(5,5,0), Radius 1
// Result:   Distance to axis is 5. SumRadii is 3. 5 > 3 -> No Intersect.
TEST(Intersects_SphereCylinder, Middle_Outside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 5.0, 5.0, 0.0 }, 1.0f);
    EXPECT_FALSE(c.Intersects(s));
}

// Case: Sphere is inside cylinder intersecting at the start of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(0,0,0), Radius 1
// Result:   Sphere is centered exactly on the cylinder start cap.
TEST(Intersects_SphereCylinder, Start_Inside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 0.0, 0.0, 0.0 }, 1.0f);
    EXPECT_TRUE(c.Intersects(s));
}

// Case: Sphere is outside cylinder intersecting at the start of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(0,-5,0), Radius 1
// Result:   Sphere is 5 units below start. SumRadii is 3. 5 > 3 -> No Intersect.
TEST(Intersects_SphereCylinder, Start_Outside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 0.0, -5.0, 0.0 }, 1.0f);
    EXPECT_FALSE(c.Intersects(s));
}

// Case: Sphere is inside cylinder intersecting at the end of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(0,10,0), Radius 1
// Result:   Sphere is centered exactly on the cylinder end cap.
TEST(Intersects_SphereCylinder, End_Inside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 0.0, 10.0, 0.0 }, 1.0f);
    EXPECT_TRUE(c.Intersects(s));
}

// Case: Sphere is outside cylinder intersecting at the end of the line segment
// Cylinder: Start(0,0,0), End(0,10,0), Radius 2
// Sphere:   Center(0,15,0), Radius 1
// Result:   Sphere is 5 units above end. SumRadii is 3. 5 > 3 -> No Intersect.
TEST(Intersects_SphereCylinder, End_Outside) {
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);
    Sphere s({ 0.0, 15.0, 0.0 }, 1.0f);
    EXPECT_FALSE(c.Intersects(s));
}