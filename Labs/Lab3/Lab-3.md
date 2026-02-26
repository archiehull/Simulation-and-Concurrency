# 700106 / 700120 Lab Book

## Week 3  - Lab 3
### Q1. Position storage review

**Question:**
Where could you store the position of your physics objects and what implications would that have on the rest of your system?

- What options did you consider?
- What were the advantages and disadvantages of each?
- What was your final decision and why?

**Solution:**

I initially had everything in a single "Scene Object" stuct, which made conceptualising things easy, but led to an inflexible and inefficient system with unnecessary memory overhead for things that werent being used.

The positions and velocities could be stored within the Collider or math classes, which would keep the library nicely self contained. However, this implementation would mean the renderer would have to constantly retrieve position values from the physics collider. This also forces all objects to have collision properties, as the logic is baked into the same system.

My solution was to decouple all of my code from the scene object into an entity component system, storing positional data in a TransformComponent. Seperate components improves efficiency and flexibility, while ensuring position is stored and accessed from a single place.

The TranformComponent holds position, rotation and scale; Physics holds velocity, mass, restitution, isStatic and Collider holds geometric bounds.


**Reflection:**



##
### Q2. Movnig a ball through space

**Solution:**

To ensure the physics simulation remains stable regardless of the rendering framerate (e.g., 60 FPS), the simulation step must be decoupled from the render step. 

If the render loop passes a deltaTime of 0.016 seconds, running the physics once over that entire duration can lead to missed collisions or mathematical instability.
```cpp
int PhysicsSystem::subSteps = 4;
IntegrationMethod PhysicsSystem::currentMethod = IntegrationMethod::SemiImplicitEuler;
bool PhysicsSystem::applyGravity = true;

void PhysicsSystem::Update(Scene& scene, float deltaTime) {
    auto& registry = scene.GetRegistry();

    float dt = deltaTime / static_cast<float>(subSteps);

    for (int i = 0; i < subSteps; ++i) {
        Integrate(registry, dt);
        ResolveCollisions(registry);
    }
}
```
The main deltaTime is divided by a user-defined number of substeps, and the physics integration and collision resolution loops are run multiple times per frame using that smaller, fixed timestep (dt).

##

Numerical integration is used to calculate the new position of an object over time based on its velocity and acceleration.

Explicit Euler
- Calculates the new position using the current velocity, then calculates the new velocity.
- Simple but unstable over time as it relies on outdated velocity data for the position step.

Semi-Implicit Euler
- Calculates the new velocity first, then uses that updated velocity to calculate the new position.
- This swap improves energy conservation and stability in games.


```cpp
void PhysicsSystem::Integrate(Registry& registry, float dt) {
    for (Entity i = 0; i < registry.GetEntityCount(); ++i) {
        if (registry.HasComponent<TransformComponent>(i) &&
            registry.HasComponent<PhysicsComponent>(i)) {

            auto& transform = registry.GetComponent<TransformComponent>(i);
            auto& physics = registry.GetComponent<PhysicsComponent>(i);

            if (!physics.isStatic) {
                // Determine acceleration (Gravity or 0 for the lab test)
                glm::vec3 acceleration = applyGravity ? glm::vec3(0.0f, -9.81f, 0.0f) : glm::vec3(0.0f);

                // --- INTEGRATION METHODS ---
                if (currentMethod == IntegrationMethod::ExplicitEuler) {
                    // 1. Position updated using CURRENT velocity
                    transform.position += physics.velocity * dt;
                    // 2. Velocity updated using acceleration
                    physics.velocity += acceleration * dt;
                }
                else if (currentMethod == IntegrationMethod::SemiImplicitEuler) {
                    // 1. Velocity updated using acceleration FIRST
                    physics.velocity += acceleration * dt;
                    // 2. Position updated using NEW velocity
                    transform.position += physics.velocity * dt;
                }

                // Apply light air resistance
                physics.velocity *= std::pow(0.999f, dt * 60.0f);

                transform.UpdateMatrix();
            }
        }
    }
}
```

