#ifndef NODE_TEXTURE_IMAGE_GLSL
#define NODE_TEXTURE_IMAGE_GLSL
void texture_sample_1d(in sampler1D tex, in float u, out vec4 color, out float alpha)
{
    color = texture(tex, u);
    alpha = color.a;
}
void texture_sample_2d(in sampler2D tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_3d(in sampler3D tex, vec3 uvw, out vec4 color, out float alpha)
{
    color = texture(tex, uvw);
    alpha = color.a;
}
void texture_sample_cube(in samplerCube tex, vec3 uvw, out vec4 color, out float alpha)
{
    color = texture(tex, uvw);
    alpha = color.a;
}
void texture_sample_2d_rect(in sampler2DRect tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_1d_array(in sampler1DArray tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_2d_array(in sampler2DArray tex, vec3 uvw, out vec4 color, out float alpha)
{
    color = texture(tex, uvw);
    alpha = color.a;
}
void texture_sample_cube_array(in samplerCubeArray tex, vec4 uvwx, out vec4 color, out float alpha)
{
    color = texture(tex, uvwx);
    alpha = color.a;
}

void texture_sample_1d_shadow(in sampler1D tex, in float u, out vec4 color, out float alpha)
{
    color = texture(tex, u);
    alpha = color.a;
}
void texture_sample_2d_shadow(in sampler2D tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_cube_shadow(in samplerCube tex, vec3 uvw, out vec4 color, out float alpha)
{
    color = texture(tex, uvw);
    alpha = color.a;
}
void texture_sample_2d_rect_shadow(in sampler2DRect tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_1d_array_shadow(in sampler1DArray tex, vec2 uv, out vec4 color, out float alpha)
{
    color = texture(tex, uv);
    alpha = color.a;
}
void texture_sample_2d_array_shadow(in sampler2DArray tex, vec3 uvw, out vec4 color, out float alpha)
{
    color = texture(tex, uvw);
    alpha = color.a;
}
void texture_sample_cube_array_shadow(in samplerCubeArray tex, vec4 uvwx, out vec4 color, out float alpha)
{
    color = texture(tex, uvwx);
    alpha = color.a;
}
#endif// NODE_TEXTURE_IMAGE_GLSL
