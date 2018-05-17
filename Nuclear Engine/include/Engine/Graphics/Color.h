#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
namespace NuclearEngine
{
	namespace Graphics
	{

		struct NEAPI Color
		{
			float r, g, b, a;

			Color();
			Color(float value);
			Color(int Red, int Green, int Blue, int Alpha);
			Color(float r, float g, float b, float a = 1.0f);
			~Color();

			Math::Vector4 Get4f();

			friend NEAPI Color operator+(const Color& left, const Color& right);
			friend NEAPI Color operator-(const Color& left, const Color& right);
			friend NEAPI Color operator*(const Color& left, const Color& right);
			friend NEAPI Color operator/(const Color& left, const Color& right);

			Color& operator=(const Color& right);
			Color& operator+=(const Color& right);
			Color& operator-=(const Color& right);
			Color& operator*=(const Color& right);
			Color& operator/=(const Color& right);

			bool operator==(const Color& right);
			bool operator!=(const Color& right);

			friend NEAPI std::ostream& operator<<(std::ostream& os, const Color& color);
		};

		namespace Colors
		{
			inline static Color IndianRed() { return Color(0.803f, 0.360f, 0.360f, 1.000f); }
			inline static Color LightCoral() { return Color(0.941f, 0.501f, 0.501f, 1.000f); }
			inline static Color Salmon() { return Color(0.980f, 0.501f, 0.447f, 1.000f); }
			inline static Color DarkSalmon() { return Color(0.913f, 0.588f, 0.478f, 1.000f); }
			inline static Color LightSalmon() { return Color(1.000f, 0.627f, 0.478f, 1.000f); }
			inline static Color Crimson() { return Color(0.862f, 0.078f, 0.235f, 1.000f); }
			inline static Color Red() { return Color(1.000f, 0.000f, 0.000f, 1.000f); }
			inline static Color FireBrick() { return Color(0.698f, 0.133f, 0.133f, 1.000f); }
			inline static Color DarkRed() { return Color(0.545f, 0.000f, 0.000f, 1.000f); }
			inline static Color Pink() { return Color(1.000f, 0.752f, 0.796f, 1.000f); }
			inline static Color LightPink() { return Color(1.000f, 0.713f, 0.756f, 1.000f); }
			inline static Color HotPink() { return Color(1.000f, 0.411f, 0.705f, 1.000f); }
			inline static Color DeepPink() { return Color(1.000f, 0.078f, 0.576f, 1.000f); }
			inline static Color MediumVioletRed() { return Color(0.780f, 0.082f, 0.521f, 1.000f); }
			inline static Color PaleVioletRed() { return Color(0.858f, 0.439f, 0.576f, 1.000f); }
			inline static Color Coral() { return Color(1.000f, 0.498f, 0.313f, 1.000f); }
			inline static Color Tomato() { return Color(1.000f, 0.388f, 0.278f, 1.000f); }
			inline static Color OrangeRed() { return Color(1.000f, 0.270f, 0.000f, 1.000f); }
			inline static Color DarkOrange() { return Color(1.000f, 0.549f, 0.000f, 1.000f); }
			inline static Color Orange() { return Color(1.000f, 0.647f, 0.000f, 1.000f); }
			inline static Color Gold() { return Color(1.000f, 0.843f, 0.000f, 1.000f); }
			inline static Color Yellow() { return Color(1.000f, 1.000f, 0.000f, 1.000f); }
			inline static Color LightYellow() { return Color(1.000f, 1.000f, 0.878f, 1.000f); }
			inline static Color LemonChiffon() { return Color(1.000f, 0.980f, 0.803f, 1.000f); }
			inline static Color LightGoldenrodYellow() { return Color(0.980f, 0.980f, 0.823f, 1.000f); }
			inline static Color PapayaWhip() { return Color(1.000f, 0.937f, 0.835f, 1.000f); }
			inline static Color Moccasin() { return Color(1.000f, 0.894f, 0.709f, 1.000f); }
			inline static Color PeachPuff() { return Color(1.000f, 0.854f, 0.725f, 1.000f); }
			inline static Color PaleGoldenrod() { return Color(0.933f, 0.909f, 0.666f, 1.000f); }
			inline static Color Khaki() { return Color(0.941f, 0.901f, 0.549f, 1.000f); }
			inline static Color DarkKhaki() { return Color(0.741f, 0.717f, 0.419f, 1.000f); }
			inline static Color Lavender() { return Color(0.901f, 0.901f, 0.980f, 1.000f); }
			inline static Color Thistle() { return Color(0.847f, 0.749f, 0.847f, 1.000f); }
			inline static Color Plum() { return Color(0.866f, 0.627f, 0.866f, 1.000f); }
			inline static Color Violet() { return Color(0.933f, 0.509f, 0.933f, 1.000f); }
			inline static Color Orchid() { return Color(0.854f, 0.439f, 0.839f, 1.000f); }
			inline static Color Fuchsia() { return Color(1.000f, 0.000f, 1.000f, 1.000f); }
			inline static Color Magenta() { return Color(1.000f, 0.000f, 1.000f, 1.000f); }
			inline static Color MediumOrchid() { return Color(0.729f, 0.333f, 0.827f, 1.000f); }
			inline static Color MediumPurple() { return Color(0.576f, 0.439f, 0.858f, 1.000f); }
			inline static Color BlueViolet() { return Color(0.541f, 0.168f, 0.886f, 1.000f); }
			inline static Color DarkViolet() { return Color(0.580f, 0.000f, 0.827f, 1.000f); }
			inline static Color DarkOrchid() { return Color(0.600f, 0.196f, 0.800f, 1.000f); }
			inline static Color DarkMagenta() { return Color(0.545f, 0.000f, 0.545f, 1.000f); }
			inline static Color Purple() { return Color(0.501f, 0.000f, 0.501f, 1.000f); }
			inline static Color RebeccaPurple() { return Color(0.400f, 0.200f, 0.600f, 1.000f); }
			inline static Color Indigo() { return Color(0.294f, 0.000f, 0.509f, 1.000f); }
			inline static Color MediumSlateBlue() { return Color(0.482f, 0.407f, 0.933f, 1.000f); }
			inline static Color SlateBlue() { return Color(0.415f, 0.352f, 0.803f, 1.000f); }
			inline static Color GreenYellow() { return Color(0.678f, 1.000f, 0.184f, 1.000f); }
			inline static Color Chartreuse() { return Color(0.498f, 1.000f, 0.000f, 1.000f); }
			inline static Color LawnGreen() { return Color(0.486f, 0.988f, 0.000f, 1.000f); }
			inline static Color Lime() { return Color(0.000f, 1.000f, 0.000f, 1.000f); }
			inline static Color LimeGreen() { return Color(0.196f, 0.803f, 0.196f, 1.000f); }
			inline static Color PaleGreen() { return Color(0.596f, 0.984f, 0.596f, 1.000f); }
			inline static Color LightGreen() { return Color(0.564f, 0.933f, 0.564f, 1.000f); }
			inline static Color MediumSpringGreen() { return Color(0.000f, 0.980f, 0.603f, 1.000f); }
			inline static Color SpringGreen() { return Color(0.000f, 1.000f, 0.498f, 1.000f); }
			inline static Color MediumSeaGreen() { return Color(0.235f, 0.701f, 0.443f, 1.000f); }
			inline static Color SeaGreen() { return Color(0.180f, 0.545f, 0.341f, 1.000f); }
			inline static Color ForestGreen() { return Color(0.133f, 0.545f, 0.133f, 1.000f); }
			inline static Color Green() { return Color(0.000f, 0.501f, 0.000f, 1.000f); }
			inline static Color DarkGreen() { return Color(0.000f, 0.392f, 0.000f, 1.000f); }
			inline static Color YellowGreen() { return Color(0.603f, 0.803f, 0.196f, 1.000f); }
			inline static Color OliveDrab() { return Color(0.419f, 0.556f, 0.137f, 1.000f); }
			inline static Color Olive() { return Color(0.501f, 0.501f, 0.000f, 1.000f); }
			inline static Color DarkOliveGreen() { return Color(0.333f, 0.419f, 0.184f, 1.000f); }
			inline static Color MediumAquamarine() { return Color(0.400f, 0.803f, 0.666f, 1.000f); }
			inline static Color DarkSeaGreen() { return Color(0.560f, 0.737f, 0.560f, 1.000f); }
			inline static Color LightSeaGreen() { return Color(0.125f, 0.698f, 0.666f, 1.000f); }
			inline static Color DarkCyan() { return Color(0.000f, 0.545f, 0.545f, 1.000f); }
			inline static Color Teal() { return Color(0.000f, 0.501f, 0.501f, 1.000f); }
			inline static Color Aqua() { return Color(0.000f, 1.000f, 1.000f, 1.000f); }
			inline static Color Cyan() { return Color(0.000f, 1.000f, 1.000f, 1.000f); }
			inline static Color LightCyan() { return Color(0.878f, 1.000f, 1.000f, 1.000f); }
			inline static Color PaleTurquoise() { return Color(0.686f, 0.933f, 0.933f, 1.000f); }
			inline static Color Aquamarine() { return Color(0.498f, 1.000f, 0.831f, 1.000f); }
			inline static Color Turquoise() { return Color(0.250f, 0.878f, 0.815f, 1.000f); }
			inline static Color MediumTurquoise() { return Color(0.282f, 0.819f, 0.800f, 1.000f); }
			inline static Color DarkTurquoise() { return Color(0.000f, 0.807f, 0.819f, 1.000f); }
			inline static Color CadetBlue() { return Color(0.372f, 0.619f, 0.627f, 1.000f); }
			inline static Color SteelBlue() { return Color(0.274f, 0.509f, 0.705f, 1.000f); }
			inline static Color LightSteelBlue() { return Color(0.690f, 0.768f, 0.870f, 1.000f); }
			inline static Color PowderBlue() { return Color(0.690f, 0.878f, 0.901f, 1.000f); }
			inline static Color LightBlue() { return Color(0.678f, 0.847f, 0.901f, 1.000f); }
			inline static Color SkyBlue() { return Color(0.529f, 0.807f, 0.921f, 1.000f); }
			inline static Color LightSkyBlue() { return Color(0.529f, 0.807f, 0.980f, 1.000f); }
			inline static Color DeepSkyBlue() { return Color(0.000f, 0.749f, 1.000f, 1.000f); }
			inline static Color DodgerBlue() { return Color(0.117f, 0.564f, 1.000f, 1.000f); }
			inline static Color CornflowerBlue() { return Color(0.392f, 0.584f, 0.929f, 1.000f); }
			inline static Color RoyalBlue() { return Color(0.254f, 0.411f, 0.882f, 1.000f); }
			inline static Color Blue() { return Color(0.000f, 0.000f, 1.000f, 1.000f); }
			inline static Color MediumBlue() { return Color(0.000f, 0.000f, 0.803f, 1.000f); }
			inline static Color DarkBlue() { return Color(0.000f, 0.000f, 0.545f, 1.000f); }
			inline static Color Navy() { return Color(0.000f, 0.000f, 0.501f, 1.000f); }
			inline static Color MidnightBlue() { return Color(0.098f, 0.098f, 0.439f, 1.000f); }
			inline static Color Cornsilk() { return Color(1.000f, 0.972f, 0.862f, 1.000f); }
			inline static Color BlanchedAlmond() { return Color(1.000f, 0.921f, 0.803f, 1.000f); }
			inline static Color Bisque() { return Color(1.000f, 0.894f, 0.768f, 1.000f); }
			inline static Color NavajoWhite() { return Color(1.000f, 0.870f, 0.678f, 1.000f); }
			inline static Color Wheat() { return Color(0.960f, 0.870f, 0.701f, 1.000f); }
			inline static Color BurlyWood() { return Color(0.870f, 0.721f, 0.529f, 1.000f); }
			inline static Color Tan() { return Color(0.823f, 0.705f, 0.549f, 1.000f); }
			inline static Color RosyBrown() { return Color(0.737f, 0.560f, 0.560f, 1.000f); }
			inline static Color SandyBrown() { return Color(0.956f, 0.643f, 0.376f, 1.000f); }
			inline static Color Goldenrod() { return Color(0.854f, 0.647f, 0.125f, 1.000f); }
			inline static Color DarkGoldenrod() { return Color(0.721f, 0.525f, 0.043f, 1.000f); }
			inline static Color Peru() { return Color(0.803f, 0.521f, 0.247f, 1.000f); }
			inline static Color Chocolate() { return Color(0.823f, 0.411f, 0.117f, 1.000f); }
			inline static Color SaddleBrown() { return Color(0.545f, 0.270f, 0.074f, 1.000f); }
			inline static Color Sienna() { return Color(0.627f, 0.321f, 0.176f, 1.000f); }
			inline static Color Brown() { return Color(0.647f, 0.164f, 0.164f, 1.000f); }
			inline static Color Maroon() { return Color(0.501f, 0.000f, 0.000f, 1.000f); }
			inline static Color White() { return Color(1.000f, 1.000f, 1.000f, 1.000f); }
			inline static Color Snow() { return Color(1.000f, 0.980f, 0.980f, 1.000f); }
			inline static Color Honeydew() { return Color(0.941f, 1.000f, 0.941f, 1.000f); }
			inline static Color MintCream() { return Color(0.960f, 1.000f, 0.980f, 1.000f); }
			inline static Color Azure() { return Color(0.941f, 1.000f, 1.000f, 1.000f); }
			inline static Color AliceBlue() { return Color(0.941f, 0.972f, 1.000f, 1.000f); }
			inline static Color GhostWhite() { return Color(0.972f, 0.972f, 1.000f, 1.000f); }
			inline static Color WhiteSmoke() { return Color(0.960f, 0.960f, 0.960f, 1.000f); }
			inline static Color Seashell() { return Color(1.000f, 0.960f, 0.933f, 1.000f); }
			inline static Color Beige() { return Color(0.960f, 0.960f, 0.862f, 1.000f); }
			inline static Color OldLace() { return Color(0.992f, 0.960f, 0.901f, 1.000f); }
			inline static Color FloralWhite() { return Color(1.000f, 0.980f, 0.941f, 1.000f); }
			inline static Color Ivory() { return Color(1.000f, 1.000f, 0.941f, 1.000f); }
			inline static Color AntiqueWhite() { return Color(0.980f, 0.921f, 0.843f, 1.000f); }
			inline static Color Linen() { return Color(0.980f, 0.941f, 0.901f, 1.000f); }
			inline static Color LavenderBlush() { return Color(1.000f, 0.941f, 0.960f, 1.000f); }
			inline static Color MistyRose() { return Color(1.000f, 0.894f, 0.882f, 1.000f); }
			inline static Color Gainsboro() { return Color(0.862f, 0.862f, 0.862f, 1.000f); }
			inline static Color LightGray() { return Color(0.827f, 0.827f, 0.827f, 1.000f); }
			inline static Color LightGrey() { return Color(0.827f, 0.827f, 0.827f, 1.000f); }
			inline static Color Silver() { return Color(0.752f, 0.752f, 0.752f, 1.000f); }
			inline static Color DarkGray() { return Color(0.662f, 0.662f, 0.662f, 1.000f); }
			inline static Color DarkGrey() { return Color(0.662f, 0.662f, 0.662f, 1.000f); }
			inline static Color Gray() { return Color(0.501f, 0.501f, 0.501f, 1.000f); }
			inline static Color Grey() { return Color(0.501f, 0.501f, 0.501f, 1.000f); }
			inline static Color DimGray() { return Color(0.411f, 0.411f, 0.411f, 1.000f); }
			inline static Color DimGrey() { return Color(0.411f, 0.411f, 0.411f, 1.000f); }
			inline static Color LightSlateGray() { return Color(0.466f, 0.533f, 0.600f, 1.000f); }
			inline static Color LightSlateGrey() { return Color(0.466f, 0.533f, 0.600f, 1.000f); }
			inline static Color SlateGray() { return Color(0.439f, 0.501f, 0.564f, 1.000f); }
			inline static Color SlateGrey() { return Color(0.439f, 0.501f, 0.564f, 1.000f); }
			inline static Color DarkSlateGray() { return Color(0.184f, 0.309f, 0.309f, 1.000f); }
			inline static Color DarkSlateGrey() { return Color(0.184f, 0.309f, 0.309f, 1.000f); }
			inline static Color Black() { return Color(0.000f, 0.000f, 0.000f, 1.000f); }
		}
	}
}