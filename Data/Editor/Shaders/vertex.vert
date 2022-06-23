#version 450

layout(set=0, binding=0) uniform ModelData {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 viewPos;
    int displayDebugTarget;
} ;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec4 inTangent;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec4 fragWorldPos;

void main()
{
    gl_Position = modelBufferObject.proj * modelBufferObject.view * modelBufferObject.model * vec4(inPosition, 1.0f);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = normalize( mat3(modelBufferObject.model) * inNormal);
    fragTangent = normalize( mat3(modelBufferObject.model) * inTangent.xyz) * inTangent.w;
    fragWorldPos = modelBufferObject.model * vec4(inPosition, 1.0f);
}