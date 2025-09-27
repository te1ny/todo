#ifndef TODO_ITEM_HPP
#define TODO_ITEM_HPP

#include <QString>
#include <string>
#include <type_traits>
#include <utility>

template <typename T>
concept is_string_type = std::is_convertible_v<T, std::string>; 

class TodoItem {
public:
    TodoItem() = default;

    template <is_string_type StringT>
    TodoItem(
            StringT&& p_text, 
            bool p_completed)
        : _text(std::forward<StringT>(p_text))
        , _completed(p_completed) {}

    template <is_string_type StringT>
    inline void set_text(StringT&& p_text) { _text = std::forward<StringT>(p_text); }
    inline const std::string& text() const { return _text; }

    inline bool is_completed() const { return _completed; }
    inline void set_completed(bool p_completed) { _completed = p_completed; } 

private:
    std::string _text;
    bool _completed;
};

#endif // TODO_ITEM_HPP
