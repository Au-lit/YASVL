#pragma once
#include <format>
#include <cstdint>
#include <compare>
#include <optional>
#include <string_view>
#include <iterator>
#include <ostream>

namespace version {
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
			std::format_to(out, "v{}.{}.{}", major, minor, patch);
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
					std::format_to(out, ".{}", pre_release_number.value());
				}
			}
			if (build) {
				std::format_to(out, "+{}", build.value());
			}
		}

		[[nodiscard]] auto to_string() const noexcept {
			std::string out;
			format_to(std::back_inserter(out));
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
}

namespace std {
	template<> struct formatter<version::version> {
		constexpr auto parse(std::format_parse_context& ctx) {			
			// we might want to parse like &ma&bu
		}

		//constexpr auto format(const version::version& value, std::format_context& ctx) { value.format_to(ctx.out()); }
	};
}