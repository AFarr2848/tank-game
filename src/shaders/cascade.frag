#version 300 es
precision highp float;
#define PI 3.14159265359

struct PointLight {
  vec2 pos;
  float radius;
  vec3 color;
};

struct CascadeInfo {
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  int mazeSize;
  vec2 screenSize;
  int numLights;
  float resolution;
};

uniform sampler2D mazeDistTex;
uniform CascadeInfo cascadeInfo;
layout(std140) uniform LightBlock {
  PointLight lights[64];
};

out vec4 FragColor;

vec4 castRay(vec2 startPos, float angle, float minLength, float maxLength, float mazeDist);
float castMaze(vec2 start, vec2 dir, float maxLen);
vec4 intersectCircleLight(vec2 startPos, vec2 rayDir, float minLength, float maxLength, PointLight light);
vec2 toUV(vec2 coord);

void main() {
  float linearInterval = cascadeInfo.linearInterval;
  float rayInterval = cascadeInfo.rayInterval;

  vec2 probeCenter = (floor(gl_FragCoord.xy / linearInterval) + 0.5) * linearInterval / cascadeInfo.resolution;

  int fragX = int(gl_FragCoord.x);
  int fragY = int(gl_FragCoord.y);

  int intervalInt = int(linearInterval);
  int rayNum = (fragX % intervalInt) + intervalInt * (fragY % intervalInt);

  float rayAngle = (float(rayNum) + 0.5) * rayInterval;

  float minLength = pow(4.0, float(cascadeInfo.cascadeLevel)) * (1.0 / cascadeInfo.resolution);
  float maxLength = pow(4.0, float(cascadeInfo.cascadeLevel + 1)) * (1.0 / cascadeInfo.resolution);

  float mazeDist = texelFetch(mazeDistTex, ivec2(fragX, fragY), 0)[cascadeInfo.cascadeLevel];

  //FragColor = vec4(vec3(mazeDist / maxLength), 1.0);
  FragColor = vec4(castRay(probeCenter, rayAngle, minLength, maxLength, mazeDist).rgb, 1.0);
  return;
}

vec4 castRay(vec2 startPos, float angle, float minLength, float maxLength, float mazeDist) {
  vec2 rayDir = vec2(cos(angle), sin(angle));
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
