#pragma once 
#include <memory>
template <typename T>
class SuperUniquePointer{
private:
    T* pointer_;
public:

    SuperUniquePointer(T* pointer) : pointer_(pointer) {}
    ~SuperUniquePointer() {delete pointer_;}

    SuperUniquePointer(const SuperUniquePointer&) = delete;
    SuperUniquePointer& operator=(const SuperUniquePointer&) = delete;

    template <class U, class = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    SuperUniquePointer(SuperUniquePointer<U>&& other) : pointer_(static_cast<T*>(other.get())) {
        // other.pointer_ = nullptr;
    }

    SuperUniquePointer(SuperUniquePointer&& other) : pointer_(other.pointer_) {
        other.pointer_ = nullptr;
    }

    SuperUniquePointer& operator=(SuperUniquePointer&& other) {
        if (this != &other) {
            delete pointer_;
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
            //std::make_unique
        }
        return *this;
    }

    T* get() const { return pointer_; }
    T& operator*() const { return *pointer_; }
    T* operator->() const { return pointer_; }

};

template <typename T, typename... Args>
auto makeUnique(Args&&... a) -> SuperUniquePointer<T>{
    return SuperUniquePointer<T>(new T(std::forward<Args>(a)...));
}

template <class T, class U, typename... Args>
auto makeUnique(std::in_place_type_t<U>, Args&&... a) -> SuperUniquePointer<T>{
    return SuperUniquePointer<T>(new U(std::forward<Args>(a)...));
}
