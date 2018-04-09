#pragma once

#include <map>
#include <vector>
#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics\API\Texture.h>

namespace NuclearEngine {
	namespace Graphics {
		class NEAPI Animator2D {
		public:
			Animator2D();
			~Animator2D();

			void Add(const std::string name, const std::vector<unsigned int> frames, const unsigned int frameRate);
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
	}
}
