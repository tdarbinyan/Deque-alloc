### Deque

#### Описание
`Deque` (двусторонняя очередь) представляет собой контейнер, который обеспечивает эффективное добавление и удаление элементов как в начале, так и в конце контейнера.

### Методы

1. **push_back(value):**
   - Добавляет элемент с заданным значением в конец деки.

```cpp
void push_back(const T& value);
```

2. **push_front(value):**
   - Добавляет элемент с заданным значением в начало деки.

```cpp
void push_front(const T& value);
```

3. **push_back(value):**
   - Добавляет элемент с перемещаемым значением в конец деки.

```cpp
void push_back(T&& value);
```

4. **push_front(value):**
   - Добавляет элемент с перемещаемым значением в начало деки.

```cpp
void push_front(T&& value);
```

5. **pop_back():**
   - Удаляет последний элемент из деки.

```cpp
void pop_back();
```

6. **pop_front():**
   - Удаляет первый элемент из деки.

```cpp
void pop_front();
```

7. **at(index):**
   - Возвращает ссылку на элемент по указанному индексу в деке. Выбрасывает исключение `std::out_of_range`, если индекс находится вне допустимого диапазона.

```cpp
T& at(size_t index);
```

8. **at(index) const:**
   - Возвращает константную ссылку на элемент по указанному индексу в деке. Выбрасывает исключение `std::out_of_range`, если индекс находится вне допустимого диапазона.

```cpp
const T& at(size_t index) const;
```

9. **insert(iter, value):**
   - Вставляет элемент со значением `value` перед элементом, на который указывает итератор `iter`.

```cpp
void insert(iterator iter, const T& value);
```

10. **erase(iter):**
    - Удаляет элемент, на который указывает итератор `iter`.

```cpp
void erase(iterator iter);
```

11. **size():**
    - Возвращает количество элементов в деке.

```cpp
size_t size() const;
```

12. **empty():**
    - Проверяет, пуста ли дека.

```cpp
bool empty() const;
```

13. **get_allocator():**
    - Возвращает текущий аллокатор, используемый декой.

```cpp
Allocator get_allocator();
```

14. **get_elem_count():**
    - Возвращает количество элементов в деке.

```cpp
size_t get_elem_count();
```

15. **cout_vector():**
    - Выводит содержимое деки в консоль в виде блоков.

```cpp
void cout_vector();
```

16. **emplace_back(args...):**
    - Конструирует элемент на месте в конце деки с использованием переданных аргументов.

```cpp
template <typename... Args>
void emplace_back(Args&&... args);
```

17. **emplace_front(args...):**
    - Конструирует элемент на месте в начале деки с использованием переданных аргументов.

```cpp
template <typename... Args>
void emplace_front(Args&&... args);
```

18. **emplace(iter, args...):**
    - Вставляет элемент перед итератором `iter`, конструируя его на месте с использованием переданных аргументов.

```cpp
template <typename... Args>
void emplace(iterator iter, Args&&... args);
```

### Конструкторы

1. **Deque(const Allocator& alloc = Allocator()):**
   - Конструктор по умолчанию. Принимает аллокатор в качестве параметра (по умолчанию используется стандартный аллокатор).

```cpp
explicit Deque(const Allocator& alloc = Allocator());
```

2. **Deque(const Deque& copy):**
   - Конструктор копирования.

```cpp
Deque(const Deque& copy);
```

3. **Deque(size_t count, const Allocator& alloc = Allocator()):**
   - Конструктор, создающий деку с `count` элементами. Все элементы инициализируются по умолчанию.

```cpp
Deque(size_t count, const Allocator& alloc = Allocator());
```

4. **Deque(size_t count, T value, Allocator alloc = Allocator()):**
   - Конструктор, создающий деку с `count` элементами, каждый из которых инициализируется значением `value`.

```cpp
Deque(size_t count, T value, Allocator alloc = Allocator());
```

5. **Deque(Deque&& copy):**
   - Конструктор перемещения.

```cpp
Deque(Deque&& copy);
```

6. **Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()):**
   - Конструктор, создающий деку с элементами из списка инициализации `init`.

```cpp
Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator());
```

### Деструктор

1. **~Deque():**
   - Деструктор, очищающий все выделенные ресурсы.

```cpp
~Deque();
```

### Операторы

1. **operator=(const Deque& copy):**
   - Оператор присваивания.

```cpp
Deque& operator=(const Deque& copy);
```

2. **operator=(Deque&& copy):**
   - Оператор перемещающего присваивания.

```cpp
Deque& operator=(Deque&& copy);
```

3. **operator[](const size_t& index):**
   - Возвращает ссылку на элемент по указанному индексу в деке.

```cpp
T& operator[](const size_t& index);
```

4. **operator[](const size_t& index) const:**
   - Возвращает константную ссылку на элемент по указанному индексу в деке.

```cpp
const T& operator[](const size_t& index) const;
```

### Итераторы

1. **begin():**
   - Возвращает итератор, указывающий на первый элемент деки.

```cpp
iterator begin();
```

2. **end():**
   - Возвращает итератор, указывающий на элемент, следующий за последним элементом деки.

```cpp
iterator end();
```

3. **cbegin() const:**
   - Возвращает константный итератор, указывающий на первый элемент деки.

```cpp
const_iterator cbegin() const;
```

4. **cend() const:**
   - Возвращает константный итератор, указывающий на элемент, следующий за последним элементом деки.

```cpp
const_iterator cend() const;
```

5. **rbegin():**
   - Возвращает обратный итератор, указывающий на последний элемент деки.

```cpp
reverse_iterator rbegin() noexcept;
```

6. **rend():**
   - Возвращает обратный итератор, указывающий на элемент перед первым элементом деки.

```cpp
reverse_iterator rend() noexcept;
```

7. **crbegin() const:**
   - Возвращает константный обратный итератор, указывающий на последний элемент деки.

```cpp
const_reverse_iterator crbegin() const noexcept;
```

8. **crend() const:**
   - Возвращает константный обратный итератор, указывающий на элемент перед первым элементом деки.

```cpp
const_reverse_iterator crend() const noexcept;
```

### Вспомогательные классы

1. **Iterator:**
   - Шаблонный класс итератора для деки, поддерживающий как константные, так и неконстантные итераторы.

### Примеры использования

#### Создание деки
```cpp
Deque<int> deque;
```

#### Добавление элементов
```cpp
deque.push_back(10);
deque.push_front(20);
```

#### Удаление элементов
```cpp
deque.pop_back();
deque.pop_front();
```

#### Доступ к элементам
```cpp
int value = deque.at(0);
int value = deque[0];
```

#### Итерация по элементам
```cpp
for (auto it = deque.begin(); it != deque.end(); ++it) {
    std::cout << *it << " ";
}
```
