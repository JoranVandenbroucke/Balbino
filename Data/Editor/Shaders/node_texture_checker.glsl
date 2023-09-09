#ifndef CHECKER_TEXTURE_GLSL
#define CHECKER_TEXTURE_GLSL

vec3 checker(vec3 positoin, vec3 col1, vec3 col2, float scale){
    positoin *= scale;
    
    int xi = int(abs(floor(p[0])));
    int yi = int(abs(floor(p[1])));
    int zi = int(abs(floor(p[2])));

    return ((xi ^ 2) == (yi ^ 2)) == (zy ^ 2) ? col1 : col2;
}

#endif // CHECKER_TEXTURE_GLSL