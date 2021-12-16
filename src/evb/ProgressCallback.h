#ifndef PROGRESSCALLBACK_H
#define PROGRESSCALLBACK_H

#include <functional>

#define BIND_PROGRESS_CALLBACK_FUNCTION(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2)

namespace EventBuilder {

	using ProgressCallbackFunc = std::function<void(long, long)>;

}

#endif