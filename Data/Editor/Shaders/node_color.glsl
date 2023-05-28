#ifndef COLOR_MATH_GLSL
#define COLOR_MATH_GLSL

void color_rgb_to_bw(vec3 col, out float bw)
{
    bw = (col.r + col.g + col.b)/3.f;
}

void color_gamma(vec3 color, float gamma, out vec3 outColor)
{
    outColor = pow(color, gamma);
}
void color_brightness_contrast(vec3 color, float brightness, float contrast, out vec3 outColor)
{
    outColor = color*contrast+brightness;
}
void color_rgb_to_hsv(vec3 rgb, out vec3 hsv)
{
    float k = 0.0f;
    float chroma;
    float minGB;
    
    if (rgb.g < rgb.b)
    {
        float t = rgb.g;
        rgb.g = rgb.b;
        rgb.b = t;
        k = -1.0f;
    }
    minGB = rgb.b;
    if (rgb.r < rgb.g) {
        float t = rgb.r;
        rgb.r = rgb.g;
        rgb.g = t;
        k = -2.0f / 6.0f - k;
        minGB = min(rgb.g, rgb.b);
    }
    
    chroma = rgb.r - minGB;
    
    hsv.x = abs(k + (rgb.g - rgb.b) / (6.0f * chroma + 1e-20f));
    hsv.y = chroma / (rgb.r + 1e-20f);
    hsv.z = rgb.r;
}
void color_hsv_to_rgb(vec3 hsv, out vec3 rgb)
{
    float nr, ng, nb;
    
    nr = abs(hsv.x * 6.0f - 3.0f) - 1.0f;
    ng = 2.0f - abs(hsv.x * 6.0f - 2.0f);
    nb = 2.0f - abs(hsv.x * 6.0f - 4.0f);
    
    clamp(nr, 0.0f, 1.0f);
    clamp(nb, 0.0f, 1.0f);
    clamp(ng, 0.0f, 1.0f);
    
    rgb.r = ((nr - 1.0f) * hsv.y + 1.0f) * hsv.z;
    rgb.g = ((ng - 1.0f) * hsv.y + 1.0f) * hsv.z;
    rgb.b = ((nb - 1.0f) * hsv.y + 1.0f) * hsv.z;
}
void color_hue_sat_val(vec3 color, float hue, float sat, float value, float fac, out vec3 outColor)
{
    vec3 hsv;
    
    color_rgb_to_hsv(color, hsv);
    
    hsv[0] = fract(hsv[0] + hue + 0.5);
    hsv[1] = clamp(hsv[1] * sat, 0.0, 1.0);
    hsv[2] = hsv[2] * value;
    
    hsv_to_rgb(hsv, outColor);
    outColor = mix(col, outColor, fac);
}
void color_invert(vec3 col, float fac, out vec3 outColor)
{
    outColor = mix(vec3(1 - col.r, 1 - col.g, 1 - col.b), col, fac);
}
void color_add(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(col1 + col2, col1, fac);
}
void color_subtract(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(col1 - col2, col1, fac);
}
void color_multiply(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(col1 * col2, col1, fac);
}
void color_screen(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(1-(1-col1)*(1-col2), col1, fac);
}
void color_divide(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(col1 / col2, col1, fac);
}
void color_difference(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(abs(col1 - col2), col1, fac);
}
void color_darken(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(vec3(min(col1.r, col2.r), min(col1.g, col2.g), min(col1.b, col2.b)), col1, fac);
}
void color_lighten(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(vec3(max(col1.r, col2.r), max(col1.g, col2.g), max(col1.b, col2.b)), col1, fac);
}
void color_overlay(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    vec3 outputColor;
    if (col1.r < 0.5f)
    outputColor.r = 2 * col1.r * col2.r;
    else
    outputColor.r = 1-2 * (1-col1.r) * (1-col2.r);
    
    if (col1.g < 0.5f)
    outputColor.g = 2 * col1.g * col2.g;
    else
    outputColor.g = 1-2 * (1-col1.g) * (1-col2.g);
    
    if (col1.b < 0.5f)
    outputColor.b = 2 * col1.b * col2.b;
    else
    outputColor.b = 1-2 * (1-col1.b) * (1-col2.b);
    
    outColor = mix(outputColor, col1, fac);
}
void color_color_dodge(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix(col2/(1-col1), col1, fac);
}
void color_color_burn(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    outColor = mix((1-col2)/col1, col1, fac);
}
void color_hue(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    vec3 outCol = col1;
    vec3 hsv2;
    color_rgb_to_hsv(col2, hsv2);
    
    if (hsv2.y != 0.0) {
        vec3 hsv;
        color_rgb_to_hsv(outCol, hsv);
        hsv.x = hsv2.x;
        vec3 tmp;
        color_hsv_to_rgb(hsv, tmp);
        
        outCol = mix(outCol, tmp, fac);
    }
    
    outColor = outCol;
}
void color_saturation(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    float tm = 1.0 - fac;
    
    vec3 outCol = col1;
    
    vec3 hsv;
    color_rgb_to_hsv(outCol, hsv);
    
    if (hsv[1] != 0.0) {
        vec3 hsv2;
        color_rgb_to_hsv(col2, hsv2);
        
        hsv[1] = tm * hsv[1] + fac * hsv2[1];
        hsv_to_rgb(hsv, outCol);
    }
    
    outColor = outCol;
}
void color_value(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    float tm = 1.0 - fac;
    
    vec3 hsv;
    vec3 hsv2;
    color_rgb_to_hsv(col1, hsv);
    color_rgb_to_hsv(col2, hsv2);
    
    hsv[2] = tm * hsv[2] + fac * hsv2[2];
    vec3 outCol;
    hsv_to_rgb(hsv, outCol);
    outColor = outCol;
}
void color_color_mix(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    vec3 outCol;
    vec3 hsv2;
    color_rgb_to_hsv(col2, hsv2);
    if (col2.x > 0.5f)
    {
        vec3 hsv;
        color_rgb_to_hsv(col1, hsv);
        
        hsv[2] = hsv2[2];
        hsv[2] = hsv2[2];
        vec3 tmp;
        hsv_to_rgb(hsv, tmp);
        outCol = mix(outCol, tmp, fac);
    }
    outColor = outCol;
}
void color_soft_light(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    float tm = 1.0 - fac;
    
    vec3 one = vec3(1.0, 1.0, 1.0);
    vec3 scr = one - (one - col2) * (one - col1);
    
    outColor = tm * col1 + fac * ((one - col1) * col2 * col1 + col1 * scr);
}
void color_linear_light(vec3 col1, vec3 col2, float fac, out vec3 outColor)
{
    vec3 outcol = col1;
    
    if (col2[0] > 0.5)
    outcol[0] = col1[0] + fac * (2.0 * (col2[0] - 0.5));
    else
    outcol[0] = col1[0] + fac * (2.0 * (col2[0]) - 1.0);
    
    if (col2[1] > 0.5)
    outcol[1] = col1[1] + fac * (2.0 * (col2[1] - 0.5));
    else
    outcol[1] = col1[1] + fac * (2.0 * (col2[1]) - 1.0);
    
    if (col2[2] > 0.5)
    outcol[2] = col1[2] + fac * (2.0 * (col2[2] - 0.5));
    else
    outcol[2] = col1[2] + fac * (2.0 * (col2[2]) - 1.0);
    
    outColor = outcol;
}

#endif// COLOR_MATH_GLSL