#pragma once
#include <memory> 

template <class T>
using SPtr<T> = std::shared_ptr<T>;
