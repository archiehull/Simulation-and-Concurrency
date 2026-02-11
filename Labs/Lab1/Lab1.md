# 700106 / 700120 Lab Book

## Week 1 - Lab A
05/02 
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
All tests pass as expected and now prove whether a larger sphere correctly "collides" with a smaller one inside it, including edge cases where the spheres are identical.

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

**Reflection:**

The suite of GoogleTests confirms the logic for aligning lines with major axis and arbitrary diagonals.


##
### Q3. Sphere Line Intersection Test

**Question:**
Determine if an infinite line intersects a sphere.

**Solution:**
This solution directly reused the logic from Q2. If the shortest distance from the sphere's center to the line is less than (or equal to) the sphere's radius, they must intersect.

```cpp
bool Intersects(const InfiniteLine& line) const
{
    double dist = ShortestDistanceToLine(line, m_position);
    return dist <= static_cast<double>(m_radius) + EPS;
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

TEST(Intersects_InfiniteLine_NoEps, NoIntersection_CentreAtOrigin) {
    Sphere s({ 0.0, 0.0, 0.0 }, 3.0f);
    InfiniteLine line{ {5.0, 5.0, 5.0}, {1.0, 0.0, 0.0} };
    double dist = Sphere::ShortestDistanceToLine(line, s.Position());
    // Expect distance strictly greater than radius (no EPS)
    EXPECT_GT(dist, static_cast<double>(s.m_radius));
}

TEST(Intersects_InfiniteLine_NoEps, PassesThroughSphere_CentreAtSphere) {
    Sphere s({ 10.0, 0.0, 0.0 }, 5.0f);
    InfiniteLine line{ {10.0, 0.0, 0.0}, {-1.0, 0.0, 0.0} }; // line passes through sphere center
    double dist = Sphere::ShortestDistanceToLine(line, s.Position());
    EXPECT_LE(dist, static_cast<double>(s.m_radius));
}

TEST(Intersects_InfiniteLine_NoEps, LineStartsInsideSphere) {
    Sphere s({ 2.0, 2.0, 2.0 }, 5.0f);
    InfiniteLine line{ {3.0, 2.0, 2.0}, {1.0, 0.0, 0.0} }; // start point lies within sphere
    double dist = Sphere::ShortestDistanceToLine(line, s.Position());
    EXPECT_LE(dist, static_cast<double>(s.m_radius));
}

TEST(Intersects_InfiniteLine_NoEps, LinePassesThroughSphereCenter) {
    Sphere s({ 0.0, 0.0, 0.0 }, 3.0f);
    InfiniteLine line{ {-5.0, 0.0, 0.0}, {1.0, 0.0, 0.0} }; // line goes through sphere center
    double dist = Sphere::ShortestDistanceToLine(line, s.Position());
    EXPECT_LE(dist, static_cast<double>(s.m_radius));
}
```

**Reflection:**

The test suite has been extended to verify different geometric relationships - including the intersection of a line through the centre of a sphere, if there is no intersection, or if the origin of the line exists within the sphere.

##
### Q4. Closest Distance from a Point to a Plane

**Question:**
Calculate the shortest distance between a specific point and a plane defined by a point and a normal vector.

**Solution:**
```cpp
double DistanceFromPoint(const Vec3& point) const
{
    return std::abs(Dot(m_normal, point) + m_d);
}
```
```sh
[----------] 7 tests from PlaneDistance
[ RUN      ] PlaneDistance.PointAbovePlane
[       OK ] PlaneDistance.PointAbovePlane (0 ms)
[ RUN      ] PlaneDistance.PointBelowPlane
[       OK ] PlaneDistance.PointBelowPlane (0 ms)
[ RUN      ] PlaneDistance.PointOnPlane
[       OK ] PlaneDistance.PointOnPlane (0 ms)
[ RUN      ] PlaneDistance.PointCloseToPlane
[       OK ] PlaneDistance.PointCloseToPlane (0 ms)
[ RUN      ] PlaneDistance.NegativeCoordinates
[       OK ] PlaneDistance.NegativeCoordinates (0 ms)
[ RUN      ] PlaneDistance.AlongNormalDirection
[       OK ] PlaneDistance.AlongNormalDirection (0 ms)
[ RUN      ] PlaneDistance.RandomDirection
[       OK ] PlaneDistance.RandomDirection (0 ms)
[----------] 7 tests from PlaneDistance (2 ms total)
```


**Reflection:**
My tests cover points on both sides of the plane (positive and negative signed distance), points exactly on the plane, and planes with non-unit normal vectors in the constructor (to ensure normalisation works).

**Questions:**

##
### Q. 

**Question:**


**Solution:**


**Sample output:**


**Reflection:**


**Questions:**