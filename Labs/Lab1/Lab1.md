# 700106 / 700120 Lab Book

## Week  - Lab 
### Q1. Sphere-Sphere Intersection Test

**Question:**
Create unit tests that determine the collisions between two spheres.

**Solution:**
```cpp
double DistanceSqToPoint(const Vec3& p) const
{
    double dx = p.x - m_position.x;
    double dy = p.y - m_position.y;
    double dz = p.z - m_position.z;
    return dx * dx + dy * dy + dz * dz;
}
```

```cpp
bool CollideWith(const Sphere& other) const
{
    double rSum = static_cast<double>(m_radius) + static_cast<double>(other.m_radius);
    return DistanceSqToPoint(other.m_position) <= (rSum * rSum) + EPS;
}
```
```cpp

// Sphere-sphere collision tests
TEST(SphereSphereCollision, NoIntersection_CentreAtOrigin) {
    Sphere a({ 0.0, 0.0, 0.0 }, 1.0f);
    Sphere b({ 5.0, 0.0, 0.0 }, 1.0f);
    EXPECT_FALSE(a.CollideWith(b));
}

TEST(SphereSphereCollision, NoIntersection_OffsetCentre) {
    Sphere a({ 3.0, 3.0, 3.0 }, 2.0f);
    Sphere b({ 10.0, 10.0, 10.0 }, 2.0f);
    EXPECT_FALSE(a.CollideWith(b));
}

TEST(SphereSphereCollision, Overlapping_CentreAtOrigin) {
    Sphere a({ 0.0, 0.0, 0.0 }, 2.0f);
    Sphere b({ 2.0, 0.0, 0.0 }, 2.0f);
    EXPECT_TRUE(a.CollideWith(b));
}

TEST(SphereSphereCollision, Overlapping_OffsetCentre) {
    Sphere a({ 5.0, 5.0, 5.0 }, 3.0f);
    Sphere b({ 8.0, 5.0, 5.0 }, 3.0f);
    EXPECT_TRUE(a.CollideWith(b));
}

TEST(SphereSphereCollision, FullyContained_CentreAtOrigin) {
    Sphere a({ 0.0, 0.0, 0.0 }, 3.0f);
    Sphere b({ 1.0, 0.0, 0.0 }, 1.0f);
    EXPECT_TRUE(a.CollideWith(b));
    EXPECT_TRUE(b.CollideWith(a)); // symmetric
}

TEST(SphereSphereCollision, FullyContained_OffsetCentre) {
    float multi = 100000000000000.1;
    Sphere a({ 6.0 *multi, 6.0 * multi, 6.0 * multi }, 5.0f * multi);
    Sphere b({ 7.0 * multi, 6.0 * multi, 6.0 * multi }, 2.0f * multi);
    EXPECT_TRUE(a.CollideWith(b));
}

TEST(SphereSphereCollision, IdenticalSpheres) {
    Sphere a({ 0.0, 0.0, 0.0 }, 2.0f);
    Sphere b({ 0.0, 0.0, 0.0 }, 2.0f);
    EXPECT_TRUE(a.CollideWith(b));
}
```
```sh
[----------] 7 tests from SphereSphereCollision
[ RUN      ] SphereSphereCollision.NoIntersection_CentreAtOrigin
[       OK ] SphereSphereCollision.NoIntersection_CentreAtOrigin (0 ms)
[ RUN      ] SphereSphereCollision.NoIntersection_OffsetCentre
[       OK ] SphereSphereCollision.NoIntersection_OffsetCentre (0 ms)
[ RUN      ] SphereSphereCollision.Overlapping_CentreAtOrigin
[       OK ] SphereSphereCollision.Overlapping_CentreAtOrigin (0 ms)
[ RUN      ] SphereSphereCollision.Overlapping_OffsetCentre
[       OK ] SphereSphereCollision.Overlapping_OffsetCentre (0 ms)
[ RUN      ] SphereSphereCollision.FullyContained_CentreAtOrigin
[       OK ] SphereSphereCollision.FullyContained_CentreAtOrigin (0 ms)
[ RUN      ] SphereSphereCollision.FullyContained_OffsetCentre
[       OK ] SphereSphereCollision.FullyContained_OffsetCentre (1 ms)
[ RUN      ] SphereSphereCollision.IdenticalSpheres
[       OK ] SphereSphereCollision.IdenticalSpheres (0 ms)
[----------] 7 tests from SphereSphereCollision (3 ms total)

[----------] Global test environment tear-down
[==========] 28 tests from 6 test cases ran. (13 ms total)
[  PASSED  ] 28 tests.
```

