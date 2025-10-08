#include "task_dialog.hpp"
#include "ui_task_dialog.h"

#include <QString>
#include <qpushbutton.h>

TaskDialog::TaskDialog(QWidget* parent) 
    : QDialog(parent)
    , mUi(new Ui::TaskDialog)
    , mIsEdit(false)
    , mCompleted(false)
    , mCreatedDate(QDateTime()) {
    mUi->setupUi(this);
    setWindowTitle("Добавление задачи");
    connect(mUi->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(mUi->rejectButton, &QPushButton::clicked, this, &QDialog::reject);
}

TaskDialog::TaskDialog(const Task& task, QWidget* parent) 
    : QDialog(parent)
    , mUi(new Ui::TaskDialog)
    , mIsEdit(true)
    , mCompleted(task.isCompleted())
    , mCreatedDate(task.getCreatedDate()) {
    mUi->setupUi(this);
    setWindowTitle("Редактирование задачи");
    mUi->taskTitle->setText(task.getTitle());
    mUi->taskDescription->setText(task.getDescription());
    connect(mUi->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(mUi->rejectButton, &QPushButton::clicked, this, &QDialog::reject);
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
        mCreatedDate
    };
}
