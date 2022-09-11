void RgbToHsv(vec3 rgb, out vec3 hsv)
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
void HsvToRgb(vec3 hsv, out vec3 rgb)
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
vec3 HueSatVal(float hue, float sat, float value, float fac, vec3 col)
{
  vec3 hsv;
  vec3 outCol;

  RgbToHsv(col, hsv);

  hsv[0] = fract(hsv[0] + hue + 0.5);
  hsv[1] = clamp(hsv[1] * sat, 0.0, 1.0);
  hsv[2] = hsv[2] * value;

  HsvToRgb(hsv, outCol);

  return mix(col, outCol, fac);
}
vec3 Invert(vec3 col, float fac)
{
return mix(vec3( 1 - col.r, 1 - col.g, 1 - col.b), col, fac);
}
vec3 Add(vec3 col1, vec3 col2, float fac)
{
    return mix(col1 + col2, col1, fac);
}
vec3 Subtract(vec3 col1, vec3 col2, float fac)
{
    return mix(col1 - col2, col1, fac);
}
vec3 Multiply(vec3 col1, vec3 col2, float fac)
{
    return mix(col1 * col2, col1, fac);
}
vec3 Screen(vec3 col1, vec3 col2, float fac)
{
    return mix(1-(1-col1)*(1-col2), col1, fac);
}
vec3 Divide(vec3 col1, vec3 col2, float fac)
{
    return mix(col1 / col2, col1, fac);
}
vec3 Difference(vec3 col1, vec3 col2, float fac)
{
    return mix(abs(col1 - col2), col1, fac);
}
vec3 Darken(vec3 col1, vec3 col2, float fac)
{
    return mix(vec3( min(col1.r, col2.r), min(col1.g, col2.g), min(col1.b, col2.b) ), col1, fac);
}
vec3 Lighten(vec3 col1, vec3 col2, float fac)
{
    return mix(vec3( max(col1.r, col2.r), max(col1.g, col2.g), max(col1.b, col2.b) ), col1, fac);
}
vec3 Overlay(vec3 col1, vec3 col2, float fac)
{
    vec3 outputColor;
    if(col1.r < 0.5f)
        outputColor.r = 2 * col1.r * col2.r;
    else
        outputColor.r = 1-2 * (1-col1.r) * (1-col2.r);
        
    if(col1.g < 0.5f)
        outputColor.g = 2 * col1.g * col2.g;
    else
        outputColor.g = 1-2 * (1-col1.g) * (1-col2.g);
        
    if(col1.b < 0.5f)
        outputColor.b = 2 * col1.b * col2.b;
    else
        outputColor.b = 1-2 * (1-col1.b) * (1-col2.b);

    return mix(outputColor, col1, fac);
}
vec3 ColorDodge(vec3 col1, vec3 col2, float fac)
{
    return mix(col2/(1-col1), col1, fac);
}
vec3 ColorBurn(vec3 col1, vec3 col2, float fac)
{
    return mix((1-col2)/col1, col1, fac);
}
vec3 Hue(vec3 col1, vec3 col2, float fac)
{
    vec3 outCol = col1;
    vec3 hsv2;
    RgbToHsv(col2, hsv2);

    if (hsv2.y != 0.0) {
        vec3 hsv;
        RgbToHsv(outCol, hsv);
        hsv.x = hsv2.x;
        vec3 tmp;
        HsvToRgb(hsv, tmp);
        
        outCol = mix(outCol, tmp, fac);
    }

    return outCol;
}
vec3 Saturation(vec3 col1, vec3 col2, float fac)
{
    float tm = 1.0 - fac;
    
    vec3 outCol = col1;
    
    vec3 hsv;
    RgbToHsv(outCol, hsv);

    if (hsv[1] != 0.0) {
        vec3 hsv2;
        RgbToHsv(col2, hsv2 );

        hsv[1] = tm * hsv[1] + fac * hsv2[1];
        HsvToRgb(hsv,outCol);
    }

    return outCol;
}
vec3 Value(vec3 col1, vec3 col2, float fac)
{
    float tm = 1.0 - fac;

    vec3 hsv;
    vec3 hsv2;
    RgbToHsv(col1, hsv);
    RgbToHsv(col2, hsv2);
    
    hsv[2] = tm * hsv[2] + fac * hsv2[2];
    vec3 outCol;
    HsvToRgb(hsv, outCol);
    return outCol;
}
vec3 ColorMix(vec3 col1, vec3 col2, float fac)
{
    vec3 outCol;
    vec3 hsv2;
    RgbToHsv(col2, hsv2);
    if(col2.x > 0.5f)
    {
        vec3 hsv;
        RgbToHsv(col1, hsv);
        
        hsv[2] = hsv2[2];
        hsv[2] = hsv2[2];
        vec3 tmp;
        HsvToRgb(hsv, tmp);
        outCol = mix(outCol, tmp, fac);
    }
    return outCol;
}
vec3 SoftLight(vec3 col1, vec3 col2, float fac)
{
    float tm = 1.0 - fac;
    
    vec3 one = vec3(1.0,1.0,1.0);
    vec3 scr = one - (one - col2) * (one - col1);
    
    return tm * col1 + fac * ((one - col1) * col2 * col1 + col1 * scr);
}
vec3 LinearLight(vec3 col1, vec3 col2, float fac)
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

  return outcol;
}