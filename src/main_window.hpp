#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QTreeWidget>

#include "task.hpp"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    
protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onTreeWidgetItemChanged(QTreeWidgetItem* item, int column);
    void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);
    void onTreeWidgetCustomContextMenu(const QPoint& point);

private:
    enum class ItemType {
        Title = 0,
        CreatedDate = 1,
        EditedDate = 2,
        Description = 3,
        Completed = 4
    };

    QString getAnyPath(const std::string& jsonKey) const;
    void loadTasks();
    void saveTasks(); // Non-const. idk why but QMessageBox cannot be called from const functions

    void addTask(const Task& task, int idx);
    void editTask();
    void removeTask();

    void rebuildTree();

private:
    Ui::MainWindow* mUi;
    QList<Task> mTasks;
};

#endif
