#version 300 es
precision highp float;

layout(location = 0) in vec2 aPos;
out vec2 uv;
uniform mat3 transform;

void main() {
  uv = aPos * vec2(0.5, -0.5) + vec2(0.5);
  vec3 transformedPos = transform * vec3(aPos, 1.0);
  gl_Position = vec4(transformedPos.xy, 0.0, 1.0);
}
