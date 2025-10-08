#include "task.hpp"
#include <qdebug.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <stdexcept>
#include <string>

Task::Task(QString title,
           QString description,
           bool completed,
           QDateTime editedDate,
           QDateTime createdDate)
           : mTitle(std::move(title))
           , mDescription(std::move(description))
           , mCompleted(completed)
           , mEditedDate(std::move(editedDate))
           , mCreatedDate(std::move(createdDate)) {}

Task::Task(Task&& other) noexcept 
    : mTitle(std::move(other.mTitle))
    , mDescription(std::move(other.mDescription))
    , mCompleted(other.mCompleted)
    , mEditedDate(std::move(other.mEditedDate))
    , mCreatedDate(std::move(other.mCreatedDate)) {}

Task& Task::operator=(Task&& other) noexcept {
    if (this != &other) {
        mTitle       = std::move(other.mTitle);
        mDescription = std::move(other.mDescription);
        mCompleted   = other.mCompleted;
        mEditedDate  = std::move(other.mEditedDate);
        mCreatedDate = std::move(other.mCreatedDate);
    }
    return *this;
}

nlohmann::json Task::toJson(const Task& task) {
    nlohmann::json json;

    json["title"]        = task.mTitle.toStdString();
    json["description"]  = task.mDescription.toStdString();
    json["completed"]    = task.mCompleted;
    json["created_date"] = task.mCreatedDate.toString(Qt::ISODate).toStdString();
    json["edited_date"]  = task.mCreatedDate.toString(Qt::ISODate).toStdString();

    return json;
}

Task Task::fromJson(const nlohmann::json& json) {
    for (const auto& field : jsonFields) {
        if (!json.contains(field))
            throw std::runtime_error("Error when parsing JSON. Missing field: " + std::string(field));
    }
    
    QString createdDateStr = QString::fromStdString(json["created_date"].get<std::string>());
    QString editedDateStr  = QString::fromStdString(json["edited_date"].get<std::string>());


    QString title          = QString::fromStdString(json["title"].get<std::string>());
    QString description    = QString::fromStdString(json["description"].get<std::string>());
    bool completed         = json["completed"].get<bool>();
    QDateTime createdDate  = QDateTime::fromString(std::move(createdDateStr), Qt::ISODate);
    QDateTime editedDate  = QDateTime::fromString(std::move(editedDateStr), Qt::ISODate);

    QDebug(QtMsgType::QtDebugMsg) << createdDate;

    return Task{
        std::move(title),
        std::move(description),
        completed,
        std::move(editedDate),
        std::move(createdDate)
    };
}

const QString& Task::getTitle() const {
    return mTitle;
}

const QString& Task::getDescription() const {
    return mDescription;
}

void Task::setCompleted(bool completed) {
    mCompleted = completed;
}

bool Task::isCompleted() const {
    return mCompleted;
}

const QDateTime& Task::getCreatedDate() const {
    return mCreatedDate;
}

const QDateTime& Task::getEditedDate() const {
    return mEditedDate;
}
