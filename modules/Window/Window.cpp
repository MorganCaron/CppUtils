module;

#include <CppUtils/System/OS.hpp>

#if defined(OS_WINDOWS)
#	include <Windows.h>
#elif defined(OS_LINUX)
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/mman.h>

#	include <wayland-client-core.h>
#	include <wayland-client-protocol.h>
#	include <xdg-shell.h>
// #	include <wayland-cursor.h>
#endif

module CppUtils.Window;

import std;

import CppUtils.Container.Size;
import CppUtils.Logger;
import CppUtils.Memory;
import CppUtils.String;
import CppUtils.System;
import CppUtils.Window.Wayland;

namespace CppUtils::Window
{
	using Logger = CppUtils::Logger<"CppUtils">;

#if defined(OS_LINUX)
	namespace
	{
		auto xdg_wm_base_ping([[maybe_unused]] void* data, xdg_wm_base* xdg_wm_base, std::uint32_t serial) -> void
		{
			xdg_wm_base_pong(xdg_wm_base, serial);
		}

		constexpr auto windowManagerBaseListener = xdg_wm_base_listener{
			.ping = xdg_wm_base_ping,
		};

		auto global_registry_handler(
			void* data,
			wl_registry* registry,
			std::uint32_t id,
			const char* interface,
			std::uint32_t version) noexcept -> void
		{
			static const auto waylandRegistryHandlers = std::unordered_map<std::string_view, Wayland::RegisterHandler>{
				Wayland::makeHandlerPair(wl_compositor_interface, Wayland::globals.compositor, 3, 4),
				Wayland::makeHandlerPair(wl_shm_interface, Wayland::globals.sharedMemory, 1, 1),
				Wayland::makeHandlerPair(xdg_wm_base_interface, Wayland::globals.windowManagerBase, 1, 1, [data]() -> void {
				xdg_wm_base_add_listener(Wayland::globals.windowManagerBase.get(), &windowManagerBaseListener, data);
			})};
			auto interfaceName = std::string_view{interface};
			if (const auto& iterator = waylandRegistryHandlers.find(interfaceName); iterator != std::cend(waylandRegistryHandlers))
			{
				iterator->second(registry, id, version);
				Logger::print<"success">("{}", interfaceName);
			}
			else
				Logger::print<"warning">("{}", interfaceName);
		}

		auto global_registry_remover(
			[[maybe_unused]] void* data,
			[[maybe_unused]] wl_registry* registry,
			[[maybe_unused]] std::uint32_t id) noexcept -> void
		{}

		constexpr auto registryListener = wl_registry_listener{
			.global = global_registry_handler,
			.global_remove = global_registry_remover,
		};

		auto wl_buffer_release([[maybe_unused]] void* data, [[maybe_unused]] wl_buffer* buffer) -> void
		{
			// Todo: basculer sur la frame suivante
		}

		constexpr auto bufferListener = wl_buffer_listener{
			.release = wl_buffer_release,
		};

		auto xdg_surface_configure(
			void* data,
			xdg_surface* xdg_surface,
			std::uint32_t serial) -> void;

		constexpr auto surfaceListener = xdg_surface_listener{
			.configure = xdg_surface_configure,
		};
	}
#endif

#if defined(OS_LINUX)
	Window::ImageStorage::ImageStorage(Container::Size2 size):
		m_size{std::move(size)}
	{
		const auto strideSize = m_size.width() * Wayland::bytesPerPixel;
		const auto screenBufferSize = strideSize * m_size.height();
		const auto bufferSize = screenBufferSize * 3; // triple buffering

		auto sharedMemory = System::allocateSharedMemory(bufferSize, "WaylandBuffer");
		if (not sharedMemory)
			throw std::runtime_error{std::string{sharedMemory.error()}};
		m_fileDescriptor = System::FileDescriptor{std::move(sharedMemory).value()};
		pixels = System::MappedSharedMemory{m_fileDescriptor, bufferSize};
		pool.reset(wl_shm_create_pool(Wayland::globals.sharedMemory.get(), m_fileDescriptor.get(), static_cast<std::int32_t>(bufferSize)));
		buffer.reset(
			wl_shm_pool_create_buffer(
				pool.get(),
				static_cast<std::int32_t>(screenBufferSize * 0),
				static_cast<std::int32_t>(m_size.width()),
				static_cast<std::int32_t>(m_size.height()),
				static_cast<std::int32_t>(strideSize),
				WL_SHM_FORMAT_ARGB8888)); // WL_SHM_FORMAT_XRGB8888

		wl_buffer_add_listener(buffer.get(), &bufferListener, nullptr);
	}
#endif

	auto Window::initWindowManager() const -> std::expected<void, std::string>
	{
		using namespace std::literals;
#if defined(OS_LINUX)
		if (Wayland::globals.display)
			return {};
		Wayland::globals.display.reset(wl_display_connect(nullptr));
		if (not Wayland::globals.display)
			return std::unexpected{"Failed to connect to Wayland display"s};
		auto* display = Wayland::globals.display.get();
		Wayland::globals.registry.reset(wl_display_get_registry(display));
		wl_registry_add_listener(Wayland::globals.registry.get(), &registryListener, &Wayland::globals);
		wl_display_roundtrip(display);
		if (not Wayland::globals.compositor)
			return std::unexpected{"Failed to find a Wayland compositor"s};
		if (not Wayland::globals.sharedMemory)
			return std::unexpected{"Failed to find Wayland shared memory interface (wl_shm)"s};
		if (not Wayland::globals.windowManagerBase)
			return std::unexpected{"Failed to find xdg_wm_base interface"s};
#endif
		return {};
	}

