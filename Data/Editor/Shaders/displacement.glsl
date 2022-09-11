vec3 Displacement( float height, float midlevel, float scale, vec3 normal )
{
    return (height - midlevel) * scale * normalize(normal);
}