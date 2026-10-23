#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTex; // scene (maze + sprites)
uniform sampler2D lightingTex; // merged level-0 rays
uniform vec2 screenSize;

vec3 probeLight(ivec2 probe, ivec2 maxProbe) {
  probe = clamp(probe, ivec2(0), maxProbe);
  ivec2 o = probe * 2; // top-left of the probe's 2x2 block
  vec3 s = texelFetch(lightingTex, o, 0).rgb
      + texelFetch(lightingTex, o + ivec2(1, 0), 0).rgb
      + texelFetch(lightingTex, o + ivec2(0, 1), 0).rgb
      + texelFetch(lightingTex, o + ivec2(1, 1), 0).rgb;
  return s * 0.25; // average the 4 directions
}

void main() {
  ivec2 size = textureSize(lightingTex, 0);
  ivec2 maxProbe = size / 2 - 1;

  vec2 f = gl_FragCoord.xy / 2.0 - 0.5; // pixel position in probe coordinates
  ivec2 b = ivec2(floor(f));
  vec2 w = fract(f);
  vec3 light = mix(
      mix(probeLight(b, maxProbe), probeLight(b + ivec2(1, 0), maxProbe), w.x),
      mix(probeLight(b + ivec2(0, 1), maxProbe), probeLight(b + ivec2(1, 1), maxProbe), w.x),
      w.y);

  vec3 scene = texelFetch(screenTex, ivec2(gl_FragCoord.xy), 0).rgb;
  FragColor = vec4(scene * (vec3(0.15) + light), 1.0); // ambient + light; tune to taste
}
