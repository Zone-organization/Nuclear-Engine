#include "Experimental/Stella/SpriteSheet.h"
#include <Engine\Graphics\API\Texture.h>
#include <string>

namespace NuclearEngine {
	namespace Graphics {
		SpriteSheet::SpriteSheet(API::Texture &texture, unsigned int framex,
			unsigned int framey, unsigned int number_of_frames)
			: Frames(texture), FrameX(framex), FrameY(framey),
			NumOfFrames(number_of_frames)
		{
			Math::Vector3ui Dim = Frames.GetDimensions();

			this->SizeInFramesX = Dim.x / this->FrameX;
			this->SizeInFramesY = Dim.y / this->FrameY;

			if (!number_of_frames) {
				this->NumOfFrames = this->SizeInFramesX * this->SizeInFramesY;
			}
		}

		SpriteSheet::~SpriteSheet() {}

		Math::Vector2 SpriteSheet::GetUV(unsigned int frame) {
			if (this->NumOfFrames <= 1) {
				return glm::vec2(0.0f, 1.0f);
			}
			frame += 1;
			Math::Vector3ui Dim = Frames.GetDimensions();

			float w = static_cast<float>(Dim.x);
			float h = static_cast<float>(Dim.y);

			if (frame > this->NumOfFrames) {
				//std::cout << "SpriteSheet::Warning: frame has a greater value than the "
							 //"number of frames.\nModulus will be applied to select another "
							 //"frame."
						  //<< std::endl;
				frame = frame % this->NumOfFrames;
			}

			int coordX = frame % this->SizeInFramesX - 1;
			if (coordX == -1) {
				coordX = this->SizeInFramesX - 1;
			}
			unsigned int coordY = ceil(frame / static_cast<float>(this->SizeInFramesX)) - 1;

			float uvX = coordX * this->FrameX / w;
			float uvY = 1.0f - coordY * this->FrameY / h;

			return glm::vec2(uvX, uvY);
		}

		Math::Vector2ui SpriteSheet::GetDimensions() const
		{
			Math::Vector3ui Dim = Frames.GetDimensions();

			return Math::Vector2ui(Dim.x,Dim.y);
		}
	}
}