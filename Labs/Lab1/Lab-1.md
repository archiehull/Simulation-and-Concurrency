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
To optimise the collision check, I avoided expensive square root operations by comparing the squared distance between sphere centres against the squared sum of their radii, using a helper function DistanceSqToPoint. 

My tests covered standard overlaps and critical edge cases, including fully contained spheres, identical spheres, and large coordinate values to verify floating-point precision at scale.

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

I calculated the closest point on an infinite line by projecting the vector from the line's origin to the target point onto the line's direction vector. 

My tests confirmed this method works for both axis-aligned and diagonal lines, and correctly returns a distance of zero (within epsilon) when the point lies exactly on the line.


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

I solved the sphere-line intersection by reusing the point-to-line distance logic; if the shortest distance from the sphere's centre to the line is less than or equal to the radius, they intersect. 

I expanded the test suite to verify lines passing through the centre, lines starting inside the sphere, and lines that clearly miss the volume to ensure the logic holds for all geometric relationships.

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
For point-to-plane distance, I utilised the stored plane equation components. The distance is the absolute value of the dot product between the plane's normal and the point, plus the constant $d$, which relies on the normal being normalised during construction. 

I tested points on positive and negative sides, as well as on the plane itself, to ensure the signed distance was handled correctly.

##
### Q5. Sphere to Plane Collision

**Question:**
Determine if a Sphere intersects with a Plane.

**Solution:**
ince I had already implemented the logic to find the closest distance from a point to a plane in Q4, I could reuse that for this collision test. A sphere intersects a plane if the distance from the sphere's center to the plane is less than or equal to the sphere's radius.

I added an Intersects method to the Plane class that takes a Sphere as an argument.

```cpp
bool Intersects(const Sphere& sphere) const
{
    // Reuse the distance calculation
    double dist = DistanceFromPoint(sphere.Position());
    
    // Check if distance is within the radius (allowing for small epsilon)
    return dist <= static_cast<double>(sphere.m_radius) + 1e-9;
}
```

**Reflection:**
Leveraging the previous distance logic, I implemented sphere-plane intersection by checking if the point-to-plane distance is less than the sphere's radius. 

I designed specific unit tests to verify various states: distinct gaps, clear intersections, boundary cases where the sphere touches the surface, and bisected spheres where the centre lies on the plane.
