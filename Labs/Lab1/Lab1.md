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

##
### Q1. 

**Question:**


**Solution:**


**Sample output:**


**Reflection:**


**Questions:**
