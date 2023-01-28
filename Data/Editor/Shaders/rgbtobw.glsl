#ifndef RGB_TO_BW_GLSL
#define RGB_TO_BW_GLSL

float RGBtoBW(vec3 col)
{
	return (col.r + col.g + col.b)/3.f;
}

#endif RGB_TO_BW_GLSL
