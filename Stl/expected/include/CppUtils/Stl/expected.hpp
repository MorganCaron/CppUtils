#pragma once

#include <stdexcept>
#include <type_traits>
#include <initializer_list>

// https://en.cppreference.com/w/cpp/header/expected
namespace CppUtils::Stl
{
	// https://en.cppreference.com/w/cpp/utility/expected/unexpected
	template<class E>
	requires !std::is_void_v<E>
	class unexpected
	{
		static_assert(!std::is_same_v<E, unexpected<E>>, "E must not be unexpected<E>");
		static_assert(!std::is_reference_v<E>, "E must not be a reference");

	public:
		constexpr unexpected(const unexpected&) = default;
		constexpr unexpected(unexpected&&) = default;

		template<class Err = E>
		requires
			!std::is_same_v<std::remove_cvref_t<Err>, unexpected> &&
			!std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t> &&
			std::is_constructible_v<E, Err>
		constexpr explicit unexpected(Err&& e):
			unex{std::forward<Err>(e)}
		{}

		template<class... Args>
		requires std::is_constructible_v<E, Args...>
		constexpr explicit unexpected(std::in_place_t, Args&&... args):
			unex{std::forward<Args>(args)...}
		{}

		template<class U, class... Args>
		requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
		constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il, Args&&... args):
			unex{il, std::forward<Args>(args)...}
		{}

		constexpr unexpected& operator=(const unexpected&) = default;
		constexpr unexpected& operator=(unexpected&&) = default;

		constexpr const E& error() const& noexcept
		{
			return unex;
		}
		constexpr E& error() & noexcept
		{
			return unex;
		}
		constexpr const E&& error() const&& noexcept
		{
			return std::move(unex);
		}
		constexpr E&& error() && noexcept
		{
			return std::move(unex);
		}

