#ifndef TASK_DIALOG_HPP
#define TASK_DIALOG_HPP

#include <QDialog>

#include "task.hpp"

namespace Ui { class TaskDialog; }

class TaskDialog : public QDialog {
    Q_OBJECT

public:
    explicit TaskDialog(QWidget* parent = nullptr);
    explicit TaskDialog(const Task& task, QWidget* parent = nullptr);
    ~TaskDialog() override;
    
    Task getTask();

private:
    Ui::TaskDialog* mUi;
    bool mIsEdit;
    bool mCompleted;
    QDateTime mCreatedDate; 
};

#endif // TASK_DIALOG_HPP
