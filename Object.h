#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <sstream>
#include <memory>
#include "Container.h"


class Object {
private:
    static uint64_t g_counter;
public:
    virtual std::string toString() const = 0;
    Object(){
        ++g_counter;
    }
    ~Object(){
        --g_counter;
    }

    static uint64_t getGCounter() {
        return g_counter;
    }
};

class Task: public virtual Object{
public:
    virtual void run() = 0;
    virtual bool has_res() const = 0;
    virtual uint64_t getResult() const = 0;
    virtual ~Task() = default;
};

class Named:public virtual Object{
protected:
    std::string name;
public:
    explicit Named(std::string const & name){
        this->name = std::string(name);
    }
};

class ContainerTask:public virtual Task{
protected:
    Container<Task> & container;
public:
    explicit ContainerTask(Container<Task> & container) :container(container) {}
};

class NoResTask: public virtual Task{
public:
    bool has_res() const override{ return false; }
    uint64_t getResult() const override{ return 0;}
};

class HasResTask: public virtual Task{
protected:
    uint64_t result = 0;
public:
    bool has_res() const override{ return true; }
    uint64_t getResult() const override{
        return result;
    }

    HasResTask(){result = 0;}
    ~HasResTask() override = default;
};

class CountTask:public ContainerTask, public HasResTask{
public:
     explicit CountTask(Container<Task> &container): ContainerTask(container),HasResTask(){};

     std::string toString() const override {
        std::stringstream out_s;
        out_s << "<CountTask obj, it finds amount of Tasks inside the given container;";
        if (result == 0){
            out_s << "doesn't have any result yet>";
        } else
            out_s << "result: '" << result << "'>";

        return out_s.str();
    }
    void run() override{
        result = container.get_size();
    }

};


class CountObjTask:public HasResTask{
public:
    std::string toString() const override {
        std::stringstream out_s;
        out_s << "<CountObjTask obj, it finds amount of Object class's instances;";
        if (result == 0){
            out_s << "doesn't have any result yet>";
        } else
            out_s << "result: '" << result << "'>";

        return out_s.str();
    }

    void run() override {
        result = Object::getGCounter();
    }
    ~CountObjTask() override = default;
};

class PushTask:public ContainerTask, public NoResTask{
private:
    Task *task;
    bool has_been_run = false;
public:

    using NoResTask::has_res;
    PushTask(Container<Task> &container, Task *task): ContainerTask(container), task(task) {}

    std::string toString() const override{
        return "<PushTask obj, it pushes the task into the container>";
    }
    void run() override{
        container.push_back(*task);
        has_been_run = true;
    }
    ~PushTask() override{
        if (!has_been_run)
            delete task;
    };

};

class CleanTask:public ContainerTask, public NoResTask{
public:

    explicit CleanTask(Container<Task> & container): ContainerTask(container){}

    std::string toString() const override{
        return "<CleanTask obj, it empties the container>";
    }

    void run() override {
        container.clear();
    }
};


class CalcTask:public Named, public HasResTask{
public:
    typedef enum op{ ADD = 0, SUB = 1, MUL = 2, DIV = 3 } op_type;
private:
    op_type action;
    Task *task_1;
    Task *task_2;
public:

    CalcTask(std::string const &name, op_type action,
             Task *task_1, Task *task_2):Named(name), action(action), task_1(task_1), task_2(task_2){}

    void run() override{
        switch (action) {
            case ADD: result = task_1->getResult() + task_2->getResult(); break;
            case SUB: result = task_1->getResult() - task_2->getResult(); break;
            case MUL: result = task_1->getResult() * task_2->getResult(); break;
            case DIV:
                if (task_2->getResult() == 0) {
                    result = 0;
                    throw std::runtime_error("division by zero is not allowed");
                }
                result = task_1->getResult() / task_2->getResult(); break;
        }
    }
    std::string toString() const override{
        std::stringstream out_s;
        out_s << "<CalcTask obj, name:'" << name << "', it's going to perform ";
        switch (action){
            case ADD: out_s << " 'add' "; break;
            case SUB: out_s << " 'sub' "; break;
            case MUL: out_s << " 'mul' "; break;
            case DIV: out_s << " 'div' "; break;
        }
        out_s << "operation; ";
        if (result == 0){
            out_s << "doesn't have any result yet>";
        } else
            out_s << "result: '" << result << "'>";

        return out_s.str();
    }

    ~CalcTask() override = default;
};

class CountHasResTask:public ContainerTask, public HasResTask{
public:
    explicit CountHasResTask(Container<Task> & container): ContainerTask(container){}

    void run() override{
        int counter = 0;
        for (auto & s1 : container)
            if (s1.has_res())
                ++counter;
        result = counter;
    }

    std::string toString() const override{
        std::stringstream out_s;
        out_s << "<CountHasResTask obj, it's going to count amount of tasks inside of a container, which may have any result;";
        if (result == 0){
            out_s << "doesn't have any result yet>";
        } else
            out_s << "result: '" << result << "'>";

        return out_s.str();
    }
};


