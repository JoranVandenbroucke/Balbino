//https://www.3dgep.com/forward-plus/#Forward
#ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 8.")
#define NUM_LIGHTS 8// should be defined by the application.
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

// Parameters required to convert screen space coordinates to view space params.
layout (set = 0, binding = 0) readonly buffer ScreenToViewParams
{
    mat4 InverseProjection;
    vec2 ScreenDimensions;
};

// Convert clip space coordinates to view space
vec4 ClipToView(vec4 clip)
{
    // View space position.
    vec4 view = InverseProjection * clip;
    // Perspecitive projection.
    view = view / view.w;

    return view;
}

// Convert screen space coordinates to view space.
vec4 ScreenToView(vec4 screen)
{
    // Convert to normalized texture coordinates
    vec2 texCoord = screen.xy / ScreenDimensions;

    // Convert to clip space
    vec4 clip = vec4(vec2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

    return ClipToView(clip);
}

struct Light
{
/**
* Position for point and spot lights (World space).
*/
    vec4   PositionWS;
//--------------------------------------------------------------( 16 bytes )
/**
* Direction for spot and directional lights (World space).
*/
    vec4   DirectionWS;
//--------------------------------------------------------------( 16 bytes )
/**
* Position for point and spot lights (View space).
*/
    vec4   PositionVS;
//--------------------------------------------------------------( 16 bytes )
/**
* Direction for spot and directional lights (View space).
*/
    vec4   DirectionVS;
//--------------------------------------------------------------( 16 bytes )
/**
* Color of the light. Diffuse and specular colors are not seperated.
*/
    vec4   Color;
//--------------------------------------------------------------( 16 bytes )
/**
* The half angle of the spotlight cone.
*/
    float    SpotlightAngle;
/**
* The range of the light.
*/
    float    Range;

/**
 * The intensity of the light.
 */
    float    Intensity;

/**
* Disable or enable the light.
*/
    bool    Enabled;
//--------------------------------------------------------------( 16 bytes )

/**
 * Is the light selected in the editor?
 */
    bool    Selected;

/**
* The type of the light.
*/
    uint    Type;
    vec2  Padding;
//--------------------------------------------------------------( 16 bytes )
//--------------------------------------------------------------( 16 * 7 = 112 bytes )
};

struct Plane
{
    vec3 N;// Plane normal.
    float  d;// Distance to origin.
};

// Compute a plane from 3 noncollinear points that form a triangle.
// This equation assumes a right-handed (counter-clockwise winding order)
// coordinate system to determine the direction of the plane normal.
Plane ComputePlane(vec3 p0, vec3 p1, vec3 p2)
{
    Plane plane;

    vec3 v0 = p1 - p0;
    vec3 v2 = p2 - p0;

    plane.N = normalize(cross(v0, v2));

    // Compute the distance to the origin using p0.
    plane.d = dot(plane.N, p0);

    return plane;
}

// Four planes of a view frustum (in view space).
// The planes are:
//  * Left,
//  * Right,
//  * Top,
//  * Bottom.
// The back and/or front planes can be computed from depth values in the
// light culling compute shader.
struct Frustum
{
    Plane planes[4];// left, right, top, bottom frustum planes.
};

struct Sphere
{
    vec3 c;// Center point.
    float  r;// Radius.
};

// Check to see if a sphere is fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool SphereInsidePlane(Sphere sphere, Plane plane)
{
    return dot(plane.N, sphere.c) - plane.d < -sphere.r;
}

// Check to see of a light is partially contained within the frustum.
bool SphereInsideFrustum(Sphere sphere, Frustum frustum, float zNear, float zFar)
{
    bool result = true;

    // First check depth
    // Note: Here, the view vector points in the -Z axis so the
    // far depth value will be approaching -infinity.
    if (sphere.c.z - sphere.r > zNear || sphere.c.z + sphere.r < zFar)
    {
        result = false;
    }

    // Then check frustum planes
    for (int i = 0; i < 4 && result; i++)
    {
        if (SphereInsidePlane(sphere, frustum.planes[i]))
        {
            result = false;
        }
    }

    return result;
}

struct Cone
{
    vec3 T;// Cone tip.
    float  h;// Height of the cone.
    vec3 d;// Direction of the cone.
    float  r;// bottom radius of the cone.
};

// Check to see if a point is fully behind (inside the negative halfspace of) a plane.
bool PointInsidePlane(vec3 p, Plane plane)
{
    return dot(plane.N, p) - plane.d < 0;
}

// Check to see if a cone if fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool ConeInsidePlane(Cone cone, Plane plane)
{
    // Compute the farthest point on the end of the cone to the positive space of the plane.
    vec3 m = cross(cross(plane.N, cone.d), cone.d);
    vec3 Q = cone.T + cone.d * cone.h - m * cone.r;

    // The cone is in the negative halfspace of the plane if both
    // the tip of the cone and the farthest point on the end of the cone to the
    // positive halfspace of the plane are both inside the negative halfspace
    // of the plane.
    return PointInsidePlane(cone.T, plane) && PointInsidePlane(Q, plane);
}

bool ConeInsideFrustum(Cone cone, Frustum frustum, float zNear, float zFar)
{
    bool result = true;

    Plane nearPlane = { vec3(0, 0, -1), -zNear };
    Plane farPlane = { vec3(0, 0, 1), zFar };

    // First check the near and far clipping planes.
    if (ConeInsidePlane(cone, nearPlane) || ConeInsidePlane(cone, farPlane))
    {
        result = false;
    }

    // Then check frustum planes
    for (int i = 0; i < 4 && result; i++)
    {
        if (ConeInsidePlane(cone, frustum.planes[i]))
        {
            result = false;
        }
    }

    return result;
}