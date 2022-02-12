/* Float Math */

float SafeDivide(float a, float b)
{
    return (b != 0.0) ? a / b : 0.0;
}

/* fmod function compatible with OSL using nvidia reference example. */
float CompatibleFmod(float a, float b)
{
    float c = (b != 0.0) ? fract(abs(a / b)) * abs(b) : 0.0;
    return (a < 0.0) ? -c : c;
}

float CompatiblePow(float x, float y)
{
    if (y == 0.0) { /* x^0 -> 1, including 0^0 */
        return 1.0;
    }

    /* glsl pow doesn't accept negative x */
    if (x < 0.0) {
        if (mod(-y, 2.0) == 0.0) {
            return pow(-x, y);
        }
        else {
            return -pow(-x, y);
        }
    }
    else if (x == 0.0) {
        return 0.0;
    }

    return pow(x, y);
}

float Wrap(float a, float b, float c)
{
    float range = b - c;
    return (range != 0.0) ? a - (range * floor((a - c) / range)) : c;
}

vec3 wrap(vec3 a, vec3 b, vec3 c)
{
    return vec3(Wrap(a.x, b.x, c.x), Wrap(a.y, b.y, c.y), Wrap(a.z, b.z, c.z));
}

float Hypot(float x, float y)
{
    return sqrt(x * x + y * y);
}

int FloorToInt(float x)
{
    return int(floor(x));
}

int QuickFloor(float x)
{
    return int(x) - ((x < 0) ? 1 : 0);
}

/* Vector Math */

vec2 SafeDivide(vec2 a, vec2 b)
{
    return vec2(SafeDivide(a.x, b.x), SafeDivide(a.y, b.y));
}

vec3 SafeDivide(vec3 a, vec3 b)
{
    return vec3(SafeDivide(a.x, b.x), SafeDivide(a.y, b.y), SafeDivide(a.z, b.z));
}

vec4 SafeDivide(vec4 a, vec4 b)
{
    return vec4(SafeDivide(a.x, b.x), SafeDivide(a.y, b.y), SafeDivide(a.z, b.z), SafeDivide(a.w, b.w));
}

vec2 SafeDivide(vec2 a, float b)
{
    return (b != 0.0) ? a / b : vec2(0.0);
}

vec3 SafeDivide(vec3 a, float b)
{
    return (b != 0.0) ? a / b : vec3(0.0);
}

vec4 SafeDivide(vec4 a, float b)
{
    return (b != 0.0) ? a / b : vec4(0.0);
}

vec3 CompatibleFmod(vec3 a, vec3 b)
{
    return vec3(CompatibleFmod(a.x, b.x), CompatibleFmod(a.y, b.y), CompatibleFmod(a.z, b.z));
}

vec3 InvertZ(vec3 v)
{
    v.z = -v.z;
    return v;
}

vec3 VectorNormalize(vec3 normal )
{
    return normalize(normal);
}

/* Matirx Math */

mat3 EulerToMat3(vec3 euler)
{
    float cx = cos(euler.x);
    float cy = cos(euler.y);
    float cz = cos(euler.z);
    float sx = sin(euler.x);
    float sy = sin(euler.y);
    float sz = sin(euler.z);

    mat3 mat;
    mat[0][0] = cy * cz;
    mat[0][1] = cy * sz;
    mat[0][2] = -sy;

    mat[1][0] = sy * sx * cz - cx * sz;
    mat[1][1] = sy * sx * sz + cx * cz;
    mat[1][2] = cy * sx;

    mat[2][0] = sy * cx * cz + sx * sz;
    mat[2][1] = sy * cx * sz - sx * cz;
    mat[2][2] = cy * cx;
    return mat;
}

vec3 DirectionTransformM4V3(vec3 vin, mat4 mat )
{
    return (mat * vec4(vin, 0.0)).xyz;
}

vec3 NormalTransformTransposedM4V3(vec3 vin, mat4 mat )
{
    return transpose(mat3(mat)) * vin;
}

vec3 PointTransformM4V3(vec3 vin, mat4 mat )
{
    return (mat * vec4(vin, 1.0)).xyz;
}
