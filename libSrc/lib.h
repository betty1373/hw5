#pragma once
#include <iostream>
#include <map>
#include <memory>

namespace libSrc 
{
    int version();
    int fact(int V);

    template<typename T,std::size_t lot>
    struct logging_allocator {
        using value_type = T;

        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        template<typename U>
        struct rebind {
            using other = logging_allocator<U,lot>;
        };

        logging_allocator() = default;
        ~logging_allocator() noexcept {
            std::cout << __PRETTY_FUNCTION__ <<" free memory "<<bufCnt <<std::endl;
              
            free(bufPtr);
            bufPtr = nullptr;
        };

        pointer allocate(std::size_t n) {    
            if (bufCnt + n > lot) {
                throw std::bad_alloc();
            }
     
            if (!bufPtr) {
                std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
                bufPtr = reinterpret_cast<pointer>(malloc(lot * sizeof(T)));
            }
            
            if (!bufPtr) {
                throw std::bad_alloc();
            }
            pointer pp = bufPtr + bufCnt;
            bufCnt += n;
            return pp;
        }
        void deallocate(pointer p, std::size_t n) {
            if (!p) {
            return;
            }
            bufCnt -= n;
            if (bufCnt != 0) {
            return;
            }            
            std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        }

        template<typename U, typename ...Args>
        void construct(U *p, Args &&...args) {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            new(p) U(std::forward<Args>(args)...);
        }

        void destroy(pointer p) {
            std::cout << __PRETTY_FUNCTION__ << std::endl;            
            p->~T();           
        }

        private:
        pointer bufPtr = nullptr;
        size_t  bufCnt  = 0;
    };

    // custom container
    template<typename T>
    struct Leaf
    {
        Leaf* next;
        T data;
        template<typename ...Args>
        Leaf(Args &&...args): next(nullptr), data(std::forward<Args>(args)...){}
    };
    template <typename T>
    struct ForwardListIterator
    {
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category=std::forward_iterator_tag;

        using _this = ForwardListIterator<T>;
        
        Leaf<T>* _leaf; 

        // constractors

        ForwardListIterator() : _leaf(){}
        ForwardListIterator(Leaf<T>* pLeaf) : _leaf(pLeaf) {}

        reference operator*() const {
            return _leaf->data;
        }

        pointer operator->() const {
            return &(_leaf->data);
        }

        _this& operator++(){
            if (_leaf==nullptr) {
                return *this;
            }
            else {
                _leaf = _leaf->next;
            }
            return *this;
        }
        bool operator ==(const _this& rhs) const{
            return _leaf==rhs._leaf;
        }
        bool operator !=(const _this& rhs) const{
            return _leaf!=rhs._leaf;
        }
    };
    template<typename T,typename _A=std::allocator<T>>
    class ForwardList {
    public:
        ForwardList(): head(nullptr), tail(nullptr),_alloc() {
            std::cout<<"FL ...ctor"<<std::endl;
        }

        ~ForwardList(){
            std::cout<<"FL ...dctor"<<std::endl;
            std::cout<<"FL head before destruct"<<head<<std::endl;
            auto current = head;
            while(current!=nullptr){
                auto remove = current;
                current = current->next;
                _alloc.destroy(remove);
                _alloc.deallocate(remove,1);
            }
        }
 
        template <typename ...Args>
        void push_back(Args &&...args) {
            Leaf<T> *new_node = _alloc.allocate(1);
            _alloc.construct(new_node, std::forward<Args>(args)...);

            if (head == nullptr) {
                head = new_node;
                tail = new_node;
            }
            else {
                tail->next = new_node;
                tail = new_node;
                tail->next = nullptr;
            }
            nums++;
        }

        ForwardListIterator<T> cbegin() const noexcept {
            return ForwardListIterator<T>(head);
        }

        ForwardListIterator<T> cend() noexcept {
            return ForwardListIterator<T>();
        }
//optional
        bool empty() const noexcept{
            return (nums==0);
        }
        size_t size() noexcept{
            return nums;
        }
    private:
        Leaf<T>* head;
        Leaf<T>* tail;

        size_t nums=0;

        using Allocator = typename _A::template rebind<Leaf<T>>::other;
        Allocator _alloc;
    };
}
