#include "default.glsl"
vec3 gradient(vec3 positoin, vec3 col1, vec3 col2, float type){
{
    float x, y, z;

    x = p[0];
    y = p[1];
    z = p[2];

    float result = 0.0;

    if (type < 1) {
        result = x;
    }
    else if (type < 2) {
        result = r * r;
    }
    else if (type < 3) {
        float t = r * r;

        result = (3.0 * t - 2.0 * t * r);
    }
    else if (type < 4) {
        result = (x + y) * 0.5;
    }
    else if (type < 5) {
        result = atan2(y, x) / BALBINO_2PI + 0.5;
    }
    else {
        float r = 1.0 - sqrt(x * x + y * y + z * z);

        if (type < 6)
            result = r * r;
        else if (type < 7)
            result = r;
    }

    return mix(col1,col2,clamp(result, 0.0, 1.0));
}