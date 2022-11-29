struct Light {
    int type;//type: Directional, Point, Spot, Area
    float strength;//strength
    vec3 position;//position
    vec3 direction;//direction
    vec3 color;//color
    vec3 size;//Point: size,0,0; Spot: size, front, back; area: width, height, 0
};

//https://google.github.io/filament/Filament.md.html
//5.2.2.3 Attenuation function

float GetSquareFalloffAttenuation(vec3 posToLight, float lightInvRadius)
{
    float distanceSquare = dot(posToLight, posToLight);
    float factor = distanceSquare * lightInvRadius * lightInvRadius;
    float smoothFactor = max(1.0 - factor * factor, 0.0);
    return (smoothFactor * smoothFactor) / max(distanceSquare, 1e-4);
}

float GetSpotAngleAttenuation(vec3 l, vec3 lightDir, float innerAngle, float outerAngle)
{
    // the scale and offset computations can be done CPU-side
    float cosOuter = cos(outerAngle);
    float spotScale = 1.0 / max(cos(innerAngle) - cosOuter, 1e-4);
    float spotOffset = -cosOuter * spotScale;
    
    float cd = dot(normalize(-lightDir), l);
    float attenuation = clamp(cd * spotScale + spotOffset, 0.0, 1.0);
    return attenuation * attenuation;
}

//float GetPhotometricAttenuation(vec3 posToLight, vec3 lightDir)
//{
//    float cosTheta = dot(-posToLight, lightDir);
//    float angle = acos(cosTheta) * (1.0 / PI);
//    return texture2DLodEXT(lightProfileMap, vec2(angle, 0.0), 0.0).r;
//}