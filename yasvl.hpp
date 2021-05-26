#pragma once
// For the liscence see at the end of this document.
static_assert(__has_include(<format>) || __has_include("fmt/format.h"),
	R"(To use yasvl you need either "fmt/format.h" or <format> stdlib support.)");

#include <format>
#include <cstdint>
#include <compare>
#include <optional>
#include <string_view>
#include <iterator>
#include <ostream>

namespace yasvl {
	namespace detail {
		#if __has_include(<format>)
			#include <format>
			using namespace std;
		#elif __has_include("fmt/format.h")
			#include "fmt/format.h"
			using namespace fmt;
		#endif
	}

	enum class pre_release /*implementation defined underlying type*/ : std::uint_fast8_t { alpha, beta, rc, none };
	struct version {
		template<typename T1, typename T2> 
		friend std::partial_ordering operator<=>(const std::optional<T1>& opt1, const std::optional<T2>& opt2) {
			if (!opt1 && !opt2) return std::partial_ordering::unordered;
			if (!opt1) return std::partial_ordering::less;
			if (!opt2) return std::partial_ordering::greater;
			return opt1.value() <=> opt2.value();
		}
		std::uint_fast8_t major = 1;
		std::uint_fast8_t minor = 0;
		std::uint_fast8_t patch = 0;
		pre_release pre_release_type = pre_release::none;
		std::optional<std::uint_fast8_t> pre_release_number = std::nullopt;
		std::optional<std::uint_fast16_t> build = std::nullopt;
		constexpr auto operator<=>(const version&) const noexcept = default;
		constexpr bool operator==(const version&) const noexcept = default;
		/*constexpr*/ static version from_string(const std::string_view& str) noexcept {}
		constexpr void format_to(std::output_iterator<const char&> auto out) const noexcept {
			detail::format_to(out, "v{}.{}.{}", major, minor, patch);
			if (pre_release_type != pre_release::none) {
				switch (pre_release_type) {
				case pre_release::alpha:
					for (const auto& ch : "-alpha")
						out++ = ch;
					break;
				case pre_release::beta:
					for (const auto& ch : "-beta")
						out++ = ch;
					break;
				case pre_release::rc:
					for (const auto& ch : "-rc")
						out++ = ch;
					break;
				case pre_release::none: 
					[[fallthrough]];
				default:
					break;
				}
				if (pre_release_number) {
					detail::format_to(out, ".{}", pre_release_number.value());
				}
			}
			if (build) {
				detail::format_to(out, "+{}", build.value());
			}
		}

		[[nodiscard]] auto to_string() const noexcept {
			std::string out;
			(*this).format_to(std::back_inserter(out));
			return out;
		}

		friend std::ostream& operator<<(std::ostream& out, const version& ver) {
			ver.format_to(std::ostream_iterator<char>(out));
			return out;
		}
	};

	constexpr version compiler_version{ 
		#if defined(__GNUC__)
		__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
		#elif defined(__clang__)
		__clang_major__, __clang_minor__, __clang_patchlevel__
		#elif defined(_MSC_FULL_VER)
		_MSC_FULL_VER / 1'000'0000 // too lazy to do more
		#endif

	};

	constexpr version cpp_version{
		[]() -> version {
			if constexpr (__cplusplus == 1) return { 0, 0, 0, pre_release::alpha };
			else if constexpr (__cplusplus <= 199711L) return { 0, 0, 0, pre_release::beta };
			else if constexpr (__cplusplus <= 201103L) return { 11, 0, 0 };
			else if constexpr (__cplusplus <= 201402L) return { 14, 0, 0 };
			else if constexpr (__cplusplus <= 201703L) return { 17, 0, 0 };
			else if constexpr (__cplusplus <= 202002L) return { 20, 0, 0 };
			else return { 23, 0, 0 };
		}()
	};
	version yasvl_version{ 1, 1, 0 };
}

namespace std {
	template<> struct formatter<yasvl::version> {
		constexpr auto parse(std::format_parse_context& ctx) {			
			// we might want to parse like &ma&bu
		}

		//constexpr auto format(const version::version& value, std::format_context& ctx) { value.format_to(ctx.out()); }
	};
}

// This software is distributed under the MIT License
//
// Copyright(c) 2021 Ollivier Roberge
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.