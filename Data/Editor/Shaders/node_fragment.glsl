void fragment_specular(
vec4 inColor,
vec3 normal,
float metallic,
float perceptualRoughness,
float ambientOcclusion,
out vec4 outColor
)
{
    vec3 baseColor = (1.0 - metallic) * inColor.rgb;
    vec3 directColor = vec3(0.0);
    
    vec3 v = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
    
    float NdotV = abs(dot(normal, v)) + 1e-5;
    float roughness = perceptualRoughness * perceptualRoughness;
    
    vec3 f0 = 0.16 * sqr(roughness) * (1.0 - metallic) + baseColor * metallic;
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        Light light = lights[i];
        if(light.strength < 0)
        break;
        
        vec3 Fd = vec3(0, 0, 0);
        vec3 Fr = vec3(0.0);
        
        vec3 lightColor = light.color * light.strength;
        vec3 lightVector = light.position.xyz - fragWorldPos.xyz;
        vec3 l = normalize(lightVector);
        if(light.type != 0)
        {
            lightColor *= get_square_falloff_attenuation(lightVector,1/light.size.x);
            if(light.type == 2)
            lightColor *= get_spot_angle_attenuation(l, light.direction,light.size.y, light.size.z);
        }
        else
        {
            lightVector = light.direction;
            l = normalize(light.direction);
        }
        
        vec3 h = normalize(normal + l);
        float NdotL = max(dot(normal, l), 0.0);
        float NdotH = max(dot(normal, h), 0.0);
        float LdotH = max(dot(l, h), 0.0);
        float VdotH = max(dot(v, h), 0.0);
        
        // perceptually linear roughness to roughness (see parameterization)
        if (NdotL > 0.0)
        {
            float D = distribution(roughness, NdotH);
            vec3  F = fresnel(roughness, f0, VdotH );
            float V = shadow( NdotL, NdotV, NdotH, VdotH, roughness);
            
            // specular BRDF
            Fr = (D * V) * F;
        }
        
        // diffuse BRDF
        Fd = baseColor * difuse(NdotV, NdotL, LdotH, roughness);
        directColor+=(Fd + Fr) * (NdotL * ambientOcclusion) * light.color * light.strength;
    }
    outColor = vec4(directColor, inColor.a);
}