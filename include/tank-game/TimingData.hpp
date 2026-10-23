
#include <iostream>
namespace Tnk {
struct TimingData {
  int frames;
  float deltaTime;
  float lastTime;

  float averageFrameTime;
  int framesSinceAverage;
  float frameTimeSum;

  void incrementTiming(float currentTime) {
    frames++;
    framesSinceAverage++;
    deltaTime = (currentTime - lastTime) / 1000;
    frameTimeSum += deltaTime;
    if (frames % 120 == 0) {
      frameTimeSum /= 120;
      std::cout << "Average frame time: " << frameTimeSum << std::endl;
    }

    lastTime = currentTime;
  }
};

};  // namespace Tnk
