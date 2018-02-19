#version 410

layout (location = 0) in vec2 position;

uniform mat3 uView;
uniform mat4 uProjection;
smooth out vec3 oViewDir;

void main() {
  vec3 unprojected = (uProjection * vec4(position, 1.0, 1.0)).xyz;
  oViewDir = uView * unprojected;
  
  gl_Position = vec4(position, 0.0, 1.0);
}