	auto Window::openWindow() -> std::expected<void, std::string_view>
	{
		using namespace std::literals;
#if defined(OS_WINDOWS)
		auto hInstance = GetModuleHandleA(nullptr);
		m_window = CreateWindowEx(
			0, windowClassName, m_title.c_str(), generateStyle(m_style), CW_USEDEFAULT, CW_USEDEFAULT, m_size.width(), m_size.height(), nullptr, nullptr, hInstance, nullptr);
		if (m_window == nullptr)
			return std::unexpected{"Window creation failed."sv};
#elif defined(OS_LINUX)
		if (not Wayland::globals.compositor)
			return std::unexpected{"Wayland compositor is not initialized"sv};
		if (not Wayland::globals.windowManagerBase)
			return std::unexpected{"Wayland window manager base is not initialized"sv};
		surface.reset(wl_compositor_create_surface(Wayland::globals.compositor.get()));
		if (not surface)
			return std::unexpected{"Failed to create a Wayland surface"sv};
		shellSurface.reset(xdg_wm_base_get_xdg_surface(Wayland::globals.windowManagerBase.get(), surface.get()));
		if (not shellSurface)
			return std::unexpected{"Failed to create a Wayland shell surface"sv};
		xdg_surface_add_listener(shellSurface.get(), &surfaceListener, this);
		topLevel.reset(xdg_surface_get_toplevel(shellSurface.get()));
		xdg_toplevel_set_title(topLevel.get(), std::data(m_title));
#endif
		return {};
	}

	Window::Window(std::string title, Container::Size2 size, Style style):
		m_title{std::move(title)},
		m_style{style},
		m_size{size}
	{
#if defined(OS_LINUX)
		if (auto result = initWindowManager(); not result)
			throw std::runtime_error{result.error()};

		wl_display_roundtrip(Wayland::globals.display.get());
		if (auto result = openWindow(); not result)
			throw std::runtime_error{std::string{result.error()}};

		m_isOpen = true;
		wl_surface_commit(surface.get());
		wl_display_roundtrip(Wayland::globals.display.get());
		wl_display_flush(Wayland::globals.display.get());
#endif
	}

	Window::~Window()
	{
#if defined(OS_WINDOWS)
		/*
		if (data != MAP_FAILED)
			munmap(data, bufferSize);
		*/
#endif
	}

	auto Window::getPixels() const noexcept -> auto*
	{
#if defined(OS_LINUX)
		return reinterpret_cast<std::uint32_t*>(m_imageStorage.pixels.get());
#else
		return static_cast<std::uint32_t*>(nullptr);
#endif
	}

	auto Window::runLoop() -> void
	{
#if defined(OS_LINUX)
		if (not m_isOpen)
			return;
		auto result = 0;
		while ((result = wl_display_dispatch(Wayland::globals.display.get())) != -1)
			Logger::print<"detail">("Event received, result = {}", result);
		Logger::print<"warning">("Wayland dispatch exited with {}", result);
#endif
	}

	auto Window::update() -> void
	{
#if defined(OS_LINUX)
		if (not m_isOpen or not m_imageStorage.buffer)
			return;
		Logger::print<"detail">("update");
		wl_surface_attach(surface.get(), m_imageStorage.buffer.get(), 0, 0);
		wl_surface_damage(surface.get(), 0, 0, std::numeric_limits<std::int32_t>::max(), std::numeric_limits<std::int32_t>::max());
		wl_surface_commit(surface.get());
		wl_display_flush(Wayland::globals.display.get());
#endif
	}

	auto drawFrame([[maybe_unused]] Window& window) -> void
	{
#if defined(OS_LINUX)
		Logger::print<"detail">("drawFrame");
		const auto& imageSize = window.getSize();
		if (not window.m_imageStorage.buffer)
		{
			Logger::print<"detail">("ImageStorage");
			window.m_imageStorage = Window::ImageStorage{imageSize};
		}
		auto* pixels = window.getPixels();
		for (auto y = 0uz; y < imageSize.height(); ++y)
		{
			for (auto x = 0uz; x < imageSize.width(); ++x)
				if ((x + y / 8 * 8) % 16 < 8)
					pixels[y * imageSize.width() + x] = 0xFF'66'66'66;
				else
					pixels[y * imageSize.width() + x] = 0xFF'EE'EE'EE;
		}
#endif
	}

#if defined(OS_LINUX)
	namespace
	{
		auto xdg_surface_configure(
			void* data,
			xdg_surface* xdg_surface,
			std::uint32_t serial) -> void
		{
			Logger::print<"detail">("Surface configure !");
			auto& window = *static_cast<Window*>(data);
			xdg_surface_ack_configure(xdg_surface, serial);

			drawFrame(window);
			window.update();
		}
	}
#endif
}
