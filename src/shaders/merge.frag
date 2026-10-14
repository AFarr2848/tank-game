#version 300 es
precision highp float;
#define PI 3.14159265359

struct CascadeInfo {
  int mazeSize;
  vec2 screenSize;
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  vec3 backgroundColor;
  int numLights;
};

struct RayEndpointInfo {
  vec2 endpoint;
  int rayNum;
};

uniform sampler2D highCascade;
uniform sampler2D lowCascade;
uniform CascadeInfo cascadeInfo;

out vec4 FragColor;

RayEndpointInfo getLowerEndpoint();
vec2 getHigherCascadeUV(vec2 endpoint, int rayNum);

vec4 fetchHigher(ivec2 q, int rayNum, int Hi) {
  ivec2 probes = ivec2(cascadeInfo.screenSize) / Hi;
  q = clamp(q, ivec2(0), probes - 1);
  vec4 sum = vec4(0.0);
  for (int k = 0; k < 4; k++) {
    int r = rayNum * 4 + k;
    sum += texelFetch(highCascade, q * Hi + ivec2(r % Hi, r / Hi), 0);
  }
  return sum * 0.25;
}

void main() {
  float L = 2.0 * exp2(float(cascadeInfo.cascadeLevel));
  float rayInterval = 2.0 * PI / (L * L);
  int Li = int(L);

  ivec2 frag = ivec2(gl_FragCoord.xy);
  vec2 probeCenter = (floor(gl_FragCoord.xy / L) + 0.5) * L;
  int rayNum = (frag.x % Li) + Li * (frag.y % Li);
  float angle = (float(rayNum) + 0.5) * rayInterval;
  float maxLength = pow(6.0, float(cascadeInfo.cascadeLevel + 1));
  vec2 endpoint = probeCenter + vec2(cos(angle), sin(angle)) * maxLength;

  float H = L * 2.0;
  int Hi = Li * 2;
  vec2 f = probeCenter / float(Hi) - 0.5;
  ivec2 base = ivec2(floor(f));
  vec2 w = fract(f);

  vec4 high =
    fetchHigher(base, rayNum, Hi) * (1.0 - w.x) * (1.0 - w.y) +
      fetchHigher(base + ivec2(1, 0), rayNum, Hi) * w.x * (1.0 - w.y) +
      fetchHigher(base + ivec2(0, 1), rayNum, Hi) * (1.0 - w.x) * w.y +
      fetchHigher(base + ivec2(1, 1), rayNum, Hi) * w.x * w.y;

  vec4 low = texelFetch(lowCascade, frag, 0);
  FragColor = vec4(low.rgb + low.a * high.rgb, low.a * high.a);
}
