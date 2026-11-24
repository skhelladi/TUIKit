#include "tuikit/io/TUICSVTailReader.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>

namespace TUIKIT {

namespace {

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r");
    if (first == std::string::npos) return "";
    return text.substr(first, text.find_last_not_of(" \t\r") - first + 1);
}

} // namespace

TUICSVTailReader::TUICSVTailReader(std::string path) : path_(std::move(path)) {}

TUICSVTailReader& TUICSVTailReader::setColumns(std::size_t x_column, std::size_t y_column) {
    x_column_ = x_column;
    y_column_ = y_column;
    x_name_.clear();
    y_name_.clear();
    error_.clear();
    valid_ = true;
    return *this;
}

TUICSVTailReader& TUICSVTailReader::setColumns(const std::string& x_name, const std::string& y_name) {
    x_name_ = x_name;
    y_name_ = y_name;
    if (first_line_done_) resolveNames();
    return *this;
}

std::vector<std::string> TUICSVTailReader::split(const std::string& line, const std::string& separator) const {
    std::vector<std::string> fields;
    if (separator == " ") {  // runs of whitespace
        std::istringstream stream(line);
        std::string field;
        while (stream >> field) fields.push_back(field);
        return fields;
    }
    std::size_t start = 0, end;
    while ((end = line.find(separator, start)) != std::string::npos) {
        fields.push_back(trim(line.substr(start, end - start)));
        start = end + separator.size();
    }
    fields.push_back(trim(line.substr(start)));
    return fields;
}

bool TUICSVTailReader::toNumber(const std::string& field, double& value) const {
    std::string text = field;
    if (separator_ == ";") {  // decimal comma
        for (char& c : text) {
            if (c == ',') c = '.';
        }
    }
    char* end = nullptr;
    value = std::strtod(text.c_str(), &end);
    return !text.empty() && end == text.c_str() + text.size();
}

void TUICSVTailReader::readFirstLine(const std::string& line) {
    // ';' is never a decimal mark: when present it is the separator. Otherwise ',' if any, else whitespace.
    separator_ = line.find(';') != std::string::npos ? ";" : line.find(',') != std::string::npos ? "," : " ";
    const auto fields = split(line, separator_);
    double value = 0.0;
    has_header_ = false;
    for (const auto& field : fields) {
        if (!toNumber(field, value)) {
            has_header_ = true;
            break;
        }
    }
    if (has_header_) {
        names_ = fields;
    } else {
        names_.clear();
        for (std::size_t i = 0; i < fields.size(); ++i) names_.push_back("col" + std::to_string(i + 1));
    }
    first_line_done_ = true;
    if (!x_name_.empty() || !y_name_.empty()) resolveNames();
}

void TUICSVTailReader::resolveNames() {
    error_.clear();
    valid_ = true;
    const auto find = [&](const std::string& name, std::size_t& column) {
        const auto it = std::find(names_.begin(), names_.end(), name);
        if (it == names_.end()) {
            error_ = "column '" + name + "' not found in " + path_;
            valid_ = false;
            return;
        }
        column = static_cast<std::size_t>(it - names_.begin());
    };
    if (!x_name_.empty()) find(x_name_, x_column_);
    if (!y_name_.empty()) find(y_name_, y_column_);
}

bool TUICSVTailReader::parse(const std::string& line, double& x, double& y) const {
    const auto fields = split(line, separator_);
    if (x_column_ >= fields.size() || y_column_ >= fields.size()) return false;
    return toNumber(fields[x_column_], x) && toNumber(fields[y_column_], y);
}

bool TUICSVTailReader::poll(std::vector<std::pair<double, double>>& out) {
    if (!stream_.is_open()) {
        stream_.open(path_);
        if (!stream_.is_open()) return false;
        position_ = 0;
        pending_.clear();
        names_.clear();
        first_line_done_ = false;
        if (!x_name_.empty() || !y_name_.empty()) resolveNames();
    }
    stream_.clear();
    stream_.seekg(0, std::ios::end);
    const std::streamoff size = stream_.tellg();
    if (size < 0) return false;
    if (static_cast<std::streamoff>(position_) > size) {  // file was truncated: start over
        position_ = 0;
        pending_.clear();
        names_.clear();
        first_line_done_ = false;
        if (!x_name_.empty() || !y_name_.empty()) resolveNames();
    }
    if (static_cast<std::streamoff>(position_) == size) return true;
    stream_.seekg(static_cast<std::streampos>(position_));
    std::string chunk(static_cast<std::size_t>(size) - position_, '\0');
    stream_.read(&chunk[0], static_cast<std::streamsize>(chunk.size()));
    chunk.resize(static_cast<std::size_t>(stream_.gcount()));
    position_ += chunk.size();

    pending_ += chunk;
    std::size_t start = 0, newline;
    while ((newline = pending_.find('\n', start)) != std::string::npos) {
        const std::string line = trim(pending_.substr(start, newline - start));
        start = newline + 1;
        if (line.empty()) continue;
        if (!first_line_done_) {
            readFirstLine(line);
            if (has_header_) continue;
        }
        double x, y;
        if (valid_ && parse(line, x, y)) out.emplace_back(x, y);
    }
    pending_.erase(0, start);  // keep the trailing partial line for the next poll
    return true;
}

} // namespace TUIKIT
