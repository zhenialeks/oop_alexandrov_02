#pragma once

#include <cstdint>
#include <iterator>
#include <iostream>

namespace cont_exceptions{
class ContainerException: public std::exception{};
class PopEmptyException: public ContainerException {};
class GetFirstOfEmptyContainer: public ContainerException {};
}


template <typename T>
class Container {
public:
    struct elem{
         T & obj;
         elem * prev;
         elem * next;
    public:
        elem(T & obj, elem * prev, elem * next):obj(obj), prev(prev), next(next){}
        ~elem(){delete &obj;}
    };

    static elem *END;
private:
    elem *first;
    elem *last;
    uint64_t size;

private:

    class Iter{
    private:
        elem *cur_elem;
        explicit Iter(Container & container): cur_elem(container.first){}

    public:

        explicit Iter(elem * node): cur_elem(node){}

        T& operator*() const {
            return cur_elem->obj;
        }
        T& operator*() {
            return cur_elem->obj;
        }
        Iter& operator++(){
            if (cur_elem != nullptr)
                cur_elem = cur_elem->next;
            return *this;
        }
        const Iter operator++(int){
            Iter tmp = *this;
            if (tmp.cur_elem != nullptr)
                tmp.cur_elem = tmp.cur_elem->next;
            return tmp;
        }
        Iter& operator--(){
            if (cur_elem != nullptr)
                cur_elem = cur_elem->prev;
            return *this;
        }
        const Iter operator--(int){
            Iter tmp = *this;
            if (tmp.cur_elem != nullptr)
                tmp.cur_elem = tmp.cur_elem->prev;
            return tmp;
        }
        bool operator==(const Iter & other){
            return cur_elem == other.cur_elem;
        }
        bool operator!=(const Iter & other){
            return !(this->operator==(other));
        }
    };
public:
    Container():first(nullptr), last(nullptr), size(0){}

    void push_front(T const & obj){
        elem * new_elem = new elem(obj, nullptr, nullptr);

        if (size == 0) {
            first = last = new_elem;
            first->next = END;
        }else {
            elem * prev_first = first;
            first = new_elem;
            first->next = prev_first;
            prev_first->prev = first;
        }

        ++size;

    }

    void push_back(T & obj){
        elem * new_elem = new elem(obj, nullptr, nullptr);

       if (size == 0){
            first = last = new_elem;
            last->next = END;
       } else {
            last->next = new_elem;
            new_elem->prev = last;
            last = new_elem;
            last->next = END;
       }
        ++size;

    }

    void pop_first(){
        if (size == 0)
            throw cont_exceptions::PopEmptyException();
        if (size == 1){
            delete first;
            first = last = nullptr;
        } else {
            elem *next_el = first->next;
            delete first;
            first = next_el;
            first->prev = nullptr;

        }
        --size;
    }

    T & get_first(){
        if (first != nullptr)
            return first->obj;
        else
            throw cont_exceptions::GetFirstOfEmptyContainer();
    }

    void pop_last(){
        if (size == 0)
            throw cont_exceptions::PopEmptyException();

        if (size == 1){
            delete last;
            first = last = nullptr;
        } else {
            elem * last_prev = last->prev;
            delete last;
            last = last_prev;
            last->next = END;
        }
        --size;
    }

    T const & get_last(){
        if (last != nullptr)
            return last->obj;
        else
            throw cont_exceptions::GetFirstOfEmptyContainer();
    }

    Iter begin(){
        if (first == nullptr)
            return Iter(END);
        else
            return Iter(this->first);
    }

    static Iter end(){
       return Iter(END);
    }

    Iter rbegin(){
        if (first == nullptr)
            return Iter(END);
        else
            return Iter(this->last);
    }

    uint64_t get_size() const {
        return size;
    }
    bool is_empty() {return size == 0;}

    void swap(Container<T> & other){
        elem * tmp_first = this->first;
        elem * tmp_last = this->last;
        uint64_t tmp_size = this->size;

        first = other.first;
        last = other.last;
        size = other.size;

        other.first = tmp_first;
        other.last = tmp_last;
        other.size = tmp_size;
    }

    void reverse(){

        if (size == 1){
            return;
        }

        elem * cur_elem = first;
        while (cur_elem != last){
            cur_elem->prev = cur_elem->next;
            cur_elem->next = cur_elem->prev;
            cur_elem = cur_elem->prev;
        }

        first->next = END;
        last->next = last->prev;
        last->prev = nullptr;

        auto * s = first;
        first = last;
        last = s;

    }

    void clear(){
        if (size == 0)
            return;
        if (size == 1){
            delete first;
        } else {
            elem *tmp = first;
            while (tmp != END) {
                elem *new_tmp = tmp->next;
                delete tmp;
                tmp = new_tmp;
            }
        }
        first = last = nullptr;
        size = 0;
    }

    ~Container(){
        clear();
    }


};
//template things
template<typename T>
typename Container<T>::elem  * Container<T>::END = static_cast<elem *>(malloc(sizeof(Container<T>::elem)*1));

