#include "task.hpp"
#include "task_loader.hpp"

int main(int argc, char** argv) {
    auto tasksList = TaskLoader::load("tasks.json");

    return 0;
}
