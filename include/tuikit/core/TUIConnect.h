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

} // namespace TUIKIT