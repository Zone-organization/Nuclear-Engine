#include "stella/graphics/animator.h"

namespace stella {
namespace graphics {
Animator::Animator() {
  this->Counter = 0;
  this->Paused = false;
  this->Reseted = false;
  this->CurrentFrame = 0;
}

Animator::~Animator() {
  for (auto i = Animations.begin(); i != Animations.end(); ++i)
    delete i->second;
}

void Animator::Add(const std::string name,
                   const std::vector<unsigned int> frames,
                   const unsigned int frameRate) {
  Animation *anim = new Animation(frames, frameRate);
  Animations.insert(std::pair<std::string, Animation *>(name, anim));
}

void Animator::Play(const std::string &name) {
  auto anim_entry = this->Animations.find(name);
  if (anim_entry != Animations.end()) {
    this->CurrentAnimation = anim_entry->second;
    this->CurrentFrameIndex = 0;
  } 
  //else
    //std::cout << "There's no animation named '" << name << "''" << std::endl;
}

unsigned int Animator::Update() {
  if (this->Paused)
    return this->CurrentFrame;
  if (this->Reseted) {
    this->Reseted = false;
    this->CurrentFrameIndex = 0;
    this->CurrentFrame = this->CurrentAnimation->frames[0];
    return this->CurrentFrame;
  }
  if (Animations.size() > 0) {
    if (this->Counter == 10000000)
      this->Counter = 0;
    if (this->Counter % this->CurrentAnimation->frameRate == 0) {
      if (this->CurrentFrameIndex == this->CurrentAnimation->frames.size() - 1)
        this->CurrentFrameIndex = 0;
      else
        ++this->CurrentFrameIndex;
      this->CurrentFrame =
          this->CurrentAnimation->frames[this->CurrentFrameIndex];
    }
    this->Counter++;
    return this->CurrentFrame;
  }
  return 0;
}

void Animator::Pause() { this->Paused = true; }

void Animator::TogglePause() { this->Paused = !this->Paused; }

void Animator::Reset() { this->Reseted = true; }

} // namespace graphics
} // namespace stella
