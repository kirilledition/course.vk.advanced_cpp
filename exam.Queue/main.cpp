// Напишите аналог класса std::queue:

#include <iostream>

template <class T>
class node {
 public:
  node(T& value) : value_(value), next(nullptr) {}
  T value_;
  node* next;
};

template <class T>
class queue {
 public:
  queue() : front_(nullptr), back_(nullptr) {}
  void push(const T& elem) {
    std::cout << elem.i << " " << elem.j << std::endl;
    if (front_ == nullptr) {
      front_ = new node(elem);
      back_ = front_;
    } else {
      back_->next = new node(elem);
      back_ = back_->next;
    }
  };
  void pop() {
    if (front_ == nullptr) {
      return;
    }
    node<T>* tmp = front_;
    front_ = front_->next;
    delete tmp;
  };
  size_t size() const {
    size_t size = 0;
    node<T>* tmp = front_;
    while (tmp != nullptr) {
      size++;
      tmp = tmp->next;
    }
    return size;
  };
  template <class... Args>
  void emplace(Args&&... args) {
    T elem(args);
    push(elem)
  };

 private:
  node<T>* front_;
  node<T>* back_;
};

struct A {
  int i = 0;
  int j = 1;
};

int main() {
    queue<A> q;
    q.emplace(1, 2);
    return 0;
};