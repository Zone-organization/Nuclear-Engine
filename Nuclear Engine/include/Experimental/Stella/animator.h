#pragma once

#include <map>
#include <string>
#include <vector>

namespace stella {
namespace graphics {
class Animator {
public:
  Animator();
  ~Animator();

  void Add(const std::string name, const std::vector<unsigned int> frames,
           const unsigned int frameRate);
  void Play(const std::string &name);
  unsigned int Update();
  void Pause();
  void TogglePause();
  void Reset();

private:
  struct Animation {
    const std::vector<unsigned int> frames;
    const unsigned int frameRate;

    Animation(const std::vector<unsigned int> frs, const unsigned int fr)
        : frames(frs), frameRate(fr) {}
  };
  std::map<std::string, Animation *> Animations;
  Animation *CurrentAnimation;
  unsigned int CurrentFrame, CurrentFrameIndex, Counter;
  bool Paused, Reseted;
};
} // namespace graphics
} // namespace stella
