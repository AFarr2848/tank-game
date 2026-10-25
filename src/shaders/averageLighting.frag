#version 300 es
precision highp float;
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTex;
uniform sampler2D lightingTex;
uniform float resolution;

const int baseInterval = 2;

vec3 probeLight(ivec2 probe, ivec2 maxProbe) {
  probe = clamp(probe, ivec2(0), maxProbe);
  ivec2 o = probe * baseInterval;
  vec3 sum = vec3(0.0);
  for (int y = 0; y < baseInterval; y++) {
    for (int x = 0; x < baseInterval; x++) {
      sum += texelFetch(lightingTex, o + ivec2(x, y), 0).rgb;
    }
  }
  return sum * 0.25;
}

void main() {
  ivec2 size = textureSize(lightingTex, 0);
  ivec2 maxProbe = size / baseInterval - 1;

  //fixme
  vec2 f = gl_FragCoord.xy * resolution / float(baseInterval) - 0.5;
  ivec2 b = ivec2(floor(f));
  vec2 w = fract(f);
  vec3 light = mix(
      mix(probeLight(b, maxProbe), probeLight(b + ivec2(1, 0), maxProbe), w.x),
      mix(probeLight(b + ivec2(0, 1), maxProbe), probeLight(b + ivec2(1, 1), maxProbe), w.x),
      w.y);

  vec3 scene = texelFetch(screenTex, ivec2(gl_FragCoord.xy), 0).rgb;
  FragColor = vec4(scene * (vec3(0.15) + light), 1.0);
}
