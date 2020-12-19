#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;
layout (location = 5) in vec3 texWeights;
layout (location = 6) in vec4 boneWeights;
layout (location = 7) in ivec4 boneIndices;

out vec4 oFragPos;
out vec3 oNormal;
out vec2 oTexCoords;
out vec3 oTexWeights;
out vec3 vertexViewDir;
out mat3 vertexTBN;
out vec3 vertexDebugColor;

uniform int uNumLights;
uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uBones[64];
uniform vec3 uEye;
uniform bool uDisableBones;
uniform vec4 uClipPlane;

void main() {
  vec4 bonePosition = vec4(position, 1.0f);
  vec3 outNormal = normal;
  if (uDisableBones == false) {
    mat4 boneTransform = uBones[boneIndices[0]] * boneWeights[0];
    boneTransform += uBones[boneIndices[1]] * boneWeights[1];
    boneTransform += uBones[boneIndices[2]] * boneWeights[2];
    boneTransform += uBones[boneIndices[3]] * boneWeights[3];
    bonePosition = boneTransform * bonePosition;
    outNormal = (boneTransform * vec4(normal, 0)).xyz;
  }

  vec4 fragPos = uModel * bonePosition;
  gl_Position = uProjection * uView * fragPos;
  gl_ClipDistance[0] = dot(fragPos, uClipPlane);
  oFragPos = fragPos;
  oTexCoords = texCoords;
  oTexWeights = texWeights;
  oNormal = outNormal;
  vertexViewDir = normalize(uEye - fragPos.xyz);
  vertexDebugColor = tangent;

  vec3 vertTangent_CameraSpace = normalize((uModel * vec4(tangent, 0.0)).xyz);
  vec3 vertBitangent_CameraSpace = normalize((uModel * vec4(bitangent, 0.0)).xyz);
  vec3 vertexNormal_CameraSpace = normalize((uModel * vec4(oNormal, 0.0)).xyz);
  vertexTBN = mat3(
      vertTangent_CameraSpace,
      vertBitangent_CameraSpace,
      vertexNormal_CameraSpace
  );
}
