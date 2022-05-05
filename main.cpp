#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <cassert>

#include "Container.h"
#include "Object.h"


uint64_t Object::g_counter = 0;

void fill_task_container(Container<Task > &container) {
    enum task_types {
        CALC_TASK = 0,
        PUSH_TASK = 1,
        COUNT_TASKS = 2,
        COUNT_HAS_RES_TASKS = 3,
        CLEAN_TASK = 4,
        COUNT_OBJS = 5
    } task_type;

    //Randomizing things
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> task_distrib(0,5);
    std::uniform_int_distribution<int> sign_distrib(0,3);


    std::queue<Task *> cached_tasks;

    uint8_t i = 0;
    while (i < 17){
        task_type = task_types(task_distrib(rng));

        auto new_sign = CalcTask::op_type(sign_distrib(rng));

        switch (task_type) {
            case CALC_TASK:
                if (cached_tasks.size() < 2)
                    continue;
                {
                    auto s = new CalcTask((std::string)"name", new_sign, cached_tasks.front(),
                                             cached_tasks.back());
                    container.push_back(*s);
                }
                cached_tasks.pop();
                cached_tasks.pop();

                cached_tasks.push(&(*(container.rbegin())));
                break;

            case PUSH_TASK:
                {
                    auto s = new PushTask(container, new CountObjTask());
                    container.push_back(*s);
                }
                break;

            case COUNT_TASKS:
                {
                    auto s = new CountTask(container);
                    container.push_back(*s);
                }
                    cached_tasks.push(&(*(container.rbegin())));

                break;

            case COUNT_HAS_RES_TASKS:
                {
                    auto s = new CountHasResTask(container);
                    container.push_back(*s);
                }
                break;

            case CLEAN_TASK:
                {
                    auto s = new CleanTask(container);
                    container.push_back(*s);
                }
                break;

            case COUNT_OBJS:
                {
                    auto s = new CountObjTask();
                    container.push_back(*s);
                }
                    cached_tasks.push(&(*(container.rbegin())));
                break;

            default:
                assert(false);
        }

        if (cached_tasks.size() > 2)
            cached_tasks.pop();

        ++i;

    }
}


void print_container(Container<Task> & con){
    uint8_t i = 0;
    for (auto & el : con){
        std::cout << (int) i++ << ":" << el.toString() << std::endl;
    }
}

int main(){
    using namespace std;

    auto tasks = Container<Task>();
    fill_task_container(tasks);
    print_container(tasks);
    Container<uint64_t> results{};

    cout << "Total amount of Objects created: " << Object::getGCounter() << endl;

    try {
        for (auto & cur_task : tasks){
            cur_task.run();
            if (tasks.is_empty())
                break;
            if (cur_task.has_res()) {
                auto * tmp = new uint64_t(cur_task.getResult());
                results.push_back(*tmp);
            }
        }
    } catch (cont_exceptions::PopEmptyException & e){
        cout << e.what() << endl;
    } catch (runtime_error & e){
        cout << e.what() << endl;
    }


    cout << "results:" << endl;
    for (auto & el : results)
        cout << el << " ";
    cout << endl;


    tasks.clear();
 /*   for (auto  & el: created_task_ttl_list)
        delete el;*/
    results.clear();
    cout << "Remaining objects amount:" << Object::getGCounter() <<  endl;

    return 0;
}