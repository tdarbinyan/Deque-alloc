#include <algorithm>
#include <iostream>
#include <vector>

template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 private:
  using alloc_tr = std::allocator_traits<Allocator>;
  using block_alloc = typename alloc_tr::template rebind_alloc<T*>;
  using block_alloc_tr = std::allocator_traits<block_alloc>;

  const size_t kStartingSize = 2;  // 2
  const size_t kArraySize = 10;    // 10

  Allocator alloc_;

  size_t start_ = 0;
  size_t finish_ = 0;
  size_t starting_block_ = 0;
  size_t ending_block_ = 0;
  size_t size_ = 0;

  std::vector<T*, block_alloc> blocks_;

  // alloc/dealloc
  T* alloc_block() { return alloc_tr::allocate(alloc_, kArraySize); }

  void dealloc_block(T* ptr) { alloc_tr::deallocate(alloc_, ptr, kArraySize); }

  void clear_mem() {
    while (!empty()) {
      pop_front();
    }

    for (size_t i = 0; i < blocks_.size(); ++i) {
      dealloc_block(blocks_[i]);
    }

    blocks_.clear();
  }

  void ifdefault_construct(T* ptr, size_t& count) try {
    for (size_t i = 0; i < kArraySize; ++i) {
      if (count == 0) {
        break;
      }

      --count;

      try {
        alloc_tr::construct(alloc_, ptr + i);
      } catch (...) {
        clear_mem();
        throw;
      }

      ++size_;
    }
  } catch (...) {
    throw;
  }

 public:
  // Constructors
  explicit Deque(const Allocator& alloc = Allocator())
      : alloc_(alloc), blocks_(block_alloc(alloc)) {}

  Deque(const Deque& copy) try
      : alloc_(alloc_tr::select_on_container_copy_construction(copy.alloc_)),
        blocks_(copy.blocks_.size(), nullptr, block_alloc(copy.alloc_)),
        start_(copy.start_),
        starting_block_(copy.starting_block_),
        finish_(copy.finish_),
        ending_block_(copy.ending_block_) {
    for (size_t i = 0; i < blocks_.size(); ++i) {
      blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);

      for (size_t j = 0; j < kArraySize; ++j) {
        bool right_from_start =
            (i == starting_block_ && j >= start_ || i > starting_block_);
        bool left_from_finish =
            (i == ending_block_ && j < finish_ || i < ending_block_);

        if (right_from_start && left_from_finish) {
          try {
            alloc_tr::construct(alloc_, blocks_[i] + j, copy.blocks_[i][j]);
            ++size_;
          } catch (...) {
            clear_mem();
            throw;
          }
        }
      }
    }
  } catch (...) {
    throw;
  }

  Deque(size_t count, const Allocator& alloc = Allocator()) try
      : alloc_(alloc),
        blocks_(count / kArraySize + 1, nullptr, block_alloc(alloc_)) {
    finish_ = count % kArraySize;
    ending_block_ = count / kArraySize;
    for (size_t i = 0; i < blocks_.size(); ++i) {
      blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
      ifdefault_construct(blocks_[i], count);
    }
  } catch (...) {
    throw;
  }

  Deque(size_t count, T val, Allocator alloc = Allocator()) try
      : blocks_(count / kArraySize + 1, nullptr, block_alloc(alloc)) {
    alloc_ = alloc;
    finish_ = count % kArraySize;
    ending_block_ = count / kArraySize;

    try {
      for (size_t i = 0; i < blocks_.size(); ++i) {
        blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);

        for (size_t j = 0; j < kArraySize; ++j) {
          if (count == 0) {
            break;
          }

          try {
            alloc_tr::construct(alloc_, blocks_[i] + j, val);
            --count;
          } catch (...) {
            clear_mem();
            throw;
          }

          ++size_;
        }
      }
    } catch (...) {
      throw 1;
    }
  } catch (...) {
    throw 1;
  }

  Deque(Deque&& copy)
      : alloc_(std::move(copy.alloc_)),
        blocks_(std::move(copy.blocks_), block_alloc(copy.alloc_)) {
    size_ = copy.size_;
    copy.size_ = 0;

    start_ = copy.start_;
    copy.start_ = 0;

    starting_block_ = copy.starting_block_;
    copy.starting_block_ = 0;

    finish_ = copy.finish_;
    copy.finish_ = 0;

    ending_block_ = copy.ending_block_;
    copy.ending_block_ = 0;

    copy.blocks_.clear();
  }

  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : blocks_(block_alloc(alloc)) {
    try {
      alloc_ = alloc;

      for (auto iter = init.begin(); iter != init.end(); ++iter) {
        push_back(*(iter));
      }
    } catch (...) {
      throw;
    }
  }

  // Destructor
  ~Deque() {
    while (!empty()) {
      pop_front();
    }

    for (size_t i = 0; i < blocks_.size(); ++i) {
      dealloc_block(blocks_[i]);
    }

    blocks_.clear();
  }

  // Operators
  Deque& operator=(const Deque& copy) {
    if (alloc_tr::propagate_on_container_copy_assignment::value) {
      alloc_ = copy.alloc_;
    }

    Deque temp(copy);
    std::swap(size_, temp.size_);
    std::swap(blocks_, temp.blocks_);
    std::swap(start_, temp.start_);
    std::swap(starting_block_, temp.starting_block_);
    std::swap(finish_, temp.finish_);
    std::swap(ending_block_, temp.ending_block_);

    return *this;
  }

  Deque& operator=(Deque&& copy) {
    if (this == &copy) {
      return *this;
    }

    blocks_ = std::move(copy.blocks_);
    copy.blocks_.clear();

    starting_block_ = copy.starting_block_;
    copy.starting_block_ = 0;

    ending_block_ = copy.ending_block_;
    copy.ending_block_ = 0;

    start_ = copy.start_;
    copy.start_ = 0;

    finish_ = copy.finish_;
    copy.finish_ = 0;

    size_ = copy.size_;
    copy.size_ = 0;

    if (alloc_tr::propagate_on_container_copy_assignment::value) {
      alloc_ = std::move(copy.alloc_);
    }

    return *this;
  }

  T& operator[](const size_t& index) {
    size_t actual_index = index + kArraySize * starting_block_ + start_;
    return blocks_[actual_index / kArraySize][actual_index % kArraySize];
  }

  const T& operator[](const size_t& index) const {
    size_t actual_index = index + kArraySize * starting_block_ + start_;
    return blocks_[actual_index / kArraySize][actual_index % kArraySize];
  }

  // Getters
  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  Allocator get_allocator() { return alloc_; }

  // Methods
  void push_back(const T& val) {
    if (ending_block_ == blocks_.size()) {
      if (empty()) {
        blocks_.resize(1, alloc_block());
      } else {
        size_t temp = blocks_.size();
        blocks_.resize(3 * temp);
        starting_block_ += temp;
        ending_block_ += temp;

        for (size_t i = 0; i < temp; ++i) {
          blocks_[i + temp] = blocks_[i];

          blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
          blocks_[i + 2 * temp] = alloc_tr::allocate(alloc_, kArraySize);
        }
      }
    }

    try {
      alloc_tr::construct(alloc_, blocks_[ending_block_] + finish_, val);

      ++size_;
      finish_ = (start_ + size_) % kArraySize;
      ending_block_ = starting_block_ + (start_ + size_) / kArraySize;

    } catch (...) {
      throw;
    }
  }

  void push_back(T&& val) {
    if (ending_block_ == blocks_.size()) {
      if (empty()) {
        blocks_.resize(1, alloc_block());
      } else {
        size_t temp = blocks_.size();
        blocks_.resize(3 * temp);
        starting_block_ += temp;
        ending_block_ += temp;

        for (size_t i = 0; i < temp; ++i) {
          blocks_[i + temp] = blocks_[i];

          blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
          blocks_[i + 2 * temp] = alloc_tr::allocate(alloc_, kArraySize);
        }
      }
    }

    try {
      alloc_tr::construct(alloc_, blocks_[ending_block_] + finish_,
                          std::move(val));

      ++size_;
      finish_ = (start_ + size_) % kArraySize;
      ending_block_ = starting_block_ + (start_ + size_) / kArraySize;

    } catch (...) {
      throw;
    }
  }

  void push_front(const T& val) {
    if (starting_block_ == 0 && start_ == 0) {
      if (empty()) {
        blocks_.resize(1, alloc_block());
      } else {
        size_t temp = blocks_.size();
        blocks_.resize(3 * temp);
        starting_block_ += temp;
        ending_block_ += temp;

        for (size_t i = 0; i < temp; ++i) {
          blocks_[i + temp] = blocks_[i];

          blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
          blocks_[i + 2 * temp] = alloc_tr::allocate(alloc_, kArraySize);
        }
      }
    }

    if (start_ > 0) {
      --start_;
    } else {
      start_ = kArraySize - 1;
      --starting_block_;
    }

    try {
      alloc_tr::construct(alloc_, blocks_[starting_block_] + start_, val);
      ++size_;
    } catch (...) {
      clear_mem();
      throw;
    }
  }

  void push_front(T&& val) {
    if (starting_block_ == 0 && start_ == 0) {
      if (empty()) {
        blocks_.resize(1, alloc_block());
      } else {
        size_t temp = blocks_.size();
        blocks_.resize(3 * temp);
        starting_block_ += temp;
        ending_block_ += temp;

        for (size_t i = 0; i < temp; ++i) {
          blocks_[i + temp] = blocks_[i];

          blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
          blocks_[i + 2 * temp] = alloc_tr::allocate(alloc_, kArraySize);
        }
      }
    }

    if (start_ > 0) {
      --start_;
    } else {
      start_ = kArraySize - 1;
      --starting_block_;
    }

    try {
      alloc_tr::construct(alloc_, blocks_[starting_block_] + start_,
                          std::move(val));
    } catch (...) {
      clear_mem();
      throw;
    }

    ++size_;
  }

  void pop_back() {
    if (!empty()) {
      if (finish_ > 0) {
        --finish_;
      } else {
        finish_ = kArraySize - 1;
        --ending_block_;
      }

      alloc_tr::destroy(alloc_, blocks_[ending_block_] + finish_);

      --size_;
    }
  }

  void pop_front() {
    if (!empty()) {
      alloc_tr::destroy(alloc_, blocks_[starting_block_] + start_);

      if (start_ < kArraySize - 1) {
        ++start_;
      } else {
        start_ = 0;
        ++starting_block_;
      }

      --size_;
    }
  }

  T& at(size_t index) {
    if (index >= get_elem_count()) {
      throw std::out_of_range("out_of_range");
    }
    size_t actual_index = index + kArraySize * starting_block_ + start_;
    return blocks_[actual_index / kArraySize][actual_index % kArraySize];
  }

  const T& at(size_t index) const {
    if (index >= get_elem_count()) {
      throw std::out_of_range("out of range");
    }
    size_t actual_index = index + kArraySize * starting_block_ + start_;
    return blocks_[actual_index / kArraySize][actual_index % kArraySize];
  }

  // Utility
  size_t get_elem_count() {
    return ending_block_ * kArraySize + finish_ - starting_block_ * kArraySize -
           start_;
  }

  void cout_vector() {
    if (get_elem_count() != size_) {
      std::cout << "SIZES DO NOT MATCH" << std::endl;
      std::cout << ending_block_ * kArraySize + finish_ -
                       starting_block_ * kArraySize - start_
                << "!=" << size_ << std::endl;
    }
    for (size_t i = 0; i < blocks_.size(); ++i) {
      std::cout << '[' << i << ']' << " : ";
      if (blocks_[i] != nullptr) {
        for (size_t j = 0; j < kArraySize; ++j) {
          if (blocks_[i][j] >= 0) {
            std::cout << blocks_[i][j] << " ";
          } else {
            std::cout << '#' << " ";
          }
        }
        std::cout << std::endl;
      }
    }
    std::cout << "----------------------------------" << std::endl;
  }

  // Iterators
  template <bool IsConst = false>
  class Iterator
      : std::iterator<std::random_access_iterator_tag,
                      typename std::conditional<IsConst, const T, T>::type> {
   private:
    Deque<T, Allocator>* ptr_;
    size_t index_ = 0;

   public:
    using value_type = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::value_type;
    using difference_type = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::difference_type;
    using iterator_category =
        typename std::iterator<std::random_access_iterator_tag,
                               typename std::conditional<IsConst, const T, T>::
                                   type>::iterator_category;
    using pointer = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::pointer;
    using reference = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::reference;

    // Constructors
    Iterator() = default;

    Iterator(Deque<T, Allocator>* deque, size_t index)
        : ptr_(deque), index_(index) {}

    Iterator(const Iterator<IsConst>& copy)
        : ptr_(copy.ptr_), index_(copy.index_) {}

    // Utility
    Deque<T>* get_deque() { return ptr_; }

    size_t get_index() { return index_; }

    // Operators
    void operator=(const Iterator& copy) {
      index_ = copy.index_;
      ptr_ = copy.ptr_;
    }

    Iterator<IsConst>& operator+=(difference_type jump) { index_ += jump; }

    Iterator<IsConst>& operator-=(difference_type jump) { index_ -= jump; }

    Iterator<IsConst> operator+(difference_type jump) const {
      Iterator<IsConst> temp_iter(*this);
      temp_iter.index_ += jump;
      return temp_iter;
    }

    Iterator<IsConst> operator-(difference_type jump) const {
      Iterator<IsConst> temp_iter(*this);
      temp_iter.index_ -= jump;
      return temp_iter;
    }

    difference_type operator-(const Iterator<IsConst>& iter) const {
      return index_ - iter.index_;
    }

    Iterator<IsConst>& operator++() {
      ++index_;
      return *this;
    }

    Iterator<IsConst>& operator--() {
      --index_;
      return *this;
    }

    Iterator<IsConst> operator++(int) {
      Iterator<IsConst> temp_iter(*this);
      ++index_;
      return temp_iter;
    }

    Iterator<IsConst> operator--(int) {
      Iterator<IsConst> temp_iter(*this);
      --index_;
      return temp_iter;
    }

    friend bool operator==(const Iterator<IsConst>& first,
                           const Iterator<IsConst>& second) {
      return (first.ptr_ == second.ptr_ && first.index_ == second.index_);
    }

    friend bool operator>(const Iterator<IsConst>& first,
                          const Iterator<IsConst>& second) {
      return (first.ptr_ == second.ptr_ && first.index_ > second.index_);
    }

    friend bool operator!=(const Iterator<IsConst>& first,
                           const Iterator<IsConst>& second) {
      return !(first == second);
    }

    friend bool operator<(const Iterator<IsConst>& first,
                          const Iterator<IsConst>& second) {
      return (first != second && !(first > second));
    }

    friend bool operator<=(const Iterator<IsConst>& first,
                           const Iterator<IsConst>& second) {
      return !(first > second);
    }

    friend bool operator>=(const Iterator<IsConst>& first,
                           const Iterator<IsConst>& second) {
      return !(first < second);
    }

    reference operator*() const { return (*ptr_)[index_]; }

    pointer operator->() const { return &((*ptr_)[index_]); }
  };

  // Methods to work with iterators
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<Iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<Iterator<false>>;

  iterator begin() { return iterator(this, 0); }

  iterator end() { return iterator(this, size_); }

  const_iterator cbegin() { return const_iterator(this, 0); }

  const_iterator cend() { return const_iterator(this, size_); }

  reverse_iterator rbegin() { return reverse_iterator(iterator(this, size_)); }

  reverse_iterator rend() { return reverse_iterator(iterator(this, 0)); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(iterator(this, size_));
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(iterator(this, 0));
  }

  void insert(iterator iter, const T& value) {
    push_back(value);
    for (size_t i = size() - 1; i > iter.get_index(); --i) {
      std::swap(this->at(i), this->at(i - 1));
    }
  }

  void erase(iterator iter) {
    for (size_t i = iter.get_index(); i < size() - 1; ++i) {
      std::swap(this->at(i), this->at(i + 1));
    }

    if (!empty()) {
      pop_back();
    }
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    if (ending_block_ == blocks_.size()) {
      if (empty()) {
        blocks_.resize(1, alloc_block());
      } else {
        size_t temp = blocks_.size();
        blocks_.resize(3 * temp);
        starting_block_ += temp;
        ending_block_ += temp;

        for (size_t i = 0; i < temp; ++i) {
          blocks_[i + temp] = blocks_[i];

          blocks_[i] = alloc_tr::allocate(alloc_, kArraySize);
          blocks_[i + 2 * temp] = alloc_tr::allocate(alloc_, kArraySize);
        }
      }
    }

    alloc_tr::construct(alloc_, blocks_[ending_block_] + finish_,
                        std::forward<Args>(args)...);

    ++size_;
    finish_ = (start_ + size_) % kArraySize;
    ending_block_ = starting_block_ + (start_ + size_) / kArraySize;
  }

  template <typename... Args>
  void emplace_front(Args&&... args) {
    if (start_ == 0) {
      blocks_.insert(blocks_.begin(), alloc_block());
      --starting_block_;
      start_ = kArraySize;
    }

    alloc_tr::construct(alloc_, blocks_[starting_block_] + start_,
                        std::forward<Args>(args)...);

    ++size_;
    --start_;
  }

  template <typename... Args>
  void emplace(iterator iter, Args&&... args) {
    if (iter == begin()) {
      emplace_front(std::forward<Args>(args)...);
      return;
    }

    if (iter == end()) {
      emplace_back(std::forward<Args>(args)...);
      return;
    }

    size_t index = iter - begin();

    blocks_.emplace_back(std::forward<Args>(args)...);

    for (size_t i = index; i < size() - 1; ++i) {
      std::swap((*this)[i], (*this)[i + 1]);
    }
  }
};
