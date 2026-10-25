#version 300 es
precision highp float;
#define PI 3.14159265359
struct MazeBakeInfo {
  int mazeSize;
  vec2 screenSize;
  int numCascades;
  float resolution;
  float initialLinearInterval;
};

uniform sampler2D mazeTex;
uniform MazeBakeInfo mazeInfo;

out vec4 FragColor;

float castMaze(vec2 start, vec2 dir, float maxLen);

void main() {
  float outColor[4];
  for (int c = 0; c < mazeInfo.numCascades; c++) {
    float L = mazeInfo.initialLinearInterval * exp2(float(c));
    vec2 probeCenter = (floor(gl_FragCoord.xy / L) + 0.5) * L / mazeInfo.resolution;
    int Li = int(L);
    int rayNum = (int(gl_FragCoord.x) % Li) + Li * (int(gl_FragCoord.y) % Li);
    float angle = (float(rayNum) + 0.5) * (2.0 * PI / (L * L));
    float maxLen = pow(4.0, float(c + 1)) * (1.0 / mazeInfo.resolution);
    outColor[c] = castMaze(probeCenter, vec2(cos(angle), sin(angle)), maxLen);
  }
  FragColor = vec4(outColor[0], outColor[1], outColor[2], outColor[3]);
}

float castMaze(vec2 start, vec2 dir, float maxLen) {
  float cs = mazeInfo.screenSize.x / float(mazeInfo.mazeSize);
  int N = mazeInfo.mazeSize;

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
      if (texelFetch(mazeTex, ivec2(line, cell.y), 0).r > 0.5) return tMax.x;
      cell.x += stepI.x;
      tMax.x += tDelta.x;
    } else {
      if (tMax.y >= maxLen) return maxLen;
      int line = cell.y + (stepI.y > 0 ? 1 : 0);
      if (texelFetch(mazeTex, ivec2(cell.x, line), 0).g > 0.5) return tMax.y;
      cell.y += stepI.y;
      tMax.y += tDelta.y;
    }
  }
  return maxLen;
}
