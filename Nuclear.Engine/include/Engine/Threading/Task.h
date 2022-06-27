#pragma once
#include <functional>
#include <vector>
#include <atomic>
#include <future>
#include <iostream>

namespace Nuclear
{
	namespace Threading
	{

		class Task : public std::enable_shared_from_this<Task>
		{
		public:


			Task& operator=(Task const&) = delete;


		private:
			std::function<void()> taskfunc;


		};
	}
}