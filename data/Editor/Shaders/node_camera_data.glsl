void camera_data(out vec3 direction, out float depth, out float z) {
    vec3 dir = modelBufferObject.viewPos.xyz - fragWorldPos.xyz;
    outview = normalize(dir);
    depth = length(dir);
    z = abs(dir.z);
}
