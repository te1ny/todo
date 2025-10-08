#include "task_loader.hpp"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <fstream>
#include <nlohmann/json.hpp>

#include <stdexcept>

QList<Task> TaskLoader::load(const QString& filePath) {
    QFile file(filePath);
    if (!file.exists())
        throw std::invalid_argument("File does not exist. Bad path: " + filePath.toStdString());
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Couldn't open the file for read. Path: " + filePath.toStdString());

    auto fileData = file.readAll();
    file.close();
    nlohmann::json tasksJson = nlohmann::json::parse(fileData.toStdString());
    if (!tasksJson.is_array())
        throw std::runtime_error("JSON is not an array. Path: " + filePath.toStdString());

    QList<Task> tasks{};
    tasks.reserve(tasksJson.size());

    for (const auto& taskJson : tasksJson)
        tasks.append(Task::fromJson(taskJson));

    return tasks;
}

void TaskLoader::save(const QString& filePath, const QList<Task>& tasks) {
    QFileInfo fileInfo(filePath);
    QDir().mkpath(fileInfo.path());

    std::ofstream out(filePath.toStdString());
    if (!out)
        throw std::runtime_error("Couldn't open the file for write. Path: " + filePath.toStdString());

    nlohmann::json json = nlohmann::json::array();
    for (const auto& task : tasks)
        json.push_back(Task::toJson(task));

    out << json.dump(4);
    out.close();
}
