#pragma once

#include <stdexcept>
#include <type_traits>
#include <initializer_list>

// https://en.cppreference.com/w/cpp/header/expected
namespace CppUtils::Stl
{
	template<class E>
	class unexpected
	{
	public:
		constexpr unexpected(const unexpected&) = default;
		constexpr unexpected(unexpected&&) = default;

		template<class... Args>
		constexpr explicit unexpected(std::in_place_t, Args&&...);

		template<class U, class... Args>
		constexpr explicit unexpected(std::in_place_t, std::initializer_list<U>, Args&&...);
		
		template<class Err = E>
		constexpr explicit unexpected(Err&&);

		constexpr unexpected& operator=(const unexpected&) = default;
		constexpr unexpected& operator=(unexpected&&) = default;

		constexpr const E& error() const& noexcept;
		constexpr E& error() & noexcept;
		constexpr const E&& error() const&& noexcept;
		constexpr E&& error() && noexcept;

		constexpr void swap(unexpected& other) noexcept(/* see description */);

		friend constexpr void swap(unexpected& x, unexpected& y) noexcept(noexcept(x.swap(y)));

		template<class E2>
		friend constexpr bool operator==(const unexpected&, const unexpected<E2>&);

	private:
		E unex;
	};

	template<class E> 
	unexpected(E) -> unexpected<E>;

	template<class E>
	class bad_expected_access:
		public bad_expected_access<void>
	{
	public:
		explicit bad_expected_access(E);

		const char* what() const noexcept override;

		E& error() & noexcept;
		const E& error() const& noexcept;
		E&& error() && noexcept;
		const E&&  error() const&& noexcept;

	private:
		E unex;
	};

	template<>
	class bad_expected_access<void>:
		public std::exception
	{
	protected:
		bad_expected_access() noexcept;
		bad_expected_access(const bad_expected_access&);
		bad_expected_access(bad_expected_access&&);
		bad_expected_access& operator=(const bad_expected_access&);
		bad_expected_access& operator=(bad_expected_access&&);

		~bad_expected_access();

	public:
		const char* what() const noexcept override;
	};

	// in-place construction of unexpected values
	struct unexpect_t
	{
		explicit unexpect_t() = default;
	};
	inline constexpr unexpect_t unexpect{};

	template<class T, class E>
	class expected
	{
	public:
		using value_type = T;
		using error_type = E;
		using unexpected_type = unexpected<E>;

		template<class U>
		using rebind = expected<U, error_type>;

		constexpr expected();
		constexpr explicit(/* see description */) 
		expected(const expected&);
		constexpr explicit(/* see description */) 
		expected(expected&&) noexcept(/* see description */);
		template<class U, class G>
		constexpr explicit(/* see description */) expected(const expected<U, G>&);
		template<class U, class G>
		constexpr explicit(/* see description */) expected(expected<U, G>&&);

		template<class U = T>
		constexpr explicit(/* see description */) expected(U&& v);

		template<class G>
		constexpr expected(const unexpected<G>&);
		template<class G>
		constexpr expected(unexpected<G>&&);

		template<class... Args>
		constexpr explicit expected(std::in_place_t, Args&&...);
		template<class U, class... Args>
		constexpr explicit expected(std::in_place_t, std::initializer_list<U>, Args&&...);
		template<class... Args>
		constexpr explicit expected(unexpect_t, Args&&...);
		template<class U, class... Args>
		constexpr explicit expected(unexpect_t, std::initializer_list<U>, Args&&...);

		constexpr ~expected();

		constexpr expected& operator=(const expected&);
		constexpr expected& operator=(expected&&) noexcept(/* see description */);
		template<class U = T> constexpr expected& operator=(U&&);
		template<class G>
		constexpr expected& operator=(const unexpected<G>&);
		template<class G>
		constexpr expected& operator=(unexpected<G>&&);

		template<class... Args>
		constexpr T& emplace(Args&&...) noexcept;
		template<class U, class... Args>
		constexpr T& emplace(std::initializer_list<U>, Args&&...) noexcept;

		constexpr void swap(expected&) noexcept(/* see description */);
		friend constexpr void swap(expected&, expected&) noexcept(/* see description */);

		constexpr const T* operator->() const noexcept;
		constexpr T* operator->() noexcept;
		constexpr const T& operator*() const& noexcept;
		constexpr T& operator*() & noexcept;
		constexpr const T&& operator*() const&& noexcept;
		constexpr T&& operator*() && noexcept;
		constexpr explicit operator bool() const noexcept;
		constexpr bool has_value() const noexcept;
		constexpr const T& value() const&;
		constexpr T& value() &;
		constexpr const T&& value() const&&;
		constexpr T&& value() &&;
		constexpr const E& error() const&;
		constexpr E& error() &;
		constexpr const E&& error() const&&;
		constexpr E&& error() &&;
		template<class U>
		constexpr T value_or(U&&) const&;
		template<class U>
		constexpr T value_or(U&&) &&;

		template<class T2, class E2>
		friend constexpr bool operator==(const expected& x, const expected<T2, E2>& y);
		template<class T2>
		friend constexpr bool operator==(const expected&, const T2&);
		template<class E2>
		friend constexpr bool operator==(const expected&, const unexpected<E2>&);

	private:
		bool has_val;
		union {
			T val;
			E unex;
		};
	};

	template<class T, class E>
	requires std::is_void_v<T>
	class expected<T, E>
	{
	public:
		using value_type = T;
		using error_type = E;
		using unexpected_type = unexpected<E>;

		template<class U>
		using rebind = expected<U, error_type>;

		constexpr expected() noexcept;
		constexpr explicit(/* see description */)
		expected(const expected&);
		constexpr explicit(/* see description */)
		expected(expected&&) noexcept(/* see description */);
		template<class U, class G>
		constexpr explicit(/* see description */) expected(const expected<U, G>&);
		template<class U, class G>
		constexpr explicit(/* see description */) expected(expected<U, G>&&);

		template<class G>
		constexpr expected(const unexpected<G>&);
		template<class G>
		constexpr expected(unexpected<G>&&);

		constexpr explicit expected(std::in_place_t) noexcept;
		template<class... Args>
		constexpr explicit expected(unexpect_t, Args&&...);
		template<class U, class... Args>
		constexpr explicit expected(unexpect_t, std::initializer_list<U>, Args&&...);

		constexpr ~expected();

		constexpr expected& operator=(const expected&);
		constexpr expected& operator=(expected&&) noexcept(/* see description */);
		template<class G>
		constexpr expected& operator=(const unexpected<G>&);
		template<class G>
		constexpr expected& operator=(unexpected<G>&&);
		constexpr void emplace() noexcept;

		constexpr void swap(expected&) noexcept(/* see description */);
		friend constexpr void swap(expected&, expected&) noexcept(/* see description */);

		constexpr explicit operator bool() const noexcept;
		constexpr bool has_value() const noexcept;
		constexpr void operator*() const noexcept;
		constexpr void value() const&;
		constexpr void value() &&;
		constexpr const E& error() const&;
		constexpr E& error() &;
		constexpr const E&& error() const&&;
		constexpr E&& error() &&;

		template<class T2, class E2>
		requires std::is_void_v<T2>
		friend constexpr bool operator==(const expected& x, const expected<T2, E2>& y);
		template<class E2>
		friend constexpr bool operator==(const expected&, const unexpected<E2>&);

	private:
		bool has_val;
		union {
			E unex;
		};
	};
}
