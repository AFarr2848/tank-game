#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTex;
uniform sampler2D lightingTex;
uniform vec2 screenSize;

void main() {
  vec4 screenColor = texture(screenTex, TexCoords);
  vec4 lightingColor = vec4(texture(lightingTex, TexCoords + (vec2(0.5) / screenSize.xy)).rgb, 1.0);
  FragColor = screenColor * lightingColor;
  //FragColor = lightingColor;
}
