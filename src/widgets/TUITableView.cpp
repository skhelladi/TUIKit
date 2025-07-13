#include "tuikit/widgets/TUITableView.h"
#include "tuikit/core/TUIStyle.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include <ftxui/dom/table.hpp>

namespace TUIKIT
{

    TUITableView::TUITableView(const std::vector<std::string> &headers, const std::vector<std::vector<std::string>> &data, TUIWidget * /*parent*/)
        : TUIWidget(ftxui::Container::Vertical({})), // Call base constructor with a default component
          headers_(headers),
          data_(data),
          selected_row_(0),
          selected_col_(0),
          is_editing_(false),
          column_widths_(headers.size(), 0)
    {
        using namespace ftxui;

        // Calculate column widths first
        for (size_t i = 0; i < headers_.size(); ++i)
        {
            column_widths_[i] = std::max(column_widths_[i], (int)headers_[i].length());
        }
        for (const auto &row : data_)
        {
            for (size_t c = 0; c < row.size(); ++c)
            {
                if (c < column_widths_.size())
                {
                    column_widths_[c] = std::max(column_widths_[c], (int)row[c].length());
                }
            }
        }

        // 1. Initialize input_component_
        input_component_ = ftxui::Input(&editing_text_, "");

        // 2. Initialize table_component_ with its own renderer and event handler
        table_component_ = ftxui::Container::Vertical({}); // Initialize as a container
        table_component_ = ftxui::Renderer(table_component_, [this]()
                                           {
            using namespace ftxui;
            std::vector<std::vector<std::string>> table_data;
            table_data.push_back(headers_); // header
            for (const auto& row : data_) {
                std::vector<std::string> row_data = row;
                if (row_data.size() < headers_.size())
                    row_data.resize(headers_.size(), "");
                table_data.push_back(row_data);
            }

            ftxui::Table table(table_data);

            auto &theme = TUIStyle::instance().currentTheme();
            for (size_t c = 0; c < headers_.size(); ++c) {
                table.SelectCell(c, 0).Decorate(bold | color(theme.primary) | bgcolor(theme.background));
            }

                        if (!data_.empty() && !headers_.empty()) {
                table.SelectCell(selected_col_, selected_row_+1).Decorate(bgcolor(Color::Blue));
            }
            return ftxui::borderDouble(table.Render()); });

        table_component_ = ftxui::CatchEvent(table_component_, [this](ftxui::Event event)
                                             {
            // Arrow navigation
            if (event == ftxui::Event::ArrowUp) {
                if (selected_row_ > 0) selected_row_--;
                if (on_select_) on_select_(selected_row_, selected_col_);
                return true;
            }
            if (event == ftxui::Event::ArrowDown) {
                if (selected_row_ < (int)data_.size() - 1) selected_row_++;
                if (on_select_) on_select_(selected_row_, selected_col_);
                return true;
            }
            if (event == ftxui::Event::ArrowLeft) {
                if (selected_col_ > 0) selected_col_--;
                if (on_select_) on_select_(selected_row_, selected_col_);
                return true;
            }
            if (event == ftxui::Event::ArrowRight) {
                if (selected_col_ < (int)headers_.size() - 1) selected_col_++;
                if (on_select_) on_select_(selected_row_, selected_col_);
                return true;
            }

            return false; });

        // 3. Initialize the main component_ (inherited from TUIWidget) as a container of both children
        TUIWidget::component_ = ftxui::Container::Vertical({table_component_, input_component_});

        // 4. Set up the renderer for the main component_ to render its active child
        TUIWidget::component_ = ftxui::Renderer(TUIWidget::component_, [this]() -> ftxui::Element
                                                { return TUIWidget::component_->ActiveChild()->Render(); });

        // 5. Set up the event handler for the main component_ to manage active child and mode switching
        TUIWidget::component_ |= ftxui::CatchEvent([this](ftxui::Event event)
                                                   {
            if (is_editing_) {
                if (event == ftxui::Event::Return) {
                    is_editing_ = false;
                    if (selected_row_ >= 0 && selected_row_ < (int)data_.size() && selected_col_ >= 0 && selected_col_ < (int)headers_.size()) {
                        data_[selected_row_][selected_col_] = editing_text_;
                        if (on_edit_complete_) on_edit_complete_(selected_row_, selected_col_, editing_text_);
                    }
                    TUIWidget::component_->SetActiveChild(table_component_.get()); // Switch to table
                    return true;
                }
                if (event == ftxui::Event::Escape) {
                    is_editing_ = false;
                    TUIWidget::component_->SetActiveChild(table_component_.get()); // Switch to table
                    return true;
                }
                return input_component_->OnEvent(event); // Pass other events to input
            } else {
                if (event == ftxui::Event::Return) {
                    if (selected_row_ >= 0 && selected_row_ < (int)data_.size() && selected_col_ >= 0 && selected_col_ < (int)headers_.size()) {
                        is_editing_ = true;
                        editing_text_ = data_[selected_row_][selected_col_];
                        TUIWidget::component_->SetActiveChild(input_component_.get()); // Switch to input
                    }
                    if (on_enter_) on_enter_(selected_row_, selected_col_);
                    return true;
                }
                return table_component_->OnEvent(event); // Pass other events to table
            } });

        // Initial selection and focus
        if (on_select_)
            on_select_(selected_row_, selected_col_);
        TUIWidget::component_->SetActiveChild(table_component_.get()); // Ensure table is active and focused initially
    }

    void TUITableView::onSelect(OnSelectCallback cb)
    {
        on_select_ = cb;
    }

    void TUITableView::onEnter(OnEnterCallback cb)
    {
        on_enter_ = cb;
    }

    void TUITableView::onEditComplete(OnEditCompleteCallback cb)
    {
        on_edit_complete_ = cb;
    }

}