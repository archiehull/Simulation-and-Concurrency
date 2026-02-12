#include "pch.h"
#include "Cylinder.h"

// -----------------------------------------------------------------------------
// Cylinder Point Containment
// -----------------------------------------------------------------------------

TEST(IsInside_Cylinder, PointInside) {
    // Cylinder from (0,0,0) to (0,10,0) with radius 2
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);

    // Point at (0, 5, 0) - middle of axis
    EXPECT_TRUE(c.IsInside({ 0.0, 5.0, 0.0 }));

    // Point at (1, 5, 0) - halfway to radius
    EXPECT_TRUE(c.IsInside({ 1.0, 5.0, 0.0 }));
}

TEST(IsInside_Cylinder, PointOutside_Radially) {
    // Cylinder from (0,0,0) to (0,10,0) with radius 2
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);

    // Point at (3, 5, 0) - distance 3 from axis (radius is 2)
    EXPECT_FALSE(c.IsInside({ 3.0, 5.0, 0.0 }));
}

TEST(IsInside_Cylinder, PointOutside_CloseToStart) {
    // Cylinder from (0,0,0) to (0,10,0) with radius 2
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);

    // Case 1: Just "behind" the start cap (longitudinally)
    // Point at (0, -0.1, 0) is on axis but before the start point
    EXPECT_FALSE(c.IsInside({ 0.0, -0.1, 0.0 }));

    // Case 2: Just outside radially near the start
    // Point at (2.1, 0, 0) is at start height but outside radius
    EXPECT_FALSE(c.IsInside({ 2.1, 0.0, 0.0 }));
}

TEST(IsInside_Cylinder, PointOutside_CloseToEnd) {
    // Cylinder from (0,0,0) to (0,10,0) with radius 2
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);

    // Case 1: Just "beyond" the end cap (longitudinally)
    // Point at (0, 10.1, 0) is on axis but past the end point
    EXPECT_FALSE(c.IsInside({ 0.0, 10.1, 0.0 }));

    // Case 2: Just outside radially near the end
    // Point at (2.1, 10.0, 0.0) is at end height but outside radius
    EXPECT_FALSE(c.IsInside({ 2.1, 10.0, 0.0 }));
}

TEST(IsInside_Cylinder, PointOnSurface) {
    // Cylinder from (0,0,0) to (0,10,0) with radius 2
    Cylinder c({ 0.0, 0.0, 0.0 }, { 0.0, 10.0, 0.0 }, 2.0f);

    // Exact radius at mid-height
    EXPECT_TRUE(c.IsInside({ 2.0, 5.0, 0.0 }));

    // Exact start cap center
    EXPECT_TRUE(c.IsInside({ 0.0, 0.0, 0.0 }));

    // Exact end cap center
    EXPECT_TRUE(c.IsInside({ 0.0, 10.0, 0.0 }));
}