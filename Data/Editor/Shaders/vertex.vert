#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec4 inTangent;

// Instanced attributes
layout (location = 5) in mat4 instanceModelMatrix;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec4 fragTangent;
layout(location = 4) out vec4 fragWorldPos;

layout(set=0, binding=0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec4 viewPos;
    int displayDebugTarget;
} ubo;

void main()
{
    gl_Position = ubo.proj * ubo.view * instanceModelMatrix * vec4(inPosition, 1.0f);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = normalize(mat3(instanceModelMatrix) * inNormal);
    fragTangent = vec4(normalize(mat3(instanceModelMatrix) * inTangent.xyz), inTangent.w);
    fragWorldPos = instanceModelMatrix * vec4(inPosition, 1.0f);
}