**Reflection:**
All tests pass as expected, using epsilon to account for floating point errors, although tests still passed without epsilon.

##
### Q2. Closest Distance from a Point to a Line

**Question:**


**Solution:**

```cpp
	static Vec3 ClosestPointOnInfiniteLine(const InfiniteLine& line, const Vec3& PG)
	{
		Vec3 PL = line.point;
		Vec3 DL = line.direction;

		// find the vector from PL to PG
		Vec3 PLPG = PG - PL;

		// find the projection scalar m
		double m = Dot(PLPG, DL) / Dot(DL, DL);

		// multiply by DL to find the point on the line
		return PL + (DL * m);
	}

	static double ShortestDistanceToLine(const InfiniteLine& line, const Vec3& PG)
	{
		Vec3 PA = ClosestPointOnInfiniteLine(line, PG);

		return Length(PG - PA);
	}
```

```cpp
TEST(InfiniteLineDistance, ClosestPointOnLine) {
    InfiniteLine line{ {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} };
    Vec3 PG{ 2.0, 3.0, 4.0 };
    double distance = Sphere::ShortestDistanceToLine(line, PG);
    EXPECT_NEAR(distance, 1.41421356, 0.01);
}

TEST(InfiniteLineDistance, PointOnLine) {
    InfiniteLine line{ {0.0, 0.0, 0.0}, {1.0, 2.0, 3.0} };
    Vec3 PG{ 3.0, 6.0, 9.0 };
    double distance = Sphere::ShortestDistanceToLine(line, PG);
    EXPECT_NEAR(distance, 0.0, 1e-9);
}

TEST(InfiniteLineDistance, VerticalLine) {
    InfiniteLine line{ {2.0, 2.0, 0.0}, {0.0, 0.0, 1.0} };
    Vec3 PG{ 4.0, 5.0, 3.0 };
    double distance = Sphere::ShortestDistanceToLine(line, PG);
    EXPECT_NEAR(distance, 3.60555, 0.01);
}

TEST(InfiniteLineDistance, HorizontalLine) {
    InfiniteLine line{ {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} };
    Vec3 PG{ 3.0, 4.0, 5.0 };
    double distance = Sphere::ShortestDistanceToLine(line, PG);
    EXPECT_NEAR(distance, 6.40312, 0.01);
}

TEST(InfiniteLineDistance, DiagonalLine) {
    InfiniteLine line{ {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0} };
    Vec3 PG{ 2.0, 5.0, 3.0 };
    double distance = Sphere::ShortestDistanceToLine(line, PG);
    EXPECT_NEAR(distance, 4.54606, 0.01);
}
```


```sh
[----------] 5 tests from InfiniteLineDistance
[ RUN      ] InfiniteLineDistance.ClosestPointOnLine
[       OK ] InfiniteLineDistance.ClosestPointOnLine (1 ms)
[ RUN      ] InfiniteLineDistance.PointOnLine
[       OK ] InfiniteLineDistance.PointOnLine (0 ms)
[ RUN      ] InfiniteLineDistance.VerticalLine
[       OK ] InfiniteLineDistance.VerticalLine (0 ms)
[ RUN      ] InfiniteLineDistance.HorizontalLine
[       OK ] InfiniteLineDistance.HorizontalLine (0 ms)
[ RUN      ] InfiniteLineDistance.DiagonalLine
[       OK ] InfiniteLineDistance.DiagonalLine (0 ms)
[----------] 5 tests from InfiniteLineDistance (3 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test case ran. (4 ms total)
[  PASSED  ] 5 tests.
```

**Sample output:**


**Reflection:**


**Questions:**

##
### Q1. 

**Question:**


**Solution:**


**Sample output:**


**Reflection:**


**Questions:**