		constexpr void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>)
		requires std::is_swappable_v<E>
		{
			using std::swap;
			swap(error(), other.error());
		}

		requires std::is_swappable_v<E>
		friend constexpr void swap(unexpected& x, unexpected& y) noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}

		template<class E2>
		friend constexpr bool operator==(const unexpected&, const unexpected<E2>&)
		{
			return x.error() == y.error()
		}

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
		explicit bad_expected_access(E e):
			unex{std::move(e)};
		{}

		const char* what() const noexcept override;
		{
			return "Bad expected access";
		}

		E& error() & noexcept
		{
			return unex;
		}
		const E& error() const& noexcept
		{
			return unex;
		}
		E&& error() && noexcept
		{
			return std::move(unex);
		}
		const E&& error() const&& noexcept
		{
			return std::move(unex);
		}

	private:
		E unex;
	};

	template<>
	class bad_expected_access<void>:
		public std::exception
	{
	protected:
		bad_expected_access() noexcept = default;
		bad_expected_access(const bad_expected_access&) = default;
		bad_expected_access(bad_expected_access&&) = default;
		bad_expected_access& operator=(const bad_expected_access&) = default;
		bad_expected_access& operator=(bad_expected_access&&) = default;

		~bad_expected_access() = default;

	public:
		const char* what() const noexcept override;
		{
			return "Bad expected access";
		}
	};

	struct unexpect_t
	{
		explicit unexpect_t() = default;
	};
	inline constexpr unexpect_t unexpect{};

	https://en.cppreference.com/w/cpp/utility/expected
	template<class T, class E>
	class expected
	{
		static_assert(!std::is_reference_v<T>, "T must not be a reference");
		static_assert(!std::is_same_v<T, unexpected<E>>, "T must not be unexpected<E>");
		static_assert(!std::is_same_v<T, std::in_place_t>, "T must not be in_place_t");
		static_assert(!std::is_same_v<T, unexpect_t>, "T must not be unexpect_t");
		static_assert(!std::is_reference_v<E>, "E must not be a reference");

	public:
		using value_type = T;
		using error_type = E;
		using unexpected_type = unexpected<E>;

		template<class U>
		using rebind = expected<U, error_type>;

		constexpr expected() noexcept(noexcept(T)):
			has_val{true},
			val{}
		{}
		constexpr explicit(/* see description */) 
		expected(const expected& other):
			has_val{other.has_value()}
		{
			if (!other.has_value())
				unex{other.error()};
			else
				val{*other};
		}
		constexpr explicit(/* see description */) 
		expected(expected&& other) noexcept(/* see description */):
			has_val{other.has_value()}
		{
			if (!other.has_value())
				unex{std::move(other.error())};
			else
				val{std::move(*other)};
		}
		template<class U, class G>
		constexpr explicit(!std::is_convertible_v<std::add_lvalue_reference_t<const U>, T> || !std::is_convertible_v<const G&, E>)
		expected(const expected<U, G>& other):
			has_val{other.has_value()}
		{
			using UF = std::add_lvalue_reference_t<const U>;
			using GF = const G&;
			if (!other.has_value())
				unex{std::forward<GF>(other.error())};
			else
				val{std::forward<UF>(*other)};
		}
		template<class U, class G>
		constexpr explicit(!std::is_convertible_v<U, T> || !std::is_convertible_v<G, E>)
		expected(expected<U, G>&& other):
			has_val{other.has_value()}
		{
			using UF = U;
			using GF = G;
			if (!other.has_value())
				unex{std::forward<GF>(other.error())};
			else
				val{std::forward<UF>(*other)};
		}

		template<class U = T>
		constexpr explicit(!std::is_convertible_v<U, T>)
		expected(U&& v):
			has_val{true},
			val(std::forward<U>(v))
		{}

		template<class G>
		explicit(!std::is_convertible_v<const G&, E>)
		constexpr expected(const unexpected<G>& e):
			has_val{false}
		{
			using GF = const G&;
			unex{std::forward<GF>(e.error())};
		}
		template<class G>
		explicit(!std::is_convertible_v<G, E>)
		constexpr expected(unexpected<G>&& e):
			has_val{false}
		{
			using GF = G;
			unex{std::forward<GF>(e.error())};
		}

		template<class... Args>
		requires std::is_constructible_v<T, Args...>
		constexpr explicit expected(std::in_place_t, Args&&... args):
			has_val{true},
			val{std::forward<Args>(args)...}
		{}
		template<class U, class... Args>
		requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
		constexpr explicit expected(std::in_place_t, std::initializer_list<U> il, Args&&... args):
			has_val{true},
			val{il, std::forward<Args>(args)...}
		{}
		template<class... Args>
		requires std::is_constructible_v<E, Args...>
		constexpr explicit expected(unexpect_t, Args&&... args):
			has_val{false},
			unex{std::forward<Args>(args)...}
		{}
		template<class U, class... Args>
		requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
		constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args):
			has_val{false},
			unex{il, std::forward<Args>(args)...}
		{}

		constexpr ~expected()
		{
			if (!has_value())
				~unex();
			else
				~val();
		}

		constexpr expected& operator=(const expected&);
		constexpr expected& operator=(expected&&) noexcept(/* see description */);
		template<class U = T> constexpr expected& operator=(U&&);
		template<class G>
		constexpr expected& operator=(const unexpected<G>&);
		template<class G>
		constexpr expected& operator=(unexpected<G>&&);

		template<class... Args>
		requires std::is_nothrow_constructible_v<T, Args...>
		constexpr T& emplace(Args&&... args) noexcept
		{
			if (has_value())
				~val();
			else
				~unex();
			val = T{std::forward<Args>(args)...};
			has_val = true;
		}
		template<class U, class... Args>
		requires std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>
		constexpr T& emplace(std::initializer_list<U> il, Args&&... args) noexcept
		{
			if (has_value())
				~val();
			else
				~unex();
			val = T{il, std::forward<Args>(args)...};
			has_val = true;
		}

		constexpr void swap(expected&) noexcept(/* see description */);
		friend constexpr void swap(expected& lhs, expected& rhs) noexcept(noexcept(lhs.swap(rhs)))
		{
			lhs.swap(rhs);
		}

		constexpr const T* operator->() const noexcept
		{
			return &val;
		}
		constexpr T* operator->() noexcept
		{
			return &val;
		}
		constexpr const T& operator*() const& noexcept
		{
			return val;
		}
		constexpr T& operator*() & noexcept
		{
			return val;
		}
		constexpr const T&& operator*() const&& noexcept
		{
			return std::move(val);
		}
		constexpr T&& operator*() && noexcept
		{
			return std::move(val);
		}
		constexpr explicit operator bool() const noexcept
		{
			return has_val;
		}
		constexpr bool has_value() const noexcept
		{
			return has_val;
		}
		constexpr const T& value() const&
		{
			if (!has_value())
				throw bad_exception_access(error());
			return val;
		}
		constexpr T& value() &
		{
			if (!has_value())
				throw bad_exception_access(error());
			return val;
		}
		constexpr const T&& value() const&&
		{
			if (!has_value())
				throw bad_exception_access(std::move(error()));
			return val;
		}
		constexpr T&& value() &&
		{
			if (!has_value())
				throw bad_exception_access(std::move(error()));
			return val;
		}
		constexpr const E& error() const&
		{
			return unex;
		}
		constexpr E& error() &
		{
			return unex;
		}
		constexpr const E&& error() const&&
		{
			return std::move(unex);
		}
		constexpr E&& error() &&
		{
			return std::move(unex);
		}
		template<class U>
		constexpr T value_or(U&&) const&
		{
			return bool(*this) ? **this : static_cast<T>(std::forward<U>(default_value));
		}
		template<class U>
		constexpr T value_or(U&&) &&
		{
			return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(default_value))
		}

		template<class T2, class E2>
		requires !std::is_void_v<T2>
		friend constexpr bool operator==(const expected& lhs, const expected<T2, E2>& rhs)
		{
			if (lhs.has_value() != rhs.has_value())
				return false;
			return x.has_value() ? *lhs == *rhs : lhs.error() == rhs.error();
		}
		template<class T2>
		friend constexpr bool operator==(const expected& x, const T2& val)
		{
			return x.has_value() && static_cast<bool>(*x == val);
		}
		template<class E2>
		friend constexpr bool operator==(const expected& x, const unexpected<E2>& e)
		{
			return !x.has_value() && static_cast<bool>(x.error() == e.error());
		}

	private:
		bool has_val;
		union {
			T val;
			E unex;
		};
	};

	// https://en.cppreference.com/w/cpp/header/expected
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
		expected(const expected& other):
			has_val{other.has_value()}
		{
			if (!other.has_value())
				unex{other.error()};
		}
		constexpr explicit(/* see description */)
		expected(expected&& other) noexcept(/* see description */):
			has_val{other.has_value()}
		{
			if (!other.has_value())
				unex{std::move(other.error())};
		}
		template<class U, class G>
		constexpr explicit(!std::is_convertible_v<std::add_lvalue_reference_t<const U>, T> || !std::is_convertible_v<const G&, E>)
		expected(const expected<U, G>& other):
			has_val{other.has_value()}
		{
			using UF = std::add_lvalue_reference_t<const U>;
			using GF = const G&;
			if (!other.has_value())
				unex{std::forward<GF>(other.error())};
		}
		template<class U, class G>
		constexpr explicit(!std::is_convertible_v<U, T> || !std::is_convertible_v<G, E>)
		expected(expected<U, G>&& other):
			has_val{other.has_value()}
		{
			using UF = U;
			using GF = G;
			if (!other.has_value())
				unex{std::forward<GF>(other.error())};
		}

		template<class G>
		explicit(!std::is_convertible_v<const G&, E>)
		constexpr expected(const unexpected<G>& e):
			has_val{false}
		{
			using GF = const G&;
			unex{std::forward<GF>(e.error())};
		}
		template<class G>
		explicit(!std::is_convertible_v<G, E>)
		constexpr expected(unexpected<G>&& e):
			has_val{false}
		{
			using GF = G;
			unex{std::forward<GF>(e.error())};
		}

		constexpr explicit expected(std::in_place_t) noexcept;
		template<class... Args>
		requires std::is_constructible_v<E, Args...>
		constexpr explicit expected(unexpect_t, Args&&... args):
			has_val{false},
			unex{std::forward<Args>(args)...}
		{}
		template<class U, class... Args>
		requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
		constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args):
			has_val{false},
			unex{il, std::forward<Args>(args)...}
		{}

		constexpr ~expected()
		{
			if (!has_value())
				~unex();
		}

		constexpr expected& operator=(const expected&);
		constexpr expected& operator=(expected&&) noexcept(/* see description */);
		template<class G>
		constexpr expected& operator=(const unexpected<G>&);
		template<class G>
		constexpr expected& operator=(unexpected<G>&&);

		constexpr void emplace() noexcept
		{
			if (!has_value())
				~unex();
			has_val = true;
		}

		constexpr void swap(expected&) noexcept(/* see description */);
		friend constexpr void swap(expected& lhs, expected& rhs) noexcept(noexcept(lhs.swap(rhs)))
		{
			lhs.swap(rhs);
		}

		constexpr explicit operator bool() const noexcept
		{
			return has_val;
		}
		constexpr bool has_value() const noexcept
		{
			return has_val;
		}
		constexpr void operator*() const noexcept
		{}
		constexpr void value() const&
		{
			if (!has_value())
				throw bad_expected_access(error());
		}
		constexpr void value() &&
		{
			if (!has_value())
				throw bad_expected_access(std::move(error()));
		}
		constexpr const E& error() const&
		{
			return unex;
		}
		constexpr E& error() &
		{
			return unex;
		}
		constexpr const E&& error() const&&
		{
			return std::move(unex);
		}
		constexpr E&& error() &&
		{
			return std::move(unex);
		}

		template<class T2, class E2>
		requires std::is_void_v<T2>
		friend constexpr bool operator==(const expected& lhs, const expected<T2, E2>& rhs)
		{
			if (lhs.has_value() != rhs.has_value())
				return false;
			return lhs.has_value() ? true : lhs.error() == rhs.error();
		}
		template<class E2>
		friend constexpr bool operator==(const expected& x, const unexpected<E2>& e)
		{
			return !x.has_value() && static_cast<bool>(x.error() == e.error());
		}

	private:
		bool has_val;
		union {
			E unex;
		};
	};
}
