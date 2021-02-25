#pragma once

#include "Callback/Callback.h"
#include "Settings/Settings.h"

struct GLFWwindow;

struct jcTerminal
{
public:
	auto& window() noexcept { return this->window_; };
	const auto& window() const noexcept { return this->window_; };

	auto& settings() noexcept { return this->settings_; };
	const auto& settings() const noexcept { return this->settings_; };

	auto& callbacks() noexcept { return this->callbacks_; };
	const auto& callbacks() const noexcept { return this->callbacks_; };

	jcTerminal() = default;
	~jcTerminal();

private:
	GLFWwindow* window_ = nullptr;
	jct::TerminalSettings settings_{};
	jct::CallbackLedger callbacks_{};

};

namespace jct
{
	using Terminal = jcTerminal;
};