IMGUI menu:
```cpp
 if (ImGui::CollapsingHeader("Physics Engine", ImGuiTreeNodeFlags_DefaultOpen)) {

     ImGui::Text("Time Step & Substepping");
     // Slider to control how many times the physics loop runs per frame
     ImGui::SliderInt("Substeps per Frame", &PhysicsSystem::subSteps, 1, 16);

     ImGui::Spacing();
     ImGui::Text("Integration Method");

     // Dropdown for Integration Method
     int currentMethodIdx = static_cast<int>(PhysicsSystem::currentMethod);
     const char* methods[] = { "Explicit Euler", "Semi-Implicit Euler" };
     if (ImGui::Combo("Algorithm", &currentMethodIdx, methods, IM_ARRAYSIZE(methods))) {
         PhysicsSystem::currentMethod = static_cast<IntegrationMethod>(currentMethodIdx);
     }

     ImGui::Spacing();
     // Checkbox to disable gravity to prove the zero-acceleration lab requirement
     ImGui::Checkbox("Apply Gravity", &PhysicsSystem::applyGravity);
 }
 ```

##
### Q3. Make a sphere fall under the effect of gravity

**Solution:**
```cpp
struct PhysicsComponent {
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 forceAccumulator = glm::vec3(0.0f); // Stores all forces for the current frame
    
    float mass = 1.0f;
    float inverseMass = 1.0f; // Always ensure this is 1.0f / mass!
    
    bool isStatic = true;
    float friction = 0.98f; 
    float restitution = 1.0f; 
    
    // Helper to safely set mass and update inverse mass
    void SetMass(float newMass) {
        if (newMass <= 0.0f) {
            mass = 0.0f;
            inverseMass = 0.0f; // Infinite mass (static object)
        } else {
            mass = newMass;
            inverseMass = 1.0f / mass;
        }
    }
};
```
In the physics loop, instead of just setting acceleration to gravity, the Force of gravity ($F = mg$) is calcualetd first, added to the accumulator, then the final acceleration is calculated using Newton's Second Law ($a = F \times m^{-1}$).
```cpp
void PhysicsSystem::Integrate(Registry& registry, float dt) {
    for (Entity i = 0; i < registry.GetEntityCount(); ++i) {
        if (registry.HasComponent<TransformComponent>(i) && registry.HasComponent<PhysicsComponent>(i)) {

            auto& transform = registry.GetComponent<TransformComponent>(i);
            auto& physics = registry.GetComponent<PhysicsComponent>(i);

            if (!physics.isStatic && physics.inverseMass > 0.0f) {

                // 1. Accumulate Forces (Gravity: F = mg)
                if (applyGravity) {
                    glm::vec3 gravityForce = glm::vec3(0.0f, -9.81f, 0.0f) * physics.mass;
                    physics.forceAccumulator += gravityForce;
                }

                // 2. Calculate Acceleration (a = F / m)
                glm::vec3 acceleration = physics.forceAccumulator * physics.inverseMass;

                // 3. Integration Methods
                if (currentMethod == IntegrationMethod::ExplicitEuler) {
                    transform.position += physics.velocity * dt;
                    physics.velocity += acceleration * dt;
                }
                else if (currentMethod == IntegrationMethod::SemiImplicitEuler) {
                    physics.velocity += acceleration * dt;
                    transform.position += physics.velocity * dt;
                }
                else if (currentMethod == IntegrationMethod::RK4) {
                    // RK4 Implementation for constant acceleration
                    glm::vec3 k1_v = acceleration;
                    glm::vec3 k1_x = physics.velocity;

                    glm::vec3 k2_v = acceleration; // Assuming constant acceleration over dt
                    glm::vec3 k2_x = physics.velocity + k1_v * (dt * 0.5f);

                    glm::vec3 k3_v = acceleration;
                    glm::vec3 k3_x = physics.velocity + k2_v * (dt * 0.5f);

                    glm::vec3 k4_v = acceleration;
                    glm::vec3 k4_x = physics.velocity + k3_v * dt;

                    physics.velocity += (k1_v + 2.0f * k2_v + 2.0f * k3_v + k4_v) * (dt / 6.0f);
                    transform.position += (k1_x + 2.0f * k2_x + 2.0f * k3_x + k4_x) * (dt / 6.0f);
                }

                // 4. Clear the accumulator for the next frame
                physics.forceAccumulator = glm::vec3(0.0f);

                // (Optional: Apply air resistance here)
                physics.velocity *= std::pow(0.999f, dt * 60.0f);

                transform.UpdateMatrix();
            }
        }
    }
}
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
