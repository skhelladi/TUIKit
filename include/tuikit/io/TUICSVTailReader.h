#ifndef TUIKIT_TUICSVTAILREADER_H
#define TUIKIT_TUICSVTAILREADER_H

#include <cstddef>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace TUIKIT {

// Reads the complete lines appended to a CSV file between two poll() calls and yields
// (x, y) pairs. Format: the first line is either a header with the column names, or data
// (default names col1, col2, ... are then used). Fields are separated by ',', ';' (comma
// then accepted as decimal mark) or whitespace. Lines that do not parse are skipped.
class TUICSVTailReader {
public:
    explicit TUICSVTailReader(std::string path);

    // Column selection by index (default 0 and 1) or by column name.
    TUICSVTailReader& setColumns(std::size_t x_column, std::size_t y_column);
    TUICSVTailReader& setColumns(const std::string& x_name, const std::string& y_name);

    // Appends the (x, y) pairs of the new complete lines to out. Returns false while the
    // file does not exist.
    bool poll(std::vector<std::pair<double, double>>& out);

    // Column names: header if the first line is one, otherwise col1, col2, ... Empty until
    // the first line has been read.
    const std::vector<std::string>& columnNames() const { return names_; }
    // Non empty when a selection by name failed; no rows are produced in that case.
    const std::string& error() const { return error_; }

private:
    std::vector<std::string> split(const std::string& line, const std::string& separator) const;
    bool toNumber(const std::string& field, double& value) const;
    void readFirstLine(const std::string& line);
    void resolveNames();
    bool parse(const std::string& line, double& x, double& y) const;

    std::string path_, pending_, separator_ = ",", error_;
    std::vector<std::string> names_;
    std::string x_name_, y_name_;  // selection by name, resolved once the first line is known
    std::size_t x_column_ = 0, y_column_ = 1, position_ = 0;
    bool first_line_done_ = false, has_header_ = false, valid_ = true;
    std::ifstream stream_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUICSVTAILREADER_H
