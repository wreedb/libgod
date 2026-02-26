#include "god/base.hpp"
#include <expected>
#include <god/util.hpp>
#include <god/types.hpp>
#include <god/output.hpp>

namespace god {

auto document::json() const noexcept -> std::string {
    return god::output::json::document(*this);
}

auto document::string() const noexcept -> std::string {
    return god::output::canon::document(*this);
}

auto document::lookup(std::string_view query) const noexcept -> std::expected<const value*, error> {
    auto parts = util::splitsv(query, '.');
    
    const std::vector<field>* current_fields = &this->fields;
    const god::value* found = nullptr;

    for (std::size_t i = 0; i < parts.size(); ++i) {
        const auto& part = parts[i];
        found = nullptr;

        for (auto& f : *current_fields) {
            if (f.name == part) {
                found = &f.val;
                break;
            }
        }

        if (!found) {
            return std::unexpected{error{std::format("key '{}' not found", part)}};
        }

        if (i < parts.size() - 1) {
            if (!found->is<god::map>()) {
                return std::unexpected{error{std::format(
                    "'{}' is not a map and cannot be traversed", part
                )}};
            }
            current_fields = &found->as<god::map>();
        }
    }

    return found;
}

auto document::clobber(god::field& f) noexcept -> document& {
    for (std::size_t n = 0; n < fields.size(); ++n) {
        // if there is an existing field with the same identifier
        // remove it before adding this one.
        if (fields[n].name == f.name)
            fields.erase(fields.begin() + n);
    }
    fields.push_back(f);
    return *this;
}

auto document::add(god::field& f) noexcept -> std::expected<int, error> {
    for (std::size_t n = 0; n < fields.size(); ++n) {
        // if there is an existing field with the same identifier
        // remove it before adding this one.
        if (fields[n].name == f.name)
            return std::unexpected<error>(std::format(
                "a field with identifier '{}' is already present",
                f.name
            ));
    }
    fields.push_back(f);
    return 0;
}
    
};