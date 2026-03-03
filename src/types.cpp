#include <expected>
#include <string>
#include <god/util.hpp>
#include <god/types.hpp>
#include <god/output.hpp>
#include <god/input.hpp>
#include <god/token.hpp>
#include <god/scanner.hpp>
#include <god/parse.hpp>

namespace god {
    
auto document::load(const std::string& inputfile) -> void {
    auto in = input::file(inputfile);
    if (not in) in.error().die();
    auto scanner = god::scanner(in.value());
    auto tokens = scanner.scan();
    if (not tokens) tokens.error().die();
    auto doc = god::parse::document(tokens.value());
    if (not doc) doc.error().die();
    fields = std::move(doc.value().fields);
}

auto document::load(std::istream& is) -> void {
    auto in = input::stream(is);
    if (not in) in.error().die();
    auto scanner = god::scanner(in.value());
    auto tokens = scanner.scan();
    if (not tokens) tokens.error().die();
    auto doc = god::parse::document(tokens.value());
    if (not doc) doc.error().die();
    fields = std::move(doc.value().fields);
}

auto operator>>(std::istream& is, god::document& doc) -> std::istream& {
    // std::deque<std::string> lines;
    // std::string line;
    // while (std::getline(is, line)) {
    //     lines.push_back(line);
    // }
    auto in = input::stream(is);
    if (not in) in.error().die();
    auto scanner = god::scanner(in.value());
    auto tokens = scanner.scan();
    if (not tokens) tokens.error().die();
    auto ndoc = god::parse::document(tokens.value());
    if (not ndoc) ndoc.error().die();
    doc.fields = std::move(ndoc.value().fields);
    return is;
}

std::ostream &operator<<(std::ostream& os, god::document& doc) {
    os << doc.json();
    return os;
}

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

auto map::clobber(field& f) noexcept -> map& {
    for (std::size_t n = 0; n < this->size(); ++n) {
        if ((*this).at(n).name == f.name) {
            (*this).erase(this->begin() + n);
        }
    }
    this->push_back(f);
    return *this;
}

auto map::add(field& f) noexcept -> std::expected<int, error> {
    for (std::size_t n = 0; n < this->size(); ++n) {
        if ((*this).at(n).name == f.name) {
            return std::unexpected<error>{
                std::format("a field with identifier '{}' is already present", f.name)
            };
        }
    }
    this->push_back(f);
    return 0;
}

auto map::operator[](std::string_view name) const noexcept -> std::expected<const field*, error> {
    for (const auto& f: *this) {
        if (f.name == name) return &f;
    }
    return std::unexpected(error(std::format("no match found for '{}'", name)));
}

auto document::operator[](std::string_view name) const noexcept -> std::expected<const value*, error> {
    const std::vector<field>* flds = &this->fields;

    for (const auto& f: *flds) {
        if (f.name == name) {
            return &f.val;
        }
    }

    return std::unexpected{error{
        std::format("no match found for '{}'", name)
    }};
}


};