#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for C++ classes
typedef struct btCollisionWorld btCollisionWorld;
typedef struct btVector3 btVector3;

// Struct to represent a 3D position
typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

// Struct to represent a bounding box
typedef struct {
    vec3_t min;
    vec3_t max;
} bbox_t;

// Initialize the physics simulation
void physics_init();

// Create a dynamic rigid body for a character
void create_character_rigid_body(const vec3_t position, const bbox_t bbox);

// Update the position and orientation of the character rigid body
void update_character_transform(const vec3_t position, const float rotation_angle);

// Perform collision detection and update the physics simulation
void perform_collision_detection();

// Get the character's current position
vec3_t get_character_position();

#ifdef __cplusplus
}
#endif

