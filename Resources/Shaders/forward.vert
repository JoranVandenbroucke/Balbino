#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragBinormal;
layout(location = 5) out vec4 fragWorldPosition;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0f);
    fragTexCoord = inTexCoord;
    fragColor = inColor;
    fragWorldPosition = ubo.model * vec4(inPosition, 1.0f);
    fragNormal = normalize( mat3(ubo.model) * inNormal);
	fragTangent = normalize(mat3 (ubo.model) * inTangent );
	fragBinormal = normalize(mat3 (ubo.model) * inBitangent );
}