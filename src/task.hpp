#ifndef TASK_HPP
#define TASK_HPP

#include <QString>
#include <QDateTime>

#include <nlohmann/json.hpp>
#include <qdatetime.h>
#include <utility>

class Task {
private:
    constexpr static std::array<const char*, 5> jsonFields = {
        "title",
        "description",
        "completed",
        "created_date",
        "edited_date"
    };

public:
    explicit Task(QString title = "",
                  QString description = "",
                  bool completed = false,
                  QDateTime editedDate = QDateTime::currentDateTime(),
                  QDateTime createdDate = QDateTime::currentDateTime());

    Task(const Task&) = default;
    Task& operator=(const Task&) = default;

    Task(Task&&) noexcept;
    Task& operator=(Task&&) noexcept;

    static nlohmann::json toJson(const Task& task);
    static Task fromJson(const nlohmann::json& json);

    template <typename String>
    void setTitle(String&& title) {
        mTitle = std::forward<String>(title);
        mEditedDate = QDateTime::currentDateTime();
    }
    const QString& getTitle() const;

    template <typename String>
    void setDescription(String&& description) {
        mDescription = std::forward<String>(description);
        mEditedDate = QDateTime::currentDateTime();
    }
    const QString& getDescription() const;

    void setCompleted(bool completed);
    bool isCompleted() const;

    const QDateTime& getCreatedDate() const;
    const QDateTime& getEditedDate() const;

private:
    QString mTitle;
    QString mDescription;
    bool mCompleted;
    QDateTime mEditedDate;
    QDateTime mCreatedDate;
};

#endif // TASK_HPP
