# 700106 / 700120 Lab Book

## Week 5 - Lab 5 
### Q1. Orientation and angular displacement 
Add appropriate tests to your testing framework to apply an angular displacement to your physics objects. 

**Solution:**
To represent orientation, a 3x3 matrix (`glm::mat3`) was added to the physics object state. The `ApplyAngularDisplacement` function was implemented using GLM's 4x4 rotation mathematics, which is then cast back to a 3x3 matrix to update the local orientation.

```cpp
void ApplyAngularDisplacement(MovingSphere& body, const glm::vec3& axis, float angleRadians)
{
    // Build a 4x4 rotation matrix and extract the 3x3 portion
    glm::mat4 rot4 = glm::rotate(glm::mat4(1.0f), angleRadians, glm::normalize(axis));
    glm::mat3 rot3 = glm::mat3(rot4);

    // Apply rotation relative to the current orientation
    body.orientation = rot3 * body.orientation;
}
```
To verify the mathematics, Google Test unit tests were written. A custom macro (ExpectMat3Near) was created to handle floating-point precision inaccuracies when comparing matrices.
```cpp
#define ExpectMat3Near(m1, m2, tolerance) \
    for (int c = 0; c < 3; ++c) { \
        for (int r = 0; r < 3; ++r) { \
            EXPECT_NEAR((m1)[c][r], (m2)[c][r], tolerance); \
        } \
    }
```

| Test Case | Axis | Angle | Expected 3x3 Matrix (Column-Major) | Status |
| :--- | :---: | :---: | :--- | :---: |
| `Rotate_X_90` | X (1,0,0) | 90° ($\pi/2$) | `[1, 0, 0], [0, 0, 1], [0, -1, 0]` | Pass |
| `Rotate_Y_180` | Y (0,1,0) | 180° ($\pi$) | `[-1, 0, 0], [0, 1, 0], [0, 0, -1]` | Pass |
| `Rotate_Z_270` | Z (0,0,1) | 270° ($3\pi/2$) | `[0, -1, 0], [1, 0, 0], [0, 0, 1]` | Pass |
| `Rotate_X_360` | X (1,0,0) | 360° ($2\pi$) | `[1, 0, 0], [0, 1, 0], [0, 0, 1]` | Pass |
| `Rotate_XY_Comb`| X then Y | 90° ea. | `[0, 0, -1], [1, 0, 0], [0, -1, 0]` | Pass |

This logic was subsequently integrated into the ECS PhysicsSystem, where angular velocity is integrated over time (dt) to update the 3x3 matrix frame-by-frame, which is then passed directly to the Vulkan renderer's 4x4 push constants.

**Reflection:**
sing a 3x3 matrix for orientation is a solid approach because it avoids gimbal lock and slots easily into the 4x4 model matrix expected by the Vulkan renderer. Relying on GLM's glm::rotate meant it was necessary to cast up to a 4x4 matrix and back down to a 3x3, but doing this was much safer and faster than writing the rotation logic from scratch.



## Q2: Angular Velocity Integration

### Approach
To support continuous rotation, an angularVelocity vector was introduced to the MovingSphere component. The magnitude of this vector represents the speed of rotation in radians per second, and the normalized vector represents the axis of rotation.

During the physics integration step, a new IntegrateAngularVelocity function calculates the discrete angular displacement for the current frame (speed * dt) and applies it to the object's orientation matrix using the existing ApplyAngularDisplacement function. To test this, a simulation loop was built inside Google Test that incrementally updates the orientation over a fixed timescale.

```cpp
void IntegrateAngularVelocity(MovingSphere& body, float dt)
{
    float speed = glm::length(body.angularVelocity);
    if (speed > 1e-6f) { 
        glm::vec3 axis = body.angularVelocity / speed; 
        float angleRadians = speed * dt;
        ApplyAngularDisplacement(body, axis, angleRadians);
    }
}
```

### Test Data & Results

The integration relies on a fixed timestep of `dt = 0.016f` (approx 60 FPS) to simulate real-world frame updates.

| Test Case | Rotation Axis | Angular Speed (rad/s) | Duration | Expected Total Rotation | Result |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **X-Axis 90°** | X (1, 0, 0) | $\pi/2$ (90°/s) | 1 sec | 90° around X | Pass |
| **Y-Axis 180°** | Y (0, 1, 0) | $\pi$ (180°/s) | 2 sec | 360° around Y (Identity) | Pass |
| **Z-Axis 270°** | Z (0, 0, 1) | $1.5\pi$ (270°/s)| 3 sec | 810° (Effectively 90°) around Z| Pass |
| **Combo X/Y** | XY (0.707, 0.707, 0) | $\pi/2$ (90°/s)| 4 sec | 360° around diagonal (Identity)| Pass |

### Reflection
Implementing rotational integration via Euler integration revealed how sensitive matrices are to floating-point drift over time. In the 4-second combination test, the final matrix drifted slightly from a perfect identity matrix, requiring a slightly relaxed test tolerance (1e-2f).

Currently, the orientation is updated by continually multiplying rotation matrices. Over a long simulation time, this can lead to the orientation matrix losing its orthogonality due to accumulated floating-point errors. Moving forward, transitioning the orientation representation from a glm::mat3 to a Quaternion (glm::quat) would improve numerical stability and allow for easy normalization to prevent scaling or shearing artifacts during rotation.




##
### Q3. Reflect on the Different Approaches on Storing Orientation

**Current Approach: 3x3 Rotation Matrix (`glm::mat3`)**
Orientation is currently stored as a 3x3 rotation matrix.
* **Advantages:** Can be applied directly to points; supported natively by graphics tools (like Vulkan); avoids gimbal lock.
* **Disadvantages:** Uses more memory (9 floats); very prone to decimal errors over time. These errors cause the object to stretch or squash, and fixing this requires slow and complex maths (orthogonalisation).

**Alternative 1: Euler Angles (`glm::vec3`)**
Orientation is stored as three separate rotations (Pitch, Yaw, Roll).
* **Advantages:** Very easy to read and edit; uses very little memory (3 floats).
* **Disadvantages:** Suffers from gimbal lock (where rotation axes overlap and get stuck); the order of rotation changes the final result; creates unnatural, looping movement when blending between angles.

**Alternative 2: Quaternions (`glm::quat`)**
Orientation is stored using a 4D maths system.
* **Advantages:** Immune to gimbal lock; allows perfectly smooth blending between rotations; very quick and easy to correct (normalise) to prevent errors; uses little memory (4 floats).
* **Disadvantages:** The maths is very hard to read or edit manually; must be converted into a 4x4 matrix before it can be drawn on screen.

**Reflection:**
While the 3x3 matrix method is simple to set up and fits well with the Vulkan renderer, its weakness to decimal errors makes it unstable for long physics simulations. Changing to Quaternions is the best choice. They provide strong maths accuracy, stop gimbal lock, and are easy to correct, ensuring the object's shape does not distort over time.
