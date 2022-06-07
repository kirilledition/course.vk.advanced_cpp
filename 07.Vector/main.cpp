#include <iostream>
#include <iterator>

template <class T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = size_t;

  pointer allocate(size_type count) {
    return static_cast<pointer>(operator new(count * sizeof(T)));
  }

  void deallocate(pointer ptr) { operator delete(ptr); }
  template <typename... Args>
  void construct(pointer ptr, Args &&...args) {
    new (ptr) T(std::forward<Args>(args)...);
  }
  void destroy(pointer ptr) { ptr->~T(); }
  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }
};

template <class T>
class Iterator : public std::iterator_traits<T> {
  T *ptr_;

 public:
  Iterator(T *ptr) : ptr_(ptr) {}

  bool operator==(const Iterator<T> &other) const { return ptr_ == other.ptr_; }
  bool operator!=(const Iterator<T> &other) const { return !(*this == other); }
  bool operator<(const Iterator<T> &other) const { return (ptr_ < other.ptr_); }
  bool operator>(const Iterator<T> &other) const { return !(*this < other); }
  bool operator<=(const Iterator<T> &other) const {
    return (ptr_ <= other.ptr);
  }
  bool operator>=(const Iterator<T> &other) const { return !(*this <= other); }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  T &operator[](size_t i) const { return *(ptr_ + i); }
  void swap(Iterator &it_1, Iterator &it_2) {
    T *tmp = it_1.ptr_;
    it_1.ptr_ = it_2.ptr_;
    it_2.ptr = tmp;
  }
  Iterator &operator+(size_t i) {
    ptr_ += i;
    return *this;
  }
  Iterator &operator-(size_t i) {
    ptr_ -= i;
    return *this;
  }
  Iterator &operator-(const Iterator &it) {
    ptr_ -= it.ptr_;
    return *this;
  }
  Iterator &operator++() {
    ++ptr_;
    return *this;
  }
  Iterator &operator--() {
    --ptr_;
    return *this;
  }
  Iterator &operator+=(size_t i) {
    ptr_ += i;
    return *this;
  }

  template <class Y>
  friend Iterator<Y> &operator+(size_t i, Iterator<Y> &it);
  Iterator &operator-=(size_t i) {
    ptr_ -= i;
    return *this;
  }
};

template <class T>
Iterator<T> &operator+(size_t i, Iterator<T> &it) {
  it.ptr_ += i;
  return it;
}

template <class T, class Alloc = Allocator<T>>
class vector {
  using size_type = size_t;
  using value_type = T;
  using vt_pointer = T *;
  using reference = T &;
  using const_reference = const T &;
  using iterator = Iterator<T>;
  using const_iterator = const Iterator<T>;
  using reverse_iterator = std::reverse_iterator<Iterator<T>>;

  size_type size_;
  value_type *pointer;
  Alloc alloc;

 public:
  vector(size_type count) : size_(count), pointer(alloc.allocate(count)) {}
  vector(size_type count, const value_type &defaultValue)
      : size_(count), pointer(alloc.allocate(count)) {
    for (size_type i = 0; i < count; i++)
      alloc.construct(pointer + i, defaultValue);
  }
  vector(std::initializer_list<value_type> init) : size_(init.size()) {
    size_type i = 0;
    auto current = init.begin();
    const auto end = init.end();
    pointer = alloc.allocate(end - current);
    while (current != end) {
      alloc.construct(pointer + i, *current);
      i++;
      current++;
    }
  }
  ~vector() {
    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);
    alloc.deallocate(pointer);
  }

  iterator begin() { return iterator(pointer); }
  iterator end() { return iterator(pointer + size_); }
  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() const { return reverse_iterator(begin()); }

  void resize(size_type newSize) {
    vt_pointer new_pointer = alloc.allocate(newSize);
    size_type min_size = size_;

    if (newSize < min_size) min_size = newSize;

    for (size_type i = 0; i < min_size; i++)
      alloc.construct(new_pointer + i, pointer[i]);

    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);

    alloc.deallocate(pointer);
    pointer = new_pointer;
    size_ = newSize;
  }

  void resize(size_type newSize, const value_type &defaultValue) {
    size_type old_size = size_;
    resize(newSize);
    if (newSize > old_size) {
      for (size_type i = old_size; i < newSize; i++)
        alloc.construct(pointer + i, defaultValue);
    }
  }

  void push_back(const value_type &value) {
    if (size_ == 0) {
      pointer = alloc.allocate(1);
      pointer[0] = value;
    } else
      resize(size_ + 1, value);
  }

  size_type size() const { return size_; }

  bool empty() const {
    if (size_ == 0)
      return true;
    else
      return false;
  }

  void pop_back() {
    if (size_ == 0) throw -1;

    resize(size_ - 1);
  }

  const_reference operator[](size_type pos) const { return pointer[pos]; }
  reference operator[](size_type pos) { return pointer[pos]; }

  void reserve(size_type count) {
    if (count <= size_) return;

    vt_pointer new_pointer = alloc.allocate(count);

    for (size_type i = 0; i < size_; i++)
      alloc.construct(new_pointer + i, pointer[i]);

    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);

    alloc.deallocate(pointer);
    pointer = new_pointer;
    size_ = count;
  }

  void erase(const_iterator where) {
    vt_pointer new_pointer = alloc.allocate(size_ - 1);
    size_type shift = 0;
    for (size_type i = 0; i < size_; i++) {
      if (&pointer[i] == &(*where)) {
        shift = 1;
        continue;
      }
      alloc.construct(new_pointer + i - shift, pointer[i]);
    }

    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);

    alloc.deallocate(pointer);
    pointer = new_pointer;
    size_ = size_ - 1;
  }

  iterator erase(const_iterator from, const_iterator to) {
    vt_pointer new_pointer = alloc.allocate(size_ - (&(*to) - &(*from)));
    size_type shift = 0;

    for (size_type i = 0; i < size_; i++) {
      if ((&pointer[i] >= &(*from)) && (&pointer[i] < &(*to))) {
        shift += 1;
        continue;
      }
      alloc.construct(new_pointer + i - shift, pointer[i]);
    }

    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);

    alloc.deallocate(pointer);
    pointer = new_pointer;
    size_ = size_ - (&(*to) - &(*from));
  }

  void clear() {
    for (size_type i = 0; i < size_; i++) alloc.destroy(pointer + i);

    size_ = 0;
  }
};