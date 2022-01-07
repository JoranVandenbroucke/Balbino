//  https://thebookofshaders.com/09/
vec3 BrickTile(vec3 point, float size, float offset, float offsetFrequency, float squash, float squashFrequecy){
    point *= size;

//  Here is where the offset is happening
//  point.x += step(1.0, mod(point.y,2.0)) * offset;
//  point.x = (rownum % offset_frequency) ? 0.0 : (brick_width * offset_amount); /* offset */
    
  	float rownum = floor(point[1]);
    point.x *= mod(rownum, squashFrequecy)!= 0. ? 1.0 : squash;           /* squash */
    point.x += mod(rownum, offsetFrequency) != 0.  ? 0.0 : offset; /* offset */
    return fract(point);
}

float Box(vec3 point, vec3 size){
    size = vec3( thickness ) - size * thickness;

    vec3 uv = smoothstep( size, size + vec3( 1e-4 ), point);
    uv *= smoothstep( size, size + vec3( 1e-4 ), vec3( 1.0 ) - point);

    return uv.x * uv.y * uv.z;
}

vec3 Brick(vec3 point, vec3 color1, vec3 color2, float morterSize, float size, float offset, float offsetFrequency, float squash, float squashFrequecy)
{
    point = brickTile(point, size, offset, offsetFrequency, squash, squashFrequecy);
    return mix( color1, color2, box( st, vec3(1 - morterSize) ));
}