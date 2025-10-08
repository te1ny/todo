#include "main_window.hpp"
#include "ui_main_window.h"

#include <QFile>
#include <QMessageBox>
#include <QMenu>

#include <nlohmann/json.hpp>
#include <exception>
#include <qdebug.h>
#include <qlogging.h>
#include <qobject.h>
#include <stdexcept>

#include "task_dialog.hpp"
#include "task_loader.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow) {
    mUi->setupUi(this);
    
    connect(mUi->treeWidget, &QTreeWidget::itemChanged, this, &MainWindow::onTreeWidgetItemChanged);
    connect(mUi->treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTreeWidgetItemDoubleClicked);
    connect(mUi->treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onTreeWidgetCustomContextMenu);

    loadTasks();
    rebuildTree();
}

MainWindow::~MainWindow() {
    delete mUi;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    saveTasks();
}

void MainWindow::onTreeWidgetItemChanged(QTreeWidgetItem* item, int column) {
    int idx = item->data(0, Qt::UserRole).toInt();
    ItemType itemType = (ItemType)item->data(0, Qt::UserRole + 1).toInt();
    if (itemType == ItemType::Title) {
        bool completed = (item->checkState(0) == Qt::Checked);
        mTasks[idx].setCompleted(completed);
    }
}

void MainWindow::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column) {
    int idx = item->data(0, Qt::UserRole).toInt();
    ItemType itemType = (ItemType)item->data(0, Qt::UserRole + 1).toInt();
    if (itemType != ItemType::Title) return;

    TaskDialog dialog(mTasks[idx], this);
    if (dialog.exec() == QDialog::Accepted) {
        mTasks[idx] = dialog.getTask();
        item->setText(0, "Заголовок: " + mTasks[idx].getTitle());
        item->child(1)->setText(0, "Дата редактирования: " + mTasks[idx].getEditedDate().toString("dd.MM.yyyy HH:mm:ss"));
        item->child(2)->setText(0, "Описание:\n" + mTasks[idx].getDescription() + "\n");
    }
}

void MainWindow::onTreeWidgetCustomContextMenu(const QPoint& point) {
    QTreeWidgetItem* item = mUi->treeWidget->itemAt(point);
    bool isClickOnTitle = item && ((ItemType)item->data(0, Qt::UserRole + 1).toInt() == ItemType::Title);

    QMenu menu(this);
    menu.addAction("Добавить новую задачу", [this]{
        TaskDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            mTasks.append(dialog.getTask());
            addTask(mTasks.last(), mTasks.size() - 1);
        }
    });

    if (isClickOnTitle) {
        menu.addAction("Редактировать задачу", [this, item]{
            onTreeWidgetItemDoubleClicked(item, 0);
        });
        menu.addAction("Удалить задачу", [this, item]{
            int idx = item->data(0, Qt::UserRole).toInt();
            mTasks.removeAt(idx);
            rebuildTree();
        });
    }

    menu.exec(mUi->treeWidget->viewport()->mapToGlobal(point));
}

QString MainWindow::getAnyPath(const std::string& jsonKey) const {
    QFile file("settings.json"); // hardcode
    if (!file.exists())
        throw std::invalid_argument("File does not exist. Bad path: " + std::string("settings.json"));
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Couldn't open the file for read. Path: " + std::string("settings.json"));

    auto fileData = file.readAll();
    file.close();
    nlohmann::json json = nlohmann::json::parse(fileData.toStdString());
    if (!json.contains(jsonKey))
        throw std::runtime_error("JSON dont have " + jsonKey + " key. Path: " + std::string("settings.json"));

    return QString::fromStdString(json[jsonKey].get<std::string>());
}

void MainWindow::loadTasks() {
    try {
        QString pathToLoad = getAnyPath("path_to_load");
        mTasks = TaskLoader::load(pathToLoad);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Loading error", e.what());
    }
}

void MainWindow::saveTasks() {
    try {
        QString pathToSave = getAnyPath("path_to_save");
        TaskLoader::save(pathToSave, mTasks);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Saving error", e.what());
    }
}

void MainWindow::addTask(const Task& task, int idx) {
    auto* title = new QTreeWidgetItem(mUi->treeWidget);
    title->setText(0, "Заголовок: " + task.getTitle());
    title->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    title->setCheckState(0, task.isCompleted() ? Qt::Checked : Qt::Unchecked);
    title->setData(0, Qt::UserRole, idx);
    title->setData(0, Qt::UserRole + 1, (int)ItemType::Title);

    auto* createdDate = new QTreeWidgetItem(title);
    createdDate->setText(0, "Дата создания: " + task.getCreatedDate().toString("dd.MM.yyyy HH:mm:ss"));
    createdDate->setFlags(Qt::ItemIsEnabled);
    createdDate->setData(0, Qt::UserRole, idx);
    createdDate->setData(0, Qt::UserRole + 1, (int)ItemType::CreatedDate);

    auto* editedDate = new QTreeWidgetItem(title);
    editedDate->setText(0, "Дата редактирования: " + task.getEditedDate().toString("dd.MM.yyyy HH:mm:ss"));
    editedDate->setFlags(Qt::ItemIsEnabled);
    editedDate->setData(0, Qt::UserRole, idx);
    editedDate->setData(0, Qt::UserRole + 1, (int)ItemType::EditedDate);

    auto* description = new QTreeWidgetItem(title);
    description->setText(0, "Описание:\n" + task.getDescription() + "\n");
    description->setFlags(Qt::ItemIsEnabled);
    description->setData(0, Qt::UserRole, idx);
    description->setData(0, Qt::UserRole + 1, (int)ItemType::Description);
}

void MainWindow::rebuildTree() {
    mUi->treeWidget->clear();
    QSignalBlocker blocker(mUi->treeWidget);
    for (auto i = 0; i < mTasks.size(); i++) {
        addTask(mTasks[i], i);
    }
}
