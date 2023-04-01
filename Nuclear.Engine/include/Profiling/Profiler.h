#pragma once
#include <NE_Common.h>
#include <Graphics/Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/DurationQueryHelper.hpp>
#include <chrono>

namespace Nuclear
{
	namespace Profiling
	{
		class Profiler_CPU
		{
		public:
			FORCE_INLINE Profiler_CPU()
			{
				mStart = std::chrono::high_resolution_clock::now();
			}

			FORCE_INLINE Float64 End()
			{
				auto end = std::chrono::high_resolution_clock::now();

				/* Getting number of milliseconds as a double. */
				std::chrono::duration<double, std::milli> ms_double = end - mStart;

				return ms_double.count();
			}

		protected:
			std::chrono::steady_clock::time_point mStart;
		};


		class Profiler_GPU
		{
		public:
			FORCE_INLINE Profiler_GPU(std::string name)
				: mName(name)
			{
				pQuery.reset(new Diligent::DurationQueryHelper{ Graphics::Context::GetInstance().GetDevice(), 2 });			
			}

			FORCE_INLINE void Begin()
			{
				pQuery->Begin(Graphics::Context::GetInstance().GetContext());
			}

			FORCE_INLINE Float64 End()
			{
				Float64 result= 0.0;
				pQuery->End(Graphics::Context::GetInstance().GetContext(), result);
				return result;
			}

			FORCE_INLINE bool End(Float64 result)
			{
				return pQuery->End(Graphics::Context::GetInstance().GetContext(), result);
			}

		protected:
			std::unique_ptr<Diligent::DurationQueryHelper> pQuery;
			std::string mName;
		};

	}
}

#define PROFILE_GPU(name) static Profiling::Profiler_GPU name(#name); name.Begin()