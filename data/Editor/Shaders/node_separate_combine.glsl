void separate_xyz(vec3 vector, out float outX, out float outY, out float outZ){
    outX = vector.x;
    outY = vector.y;
    outZ = vector.z;
}
void separate_rgb(color, out float outR, out float outG, out float outB){
    outR = vector.r;
    outG = vector.g;
    outB = vector.b;
}
void combine_xyz( float x, float y, float z, out vec3 outVector){
    outVector = vec3(x,y,z);
}
void combine_rgb(float r, float g, float b, vec4 outColor){
    outColor = vec4(r,g,b,1);
}
