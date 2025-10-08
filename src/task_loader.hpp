#ifndef TASK_LOADER_HPP
#define TASK_LOADER_HPP

#include <QList>

#include "task.hpp"

class TaskLoader {
public:
    static QList<Task> load(const QString& filePath);
    static void save(const QString& filePath, const QList<Task>& tasks);
};

#endif // TASK_LOADER_HPP
