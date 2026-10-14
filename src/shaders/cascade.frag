#version 300 es
precision highp float;
#define PI 3.14159265359

struct PointLight {
  vec2 pos;
  float radius;
  vec3 color;
};

struct CascadeInfo {
  int mazeSize;
  vec2 screenSize;
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  vec3 backgroundColor;
  int numLights;
};

uniform sampler2D wallTex;
uniform CascadeInfo cascadeInfo;
layout(std140) uniform LightBlock {
  PointLight lights[64];
};

out vec4 FragColor;

vec4 castRay(vec2 startPos, float angle, float minLength, float maxLength);
float castMaze(vec2 start, vec2 dir, float maxLen);
vec4 intersectCircleLight(vec2 startPos, vec2 rayDir, float minLength, float maxLength, PointLight light);
vec2 toUV(vec2 coord);

void main() {
  float linearInterval = 2.0 * exp2(float(cascadeInfo.cascadeLevel));
  float rayInterval = 2.0 * PI / (linearInterval * linearInterval);

  vec2 probeCenter = (floor(gl_FragCoord.xy / linearInterval) + 0.5) * linearInterval;

  int fragX = int(gl_FragCoord.x);
  int fragY = int(gl_FragCoord.y);

  int intervalInt = int(linearInterval);
  int rayNum = (fragX % intervalInt) + intervalInt * (fragY % intervalInt);

  float rayAngle = (float(rayNum) + 0.5) * rayInterval;

  float minLength = pow(4.0, float(cascadeInfo.cascadeLevel));
  float maxLength = pow(4.0, float(cascadeInfo.cascadeLevel + 1));

  FragColor = vec4(castRay(probeCenter, rayAngle, minLength, maxLength).rgba);
  return;
}

vec4 castRay(vec2 startPos, float angle, float minLength, float maxLength) {
  vec2 rayDir = vec2(cos(angle), sin(angle));
  float mazeDist = castMaze(startPos, rayDir, maxLength);
  bool hitWall = mazeDist < maxLength - 0.01;

  for (int i = 0; i < 64; i++) {
    if (i >= cascadeInfo.numLights) break;
    vec4 hit = intersectCircleLight(startPos, rayDir, minLength, mazeDist, lights[i]);
    if (hit.a == 0.0) return hit;
  }
  return vec4(0.0, 0.0, 0.0, hitWall ? 0.0 : 1.0);
}

vec2 toUV(vec2 coord) {
  return coord / cascadeInfo.screenSize;
}

vec4 intersectCircleLight(vec2 startPos, vec2 rayDir, float minLength, float maxLength, PointLight light) {
  if (maxLength <= minLength)
    return vec4(vec3(0.0), 1.0);

  vec2 lightPos = (light.pos * 0.5 + 0.5) * cascadeInfo.screenSize;
  float lightRadius = light.radius * cascadeInfo.screenSize.x * 0.5;

  vec2 toCenter = lightPos - startPos;
  float t = dot(toCenter, rayDir);
  t = clamp(t, minLength, max(minLength, maxLength));
  vec2 closestPoint = startPos + rayDir * t;
  float distSquared = dot(closestPoint - lightPos, closestPoint - lightPos);
  float radiusSquared = lightRadius * lightRadius;

  if (distSquared <= radiusSquared) {
    return vec4(light.color, 0.0);
  }
  return vec4(vec3(0.0), 1.0);
}

float castMaze(vec2 start, vec2 dir, float maxLen) {
  float cs = cascadeInfo.screenSize.x / float(cascadeInfo.mazeSize);
  int N = cascadeInfo.mazeSize;

  if (abs(dir.x) < 1e-6) dir.x = 1e-6;
  if (abs(dir.y) < 1e-6) dir.y = 1e-6;

  ivec2 stepI = ivec2(sign(dir));
  vec2 invDir = 1.0 / dir;
  ivec2 cell = ivec2(floor(start / cs));

  vec2 nextB = (vec2(cell) + max(vec2(stepI), 0.0)) * cs;
  vec2 tMax = (nextB - start) * invDir;
  vec2 tDelta = abs(cs * invDir);

  for (int i = 0; i < 128; i++) {
    if (cell.x < 0 || cell.y < 0 || cell.x >= N || cell.y >= N) return maxLen;

    if (tMax.x < tMax.y) {
      if (tMax.x >= maxLen) return maxLen;
      int line = cell.x + (stepI.x > 0 ? 1 : 0);
      if (texelFetch(wallTex, ivec2(line, cell.y), 0).r > 0.5) return tMax.x;
      cell.x += stepI.x;
      tMax.x += tDelta.x;
    } else {
      if (tMax.y >= maxLen) return maxLen;
      int line = cell.y + (stepI.y > 0 ? 1 : 0);
      if (texelFetch(wallTex, ivec2(cell.x, line), 0).g > 0.5) return tMax.y;
      cell.y += stepI.y;
      tMax.y += tDelta.y;
    }
  }
  return maxLen;
}
