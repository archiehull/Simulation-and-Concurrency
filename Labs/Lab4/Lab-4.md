# 700106 / 700120 Lab Book

## Week 4 - Lab 4
### Q1. Be able to collide a ball with a fixed object


To handle collisions with fixed objects, I treat their mass as infinite. This is represented by storing an invMass (inverse mass, $1/m$) property on the physics component. 

A fixed object has an invMass of 0.0f. The general impulse scalar equation is:$$j = \frac{-(1 + e)(v_{rel} \cdot n)}{m_A^{-1} + m_B^{-1}}$$By setting $m_B^{-1} = 0$ for fixed objects, the equation simplifies without requiring separate "if fixed" branching logic or risking division-by-zero errors.

```cpp
inline void ResolveSpherePlaneCollision(MovingSphere& a, const Plane& p, float planeRestitution) {
    float velAlongNormal = glm::dot(a.velocity, p.GetNormal());

    // If moving away from the plane, do nothing
    if (velAlongNormal > 0.0f) return;

    // Combine restitution (bounciness)
    float e = a.restitution * planeRestitution;
    float j = -(1.0f + e) * velAlongNormal;

    // Mass of plane is infinite, so we only divide by sphere's mass
    j /= (1.0f / a.mass);

    glm::vec3 impulse = p.GetNormal() * j;
    a.velocity += impulse * (1.0f / a.mass);
}
```

I added specific tests to verify that the impulse calculation correctly reflects velocity against planes, static spheres, and cylinders. These tests ensure the logic holds even for non-axis-aligned surfaces.

```cpp
TEST(Physics_FixedCollision, SpherePlane_DiagonalReflection) {
    // Ball moving Down-Right hitting an Up-Left diagonal plane
    glm::vec3 initialVel(5.0f, -5.0f, 0.0f);
    MovingSphere ball({ 0.0f, 0.1f, 0.0f }, 1.0f, initialVel, 1.0f, 1.0f);
    
    glm::vec3 planeNormal = glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f));
    Plane fixedPlane({ 0.0f, 0.0f, 0.0f }, planeNormal);

    ASSERT_TRUE(fixedPlane.Intersects(ball.sphere));
    ResolveSpherePlaneCollision(ball, fixedPlane, 1.0f);

    // Should reflect from (5, -5) to (-5, 5)
    ExpectVec3Near(ball.velocity, glm::vec3(-5.0f, 5.0f, 0.0f));
}

TEST(Physics_FixedCollision, SphereStaticSphere_HeadOn) {
    MovingSphere ballA({ 0.0f, 0.0f, 0.0f }, 1.0f, { 10.0f, 0.0f, 0.0f }, 1.0f, 1.0f);
    // Infinite mass simulated via high mass (or invMass = 0 in updated versions)
    MovingSphere fixedBall({ 1.5f, 0.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f }, 1e5f, 1.0f);

    ASSERT_TRUE(ballA.sphere.CollideWith(fixedBall.sphere));
    ResolveElasticCollision(ballA, fixedBall);

    EXPECT_NEAR(ballA.velocity.x, -10.0f, 1e-4f);
}
```

**Reflection:**
I implemented an impulse-based resolution which calculates a scalar $j$ based on the relative velocity projected onto this normal. By treating fixed objects as having infinite mass (where $1/m = 0$), the impulse equation effectively ignores the static object's momentum, applying the entire change in velocity to the moving ball.

##
### Q2. To be able to collide a ball with another moving ball with the same mass

Collisions between two moving spheres of equal mass are resolved using an impulse-based method. The collision normal is computed dynamically as the vector between the two sphere centres at the moment of impact. The relative velocity along this normal is calculated to determine the impulse scalar.

Because both objects possess an equal inverse mass (invMass), the calculated impulse applies equally but in opposite directions to each body. The velocity of each sphere is updated by applying the impulse vector scaled by their respective inverse mass. This implementation exchanges their velocities strictly along the collision normal, leaving any tangential velocity unaffected.

I implemented several Google Test cases in SphereTests.cpp to verify equal-mass collision resolution across different head-on scenarios, including collisions with stationary targets, opposing velocities, and same-direction impacts.

To verify that glancing collisions correctly preserve tangential velocity, I added the following test case:

```cpp
TEST(Physics_Collision, TwoMoving_Glancing) {
    // A is moving right at 10 m/s
    MovingSphere a({ 0.0f, 0.0f, 0.0f }, 1.0f, { 10.0f, 0.0f, 0.0f }, 1.0f, 1.0f);
    
    // B is stationary, offset to create a 45-degree collision angle
    // At distance sqrt(2) ~ 1.414 on both X and Y, the distance is exactly 2.0 (sum of radii)
    MovingSphere b({ 1.414213f, 1.414213f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f);

    ResolveElasticCollision(a, b);

    // The collision normal is (1, 1, 0) normalized. 
    // A loses half its X velocity and gains negative Y velocity.
    // B inherits the exact velocity vector that A lost.
    EXPECT_NEAR(a.velocity.x, 5.0f, 1e-4f);
    EXPECT_NEAR(a.velocity.y, -5.0f, 1e-4f);
    
    EXPECT_NEAR(b.velocity.x, 5.0f, 1e-4f);
    EXPECT_NEAR(b.velocity.y, 5.0f, 1e-4f);
}
```
To demonstrate this functionality visually, I configured a scenario in the sandbox environment that spawns pairs of equal-mass spheres with intersecting trajectories, observing both head-on and glancing collisions over multiple fixed timesteps.
##
### Q1. 

**Question:**


**Solution:**


**Sample output:**


**Reflection:**


**Questions:**
