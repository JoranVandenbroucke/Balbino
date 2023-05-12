float PerezSky(float theta, float gamma)
{
    // CIE Standard Clear Sky, low atmospheric turbidity
    float A = -1.0;
    float B = -0.32;
    float C = 10;
    float D = -3;
    float E = 0.45;
    
    float cGamma2 = cos(gamma);
    cGamma2 = cGamma2 * cGamma2;
    
    return (1+A*exp(b/cos(theta)))*(1+C+exp(D*gamma)+ E * cGamma2);
}

float CIESky(float theta, float gamma)
{
    // CIE Standard Clear Sky, low atmospheric turbidity
    float A = -1.0;
    float B = -0.32;
    float C = 10;
    float D = -3;
    float E = 0.45;
    
    float cGamma2 = cos(gamma);
    cGamma2 = cGamma2 * cGamma2;
    
    return (1+A*exp(b/cos(theta)))*(1+C*(exp(D*gamma)+exp(D*PI_DIV2))+ E * cGamma2);
}

vec3 PrezLum(vec3 skyColor, float maxLumin)
{
    float theta = .05;// angle zinith and view
    float gamma = .78;//angle between sun and zinith
    float yota = .78;//angle between sun and zinith
    return PerezSky(theta, gamma)/PerezSky(0, yota)*maxLumin*skyColor;
}

vec3 CIELum(vec3 skyColor, float maxLumin)
{
    float theta = .05;// angle zinith and view
    float gamma = .78;//angle between sun and zinith
    float yota = .78;//angle between sun and zinith
    return CIESky(theta, gamma)/CIESky(0, yota)*maxLumin*skyColor;
}

float HosekWilliePhaseFunction(float g, float alpha)
{
    float cAlpha = cos(alpha);
    float cAlpha2 = cAlpha2 * cAlpha2;
    float g2 = g*g;
    float gg = g+g;
    return (1+cAlpha2)/pow(1+g*g-2*g*cAlpha, 1.5);
}

vec3 HosekWillieSky(float dence, float gass, float anisotropy, vec2 view, vec2 sun, float albedo)
{
    float turbidity = (dence + gass)/gass;
    
    // Calculate the sky's radiance using the Preetham model
    float A = 0.15 * turbidity - 0.50;
    float B = 0.50 * turbidity - 4.50;
    float C = 0.15 * turbidity - 2.50;
    float D = 0.50 * turbidity - 2.40;
    float E = 0.02 * turbidity - 0.40;
    float F = 0.02 * turbidity - 0.40;
    float G = 0.02 * turbidity - 0.40;
    float H = 0.02 * turbidity - 0.40;
    float I = 0.02 * turbidity - 0.40;
    
    float theta = .05;// angle zinith and view
    float gamma = .82;// angle view and sun
    float anisotropy = .22;
    float cGamma2 = cos(gamma);
    cGamma2 = cGamma2 * cGamma2;
    
    //    (1 + Ae (B/(cos(θ)+0.01)))*(C+pow(De,Eγ)+F pow(cos(γ),2) + G * χ(H, γ) + I * pow(cos(θ),0.5)
    vec3 prez = (1+A*exp(b/(cos(theta)+0.01)))*(C+D*exp(E*gamma)+ F * cGamma2+G*HosekWilliePhaseFunction(H, gamma)+I*pow(cos(theta), 0.5));
    return prez;
}