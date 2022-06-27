#pragma once
#include <string>
#include <functional>

enum class CallbackType
{
	capture
};

class EventCallback
{
public:
	using CALLBACK_T = void(const std::string&);

	void Register(CALLBACK_T f) {
		capture_callback = f;
	}

	void UnRegister() {
		capture_callback = nullptr;
	}

	void Execute(const std::string& content) {
		if (capture_callback)
			capture_callback(content);
	}

private:
	std::function<CALLBACK_T> capture_callback;
};