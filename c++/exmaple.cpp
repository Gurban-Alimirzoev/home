#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
// Перечислимый тип для статуса задачи
enum class TaskStatus
{
    NEW,         // новая
    IN_PROGRESS, // в разработке
    TESTING,     // на тестировании
    DONE         // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks
{
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo &GetPersonTasksInfo(const string &person) const
    {
        TasksInfo tasks_number = result_person[person];
        sort(tasks_number.begin(), tasks_number.end(), [](TasksInfo lhs, TasksInfo rhs)
             { return lhs.TaskStatus < rhs.TaskStatus; });
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string &person)
    {
        ++result_person[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
        const string &person, int task_count)
    {
        TasksInfo load_tasks, old_tasks;
        if ((result_person.at(person)).count(TaskStatus::NEW) &&
            result_person[person][TaskStatus::NEW] - task_count >= 0)
        {
            old_tasks[TaskStatus::NEW] = result_person[person][TaskStatus::NEW] - task_count;
            load_tasks[TaskStatus::IN_PROGRESS] = task_count;
            result_person.at(person)[TaskStatus::NEW] -= task_count;
            result_person.at(person)[TaskStatus::IN_PROGRESS] += task_count;
        }
        else
        {
            old_tasks[TaskStatus::NEW] = 0;
            load_tasks[TaskStatus::IN_PROGRESS] = result_person.at(person)[TaskStatus::NEW];
            task_count -= result_person.at(person)[TaskStatus::NEW];
            result_person.at(person)[TaskStatus::IN_PROGRESS] += result_person.at(person)[TaskStatus::NEW];
            result_person.at(person)[TaskStatus::NEW] = 0;

            if ((result_person.at(person)).count(TaskStatus::IN_PROGRESS) &&
                result_person[person][TaskStatus::IN_PROGRESS] - task_count >= 0)
            {
                old_tasks[TaskStatus::IN_PROGRESS] = result_person[person][TaskStatus::IN_PROGRESS] - task_count;
                load_tasks[TaskStatus::TESTING] = task_count;
                result_person.at(person)[TaskStatus::IN_PROGRESS] -= task_count;
                result_person.at(person)[TaskStatus::TESTING] += task_count;
            }
            else
            {
                old_tasks[TaskStatus::TESTING] = 0;
                load_tasks[TaskStatus::DONE] = result_person.at(person)[TaskStatus::TESTING];
                task_count -= result_person.at(person)[TaskStatus::TESTING];
                result_person.at(person)[TaskStatus::DONE] += result_person.at(person)[TaskStatus::TESTING];
                result_person.at(person)[TaskStatus::TESTING] = 0;
                if ((result_person.at(person)).count(TaskStatus::TESTING) &&
                    result_person[person][TaskStatus::TESTING] - task_count >= 0)
                {
                    old_tasks[TaskStatus::TESTING] = result_person[person][TaskStatus::TESTING] - task_count;
                    load_tasks[TaskStatus::DONE] = task_count;
                    result_person.at(person)[TaskStatus::TESTING] -= task_count;
                    result_person.at(person)[TaskStatus::DONE] += task_count;
                }
                else
                {
                    old_tasks[TaskStatus::TESTING] = 0;
                    load_tasks[TaskStatus::DONE] = result_person.at(person)[TaskStatus::TESTING];
                    task_count -= result_person.at(person)[TaskStatus::TESTING];
                    result_person.at(person)[TaskStatus::DONE] += result_person.at(person)[TaskStatus::TESTING];
                    result_person.at(person)[TaskStatus::TESTING] = 0;
                }
            }
        }

        return tuple(old_tasks, load_tasks);
    }

private:
    map<TaskStatus, int> tasks_number;
    map<const string &, map<TaskStatus, int>> result_person

        // Принимаем словарь по значению, чтобы иметь возможность
        // обращаться к отсутствующим ключам с помощью [] и получать 0,
        // не меняя при этом исходный словарь
        void
        PrintTasksInfo(TasksInfo tasks_info)
    {
        cout << tasks_info[TaskStatus::NEW] << " new tasks"s
             << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
             << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
             << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
    }

    int main()
    {
        TeamTasks tasks;
        tasks.AddNewTask("Ilia"s);
        for (int i = 0; i < 3; ++i)
        {
            tasks.AddNewTask("Ivan"s);
        }
        cout << "Ilia's tasks: "s;
        PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
        cout << "Ivan's tasks: "s;
        PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

        TasksInfo updated_tasks, untouched_tasks;

        tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan"s, 2);
        cout << "Updated Ivan's tasks: "s;
        PrintTasksInfo(updated_tasks);
        cout << "Untouched Ivan's tasks: "s;
        PrintTasksInfo(untouched_tasks);

        tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan"s, 2);
        cout << "Updated Ivan's tasks: "s;
        PrintTasksInfo(updated_tasks);
        cout << "Untouched Ivan's tasks: "s;
        PrintTasksInfo(untouched_tasks);
    }