#pragma once

#include "Callback/Callback.h"
#include "Settings/Settings.h"
#include "Text/Font.h"
#include "CellBuffer/CellBuffer.h"
#include "Texture/Texture.h"



#include <unordered_map>
#include <filesystem>

struct GLFWwindow;

struct jcTerminal
{
public:
	struct LoadedFont
	{
		int index = 0;
		std::filesystem::path path;
		jct::text::Font font;
	};

	auto& window() noexcept { return this->window_; };
	const auto& window() const noexcept { return this->window_; };

	auto& settings() noexcept { return this->settings_; };
	const auto& settings() const noexcept { return this->settings_; };

	auto& callbacks() noexcept { return this->callbacks_; };
	const auto& callbacks() const noexcept { return this->callbacks_; };

	auto& fonts() noexcept { return this->fonts_; };
	const auto& fonts() const noexcept { return this->fonts_; };

	auto& cell_buffer() noexcept { return this->cellbuffer_; };
	const auto& cell_buffer() const noexcept { return this->cellbuffer_; };

	

	auto& texture_sheet() noexcept { return this->texture_sheet_; };
	const auto& texture_sheet() const noexcept { return this->texture_sheet_; };

	auto& mask_texture_sheet() noexcept { return this->mask_texture_sheet_; };
	const auto& mask_texture_sheet() const noexcept { return this->mask_texture_sheet_; };

	bool texture_needs_update_v = false;


	jcTerminal() = default;
	~jcTerminal();

private:
	GLFWwindow* window_ = nullptr;
	
	std::unordered_map<int, LoadedFont> fonts_{};
	
	jct::TerminalSettings settings_{};
	jct::CallbackLedger callbacks_{};

	jct::CellBuffer cellbuffer_{};
	
	jct::Texture texture_sheet_{};
	jct::MaskTexture mask_texture_sheet_{};



};

namespace jct
{
	using Terminal = jcTerminal;
};

