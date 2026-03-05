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

```cpp
inline void ResolveElasticCollision(MovingSphere& a, MovingSphere& b) {
    glm::vec3 normal = b.sphere.Position() - a.sphere.Position();
    float distSq = glm::dot(normal, normal);
    if (distSq == 0.0f) return;

    glm::vec3 relVel = a.velocity - b.velocity;
    float velAlongNormal = glm::dot(relVel, normal);

    if (velAlongNormal < 0.0f) return;

    double e = static_cast<double>(a.restitution) * static_cast<double>(b.restitution);

    // Sum of inverse masses is used to distribute impulse
    double invMassSum = static_cast<double>(a.invMass) + static_cast<double>(b.invMass);
    if (invMassSum <= 0.0) return; 

    double j = -((1.0 + e) * static_cast<double>(velAlongNormal));
    j /= (invMassSum * static_cast<double>(distSq));

    glm::vec3 impulse = normal * static_cast<float>(j);

    // Apply impulse multiplied by individual inverse mass
    a.velocity += impulse * a.invMass;
    b.velocity -= impulse * b.invMass;
}
```


I verified equal-mass collision resolution across different head-on scenarios, including collisions with stationary targets, opposing velocities, and same-direction impacts.

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
### Q3. Be able to collide a ball with another moving ball with different masses

Collisions between spheres of different masses are handled natively by the existing impulse-based resolution implementation. The impulse scalar formula divides by the sum of the inverse masses (invMassSum), meaning the total impulse is scaled according to the combined inertia of both objects.

When applying the resulting impulse vector to the velocities, each object's velocity change is multiplied by its own inverse mass (invMass). This ensures that lighter objects experience a larger change in velocity, while heavier objects experience a smaller change, accurately conserving total momentum across the system.

To explicitly verify the resulting velocities against the standard 1D kinematic formulas provided in the lab requirements, I added the following test case. It simulates a 2kg ball moving at 10m/s striking a stationary 3kg ball:

```cpp
TEST(Physics_Collision, UnequalMass_HeadOn) {
    // Ball A: mass = 2.0 (invMass = 0.5), moving right at 10 m/s
    MovingSphere a({ 0.0f, 0.0f, 0.0f }, 1.0f, { 10.0f, 0.0f, 0.0f }, 0.5f, 1.0f);
    
    // Ball B: mass = 3.0 (invMass = 0.3333f), stationary
    MovingSphere b({ 2.0f, 0.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f }, 1.0f / 3.0f, 1.0f);

    ResolveElasticCollision(a, b);

    // Using 1D elastic collision formulas:
    // v1 = ((m1 - m2)/(m1 + m2))*u1 = ((2-3)/5)*10 = -2.0 m/s
    // v2 = ((2*m1)/(m1 + m2))*u1 = (4/5)*10 = 8.0 m/s
    EXPECT_NEAR(a.velocity.x, -2.0f, 1e-4f);
    EXPECT_NEAR(a.velocity.y, 0.0f, 1e-4f);
    
    EXPECT_NEAR(b.velocity.x, 8.0f, 1e-4f);
    EXPECT_NEAR(b.velocity.y, 0.0f, 1e-4f);
}
```



##
### Q4. Reflect on setting velocities versus calculating impulse

To evaluate both approaches, I implemented a toggle system within the physics engine to switch between direct velocity changes (Impulse) and calculating a force over a fixed timestep (Force Accumulation).

In the Impulse approach, the required change in momentum ($J$) is calculated, and the object's velocity is modified directly within the collision resolution function.

In the Force Accumulation approach, the equivalent force required to achieve the same velocity change over the fixed timestep ($dt$) is calculated using $F = J / dt$. 

This force is added to the entity's forceAccumulator. The actual velocity change is deferred until the integration step, where acceleration is derived from accumulated forces ($a = F \cdot m^{-1}$) and integrated into velocity ($v = v + a \cdot dt$).

```cpp
glm::vec3 impulse = normal * static_cast<float>(j);

    if (useForce && dt > 0.0f) {
        // Force Accumulation Approach: F = J / dt
        glm::vec3 force = impulse / dt;
        if (a.invMass > 0.0f) a.forceAccumulator += force;
        if (b.invMass > 0.0f) b.forceAccumulator -= force;
    } else {
        // Direct Velocity Change (Impulse) Approach
        a.velocity += impulse * a.invMass;
        b.velocity -= impulse * b.invMass;
    }
```

When simulating a single isolated collision, both approaches are mathematically equivalent and result in the exact same final velocity at the end of the frame integration step.

However, significant differences emerge when dealing with multiple objects or simultaneous collisions. Applying forces to an accumulator delays the actual velocity update until the integration phase at the end of the fixed timestep. If an object experiences multiple collisions in the same substep (e.g. trapped in a corner), the second collision resolution will compute its response based on the old, pre-collision velocity. This results in inaccurate impulse calculations, causing severe overlap and physics instability.

By contrast, applying an instantaneous impulse modifies the velocity vector in memory immediately upon detection. Subsequent collisions in the same substep accurately use the newly updated velocity. Therefore, direct velocity manipulation is required for resolving hard rigid-body impacts sequentially, while force accumulation should be reserved strictly for continuous environmental interactions such as gravity and friction.


##
### Q5. Add elasticity to your physics model

Elasticity is integrated into the physics engine using the Coefficient of Restitution ($e$), which is stored as a property on the PhysicsComponent and passed into the MovingSphere struct.The restitution value modifies the impulse scalar equation:$$j = \frac{-(1 + e)(v_{rel} \cdot n)}{m_A^{-1} + m_B^{-1}}$$


In my implementation, the combined coefficient of restitution for a collision is calculated by multiplying the individual restitution values of the two colliding bodies, which natively keeps the value clamped between 0.0 and 1.0.If $e = 1.0$ (perfectly elastic), the impulse scalar is multiplied by 2, resulting in a perfect reflection of relative velocity along the collision normal. 


If $e = 0.0$ (perfectly inelastic), the impulse scalar is multiplied by 1, which provides exactly enough force to stop the objects relative to each other along the normal axis, resulting in zero bounce.

```cpp
// Calculate combined restitution
    double e = static_cast<double>(a.restitution) * static_cast<double>(b.restitution);
    double invMassSum = static_cast<double>(a.invMass) + static_cast<double>(b.invMass);
    if (invMassSum <= 0.0) return; 

    // Apply elasticity (1 + e) to the impulse scalar
    double j = -((1.0 + e) * static_cast<double>(velAlongNormal));
    j /= (invMassSum * static_cast<double>(distSq));
```
