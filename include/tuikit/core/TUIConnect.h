#pragma once

#include <functional>
#include <memory>
#include "tuikit/widgets/TUIButton.h"
#include "tuikit/widgets/TUICheckBox.h"
#include "tuikit/widgets/TUISlider.h"
#include "tuikit/widgets/TUITextField.h"
#include "tuikit/widgets/TUIComboBox.h"
#include "tuikit/widgets/TUIRadioBox.h"
#include "tuikit/widgets/TUIMenu.h"
#include "tuikit/widgets/TUITreeView.h"
#include "tuikit/widgets/TUISpinBox.h"
#include "tuikit/widgets/TUIDoubleSpinBox.h"
#include "tuikit/widgets/TUIListView.h"
#include "tuikit/widgets/TUITableView.h"

namespace TUIKIT {

// TUIButton: onClick() -> void()
inline void connect(std::shared_ptr<TUIButton> sender, std::function<void()> slot) {
    sender->onClick(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIButton> sender, Object* receiver, void (Object::*slot)()) {
    sender->onClick([receiver, slot]() { (receiver->*slot)(); });
}

// TUICheckBox: onChange(bool) -> void(bool)
inline void connect(std::shared_ptr<TUICheckBox> sender, std::function<void(bool)> slot) {
    sender->onChange(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUICheckBox> sender, Object* receiver, void (Object::*slot)(bool)) {
    sender->onChange([receiver, slot](bool value) { (receiver->*slot)(value); });
}

// TUISlider: onChange(float) -> void(float)
inline void connect(std::shared_ptr<TUISlider> sender, std::function<void(float)> slot) {
    sender->onChange(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUISlider> sender, Object* receiver, void (Object::*slot)(float)) {
    sender->onChange([receiver, slot](float value) { (receiver->*slot)(value); });
}

// TUITextField: onChange(const std::string&) -> void(const std::string&)
inline void connect(std::shared_ptr<TUITextField> sender, std::function<void(const std::string&)> slot) {
    sender->onChange(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUITextField> sender, Object* receiver, void (Object::*slot)(const std::string&)) {
    sender->onChange([receiver, slot](const std::string& value) { (receiver->*slot)(value); });
}

// TUITextField: onEnter() -> void()
inline void connect(std::shared_ptr<TUITextField> sender, std::function<void()> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUITextField> sender, Object* receiver, void (Object::*slot)()) {
    sender->onEnter([receiver, slot]() { (receiver->*slot)(); });
}

// TUIComboBox: onSelect(int) -> void(int)
inline void connect(std::shared_ptr<TUIComboBox> sender, std::function<void(int)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIComboBox> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onSelect([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUIRadioBox: onSelect(int) -> void(int)
inline void connect(std::shared_ptr<TUIRadioBox> sender, std::function<void(int)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIRadioBox> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onSelect([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUIMenu: onSelect(int) -> void(int)
inline void connect(std::shared_ptr<TUIMenu> sender, std::function<void(int)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIMenu> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onSelect([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUIMenu: onEnter(int) -> void(int)
inline void connectOnEnter(std::shared_ptr<TUIMenu> sender, std::function<void(int)> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connectOnEnter(std::shared_ptr<TUIMenu> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onEnter([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUITreeView: onSelect(const std::string&) -> void(const std::string&)
inline void connect(std::shared_ptr<TUITreeView> sender, std::function<void(const std::string&)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUITreeView> sender, Object* receiver, void (Object::*slot)(const std::string&)) {
    sender->onSelect([receiver, slot](const std::string& value) { (receiver->*slot)(value); });
}

// TUISpinBox: onChange(int) -> void(int)
inline void connect(std::shared_ptr<TUISpinBox> sender, std::function<void(int)> slot) {
    sender->onChange(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUISpinBox> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onChange([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUISpinBox: onEnter() -> void()
inline void connect(std::shared_ptr<TUISpinBox> sender, std::function<void()> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUISpinBox> sender, Object* receiver, void (Object::*slot)()) {
    sender->onEnter([receiver, slot]() { (receiver->*slot)(); });
}

// TUIDoubleSpinBox: onChange(double) -> void(double)
inline void connect(std::shared_ptr<TUIDoubleSpinBox> sender, std::function<void(double)> slot) {
    sender->onChange(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIDoubleSpinBox> sender, Object* receiver, void (Object::*slot)(double)) {
    sender->onChange([receiver, slot](double value) { (receiver->*slot)(value); });
}

// TUIDoubleSpinBox: onEnter() -> void()
inline void connect(std::shared_ptr<TUIDoubleSpinBox> sender, std::function<void()> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIDoubleSpinBox> sender, Object* receiver, void (Object::*slot)()) {
    sender->onEnter([receiver, slot]() { (receiver->*slot)(); });
}

// TUIListView: onSelect(int) -> void(int)
inline void connect(std::shared_ptr<TUIListView> sender, std::function<void(int)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUIListView> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onSelect([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUIListView: onEnter(int) -> void(int)
inline void connectOnEnter(std::shared_ptr<TUIListView> sender, std::function<void(int)> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connectOnEnter(std::shared_ptr<TUIListView> sender, Object* receiver, void (Object::*slot)(int)) {
    sender->onEnter([receiver, slot](int value) { (receiver->*slot)(value); });
}

// TUITableView: onSelect(int, int) -> void(int, int)
inline void connect(std::shared_ptr<TUITableView> sender, std::function<void(int, int)> slot) {
    sender->onSelect(std::move(slot));
}

template <typename Object>
void connect(std::shared_ptr<TUITableView> sender, Object* receiver, void (Object::*slot)(int, int)) {
    sender->onSelect([receiver, slot](int row, int col) { (receiver->*slot)(row, col); });
}

// TUITableView: onEnter(int, int) -> void(int, int)
inline void connectOnEnter(std::shared_ptr<TUITableView> sender, std::function<void(int, int)> slot) {
    sender->onEnter(std::move(slot));
}

template <typename Object>
void connectOnEnter(std::shared_ptr<TUITableView> sender, Object* receiver, void (Object::*slot)(int, int)) {
    sender->onEnter([receiver, slot](int row, int col) { (receiver->*slot)(row, col); });
}

} // namespace TUIKIT