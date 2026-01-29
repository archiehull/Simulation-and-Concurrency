#include "pch.h"
#include "Sphere.h"
#include "Plane.h"
#include "Collider.h"
#include <memory>

TEST(IsInside, BasicCentreInside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 0.0, 0.0, 0.0 };
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, DiagonalInside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 3.0, 4.0, 0.0 }; // distance 5
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, NonOriginInside) {
    Sphere sphere({ 2.0, 3.0, -1.0 }, 10.0);
    Vec3 point{ 5.0, 6.0, -2.0 }; // ~4.36 from center
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, PrecisionInside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 4.999999, 0.0, 0.0 }; // ~4.999999 < 5
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, NegativeCoordsInside) {
    Sphere sphere({ -2.0, -3.0, -4.0 }, 7.0);
    Vec3 point{ -5.0, -6.0, -4.0 }; // ~4.2426 from center
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, CloseCallInside) {
    Sphere sphere({ 7.0, 8.0, 9.0 }, 10.0);
    Vec3 point{ 16.99, 8.0, 9.0 }; // ~9.99 < 10
    EXPECT_TRUE(sphere.IsInside(point));
}

TEST(IsInside, BasicCentreOutside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 6.0, 0.0, 0.0 };
    EXPECT_FALSE(sphere.IsInside(point));
}

TEST(IsInside, DiagonalOutside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 4.0, 4.0, 0.0 }; // ~5.66
    EXPECT_FALSE(sphere.IsInside(point));
}

TEST(IsInside, NonOriginOutside) {
    Sphere sphere({ 2.0, 3.0, -1.0 }, 10.0);
    Vec3 point{ 15.0, 3.0, -1.0 }; // 13
    EXPECT_FALSE(sphere.IsInside(point));
}

TEST(IsInside, PrecisionOutside) {
    Sphere sphere({ 0.0, 0.0, 0.0 }, 5.0);
    Vec3 point{ 5.000001, 0.0, 0.0 };
    EXPECT_FALSE(sphere.IsInside(point));
}

TEST(IsInside, NegativeCoordsOutside) {
    Sphere sphere({ -2.0, -3.0, -4.0 }, 7.0);
    Vec3 point{ -10.0, -3.0, -4.0 }; // 8
    EXPECT_FALSE(sphere.IsInside(point));
}

TEST(IsInside, CloseCallOutside) {
    Sphere sphere({ 7.0, 8.0, 9.0 }, 10.0);
    Vec3 point{ 17.01, 8.0, 9.0 }; // ~10.01
    EXPECT_FALSE(sphere.IsInside(point));
}

// Sphere intersection tests
TEST(Intersects_Sphere, SegmentThroughSphere) {
    Sphere s({ 0.0, 0.0, 0.0 }, 5.0f);
    Line seg{ { -10.0, 0.0, 0.0 }, { 10.0, 0.0, 0.0 } };
    EXPECT_TRUE(s.Intersects(seg));
}

TEST(Intersects_Sphere, SegmentMissesSphere) {
    Sphere s({ 0.0, 0.0, 0.0 }, 5.0f);
    Line seg{ { -10.0, 6.0, 0.0 }, { -6.0, 6.0, 0.0 } }; // passes above the sphere
    EXPECT_FALSE(s.Intersects(seg));
}

TEST(Intersects_Sphere, DegeneratePointInside) {
    Sphere s({ 0.0, 0.0, 0.0 }, 5.0f);
    Line seg{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 } }; // point inside
    EXPECT_TRUE(s.Intersects(seg));
}

TEST(Intersects_Sphere, DegeneratePointOutside) {
    Sphere s({ 0.0, 0.0, 0.0 }, 5.0f);
    Line seg{ { 6.0, 0.0, 0.0 }, { 6.0, 0.0, 0.0 } }; // point outside
    EXPECT_FALSE(s.Intersects(seg));
}

// Plane tests
TEST(IsInside_Plane, HalfSpaceAbove) {
    Plane p({ 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }); // plane y = 0, inside is y >= 0
    Vec3 above{ 0.0, 1.0, 0.0 };
    Vec3 below{ 0.0, -1.0, 0.0 };
    Vec3 onPlane{ 0.0, 0.0, 0.0 };
    EXPECT_TRUE(p.IsInside(above));
    EXPECT_FALSE(p.IsInside(below));
    EXPECT_TRUE(p.IsInside(onPlane)); // point on plane considered inside
}

TEST(Intersects_Plane, SegmentCrossesPlane) {
    Plane p({ 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 });
    Line seg{ { 0.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0 } };
    EXPECT_TRUE(p.Intersects(seg));
}

TEST(Intersects_Plane, SegmentParallelNoIntersection) {
    Plane p({ 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 });
    Line seg{ { 0.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 } }; // both endpoints above plane
    EXPECT_FALSE(p.Intersects(seg));
}

// Polymorphism via Collider
TEST(Collider_Polymorphic, SphereAsCollider) {
    std::unique_ptr<Collider> c(new Sphere({ 0.0, 0.0, 0.0 }, 5.0f));
    Vec3 inside{ 3.0, 4.0, 0.0 };
    Vec3 outside{ 6.0, 0.0, 0.0 };
    EXPECT_TRUE(c->IsInside(inside));
    EXPECT_FALSE(c->IsInside(outside));

    Line seg{ { -10.0, 0.0, 0.0 }, { 10.0, 0.0, 0.0 } };
    EXPECT_TRUE(c->Intersects(seg));
}

TEST(Collider_Polymorphic, PlaneAsCollider) {
    std::unique_ptr<Collider> c(new Plane({ 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }));
    Vec3 above{ 0.0, 2.0, 0.0 };
    Vec3 below{ 0.0, -2.0, 0.0 };
    EXPECT_TRUE(c->IsInside(above));
    EXPECT_FALSE(c->IsInside(below));

    Line seg{ { 0.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0 } };
    EXPECT_TRUE(c->Intersects(seg));
}