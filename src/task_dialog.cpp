#include "task_dialog.hpp"
#include "ui_task_dialog.h"

#include <QString>

TaskDialog::TaskDialog(QWidget* parent) 
    : QDialog(parent)
    , mUi(new Ui::TaskDialog)
    , mIsEdit(false)
    , mCompleted(false)
    , mCreatedDate(QDateTime()){
    mUi->setupUi(this);
    setWindowTitle("Add task | Добавление задачи");
}

TaskDialog::TaskDialog(const Task& task, QWidget* parent) 
    : QDialog(parent)
    , mUi(new Ui::TaskDialog)
    , mIsEdit(true)
    , mCompleted(task.isCompleted())
    , mCreatedDate(task.getCreatedDate()) {
    mUi->setupUi(this);
    setWindowTitle("Edit task | Редактирование задачи");
}

TaskDialog::~TaskDialog() {
    delete mUi;
}
    
Task TaskDialog::getTask() {
    if (!mIsEdit)
        mCreatedDate = QDateTime::currentDateTime();

    return Task{
        mUi->taskTitle->text(),
        mUi->taskDescription->toPlainText(),
        mCompleted,
        QDateTime::currentDateTime(),
        std::move(mCreatedDate)
    };
}
