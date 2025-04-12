#pragma once

template <typename Base, typename Tag>
struct StrongAlias {
  explicit StrongAlias(Base value) : value{std::move(value)} { }

  [[nodiscard]] bool
  operator==(const StrongAlias<Base,Tag>& other) const noexcept {
    return value == other.value;
  }

  Base value;
};