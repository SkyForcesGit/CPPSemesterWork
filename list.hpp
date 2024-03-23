/// \file list.hpp.
/// \author Лошкарев Дмитрий.
/// \date 07.02.2024.
///
/// \brief Содержит собственную реализацию двусвязного списка.
///
/// \namespaces
/// • DataStructures
/// \classes
/// • List

#ifndef SEMESTERWORK_LIST_H
#define SEMESTERWORK_LIST_H

#include <iostream>
#include <random>
#include <cstdint>
#include <initializer_list>

/// \namespace Пространство имен DataStructures содержит в себе
/// классы-реализации двух структур данных: двусвязного списка в виде
/// DataStructures::List и упорядоченной динамически расширяемой хеш-таблицы
/// DataStructures::OrderedHashTable.
///
/// Доступные классы:
/// \n • OrderedHashTable;
/// \n • List;
/// \n • Record.
namespace DataStructures {
// Объявление классов.

    /// \class Класс List предоставляет реализацию структуры данных "двусвязный
    /// список". Также предоставляет класс-итератор для более удобной работы со
    /// списком.
    ///
    /// Публичные методы:
    /// \n • NodeType pop_front()
    /// \n • NodeType pop_back()
    /// \n • void erase(const size_t&)
    /// \n • size_t length() const noexcept
    /// \n • Iterator begin() const
    /// \n • Iterator end() const noexcept
    /// \n • Iterator rbegin() const
    /// \n • Iterator rend() const noexcept
    /// \n • NodeType& operator [] (const size_t& index)
    /// \n • void push_front(const NodeType& data)
    /// \n • void push_back(const NodeType& data)
    /// \n • void insert(const size_t& index, const NodeType& value)
    /// \n • void sort(const size_t& low = 0, const size_t& high = -1,
    ///                int8_t (*comp)(const void*, const void*) = nullptr)
    /// \n • bool empty() const noexcept
    ///
    /// \tparam NodeType Тип данных, который предполагается для использования
    /// в качестве "контейнера" для считываемой и обрабатываемой информации.
    template <class NodeType>
    class List {
        private:
            /// \class класс Node описывает структуру узла двусвязного списка.
            class Node {
                private:
                    NodeType value_;
                    Node* next_;   ///< \brief Указатель на след. элем. списка.
                    Node* prev_;   ///< \brief Указатель на пред. элем. списка.
                public:
                    explicit Node(const NodeType&) noexcept;

                friend class List;
            };

            /// \class класс Iterator предоставляет объект-итератор, дающий
            /// возможность более комфортно итерироваться по элементам списка.
            ///
            /// Публичные методы:
            /// \n • Iterator& operator = (const Node* node) noexcept
            /// \n • Iterator& operator ++ () noexcept
            /// \n • Iterator* operator ++ (int) noexcept
            /// \n • Iterator& operator -- () noexcept
            /// \n • Iterator* operator -- (int) noexcept
            /// \n • bool operator != (const Iterator& iterator) const noexcept
            /// \n • NodeType operator * () const
            class Iterator {
            private:
                const Node* current_node;
            public:
                explicit Iterator(const Node*) noexcept;

                Iterator& operator = (const Node*) noexcept;
                Iterator& operator ++ () noexcept;
                Iterator operator ++ (int) noexcept;
                Iterator& operator -- () noexcept;
                Iterator operator -- (int) noexcept;
                bool operator != (const Iterator&) const noexcept;
                NodeType operator * () const;

                friend class List;
            };

            size_t length_;
            Node *head_, *tail_;

            Node* at(const size_t&);
            size_t partition(const size_t&, const size_t&,
                             int8_t (*)(const void*, const void*));
        public:
            explicit List() noexcept;
            [[maybe_unused]]
            List(std::initializer_list<NodeType>);
            [[maybe_unused]]
            List(const List<NodeType>&);
            ~List();

            NodeType pop_front();
            NodeType pop_back();
            void erase(const size_t&);

            void push_front(const NodeType&);
            void push_back(const NodeType&);
            [[maybe_unused]]
            void insert(const size_t&, const NodeType&);

            NodeType& operator [] (const size_t&);

            [[maybe_unused]]
            void sort(const size_t& = 0, const size_t& = -1,
                      int8_t (*)(const void*, const void*) = nullptr);

            [[maybe_unused]] [[nodiscard]]
            inline size_t length() const noexcept;
            [[maybe_unused]] [[nodiscard]]
            inline Iterator begin() const noexcept;
            [[maybe_unused]] [[nodiscard]]
            inline Iterator end() const noexcept;
            [[maybe_unused]] [[nodiscard]]
            inline Iterator rbegin() const noexcept;
            [[maybe_unused]] [[nodiscard]]
            inline Iterator rend() const noexcept;
            [[maybe_unused]] [[nodiscard]]
            inline bool empty() const noexcept;
    };
    // Определения методов классов.
/* ================================= Iterator ================================= */

    /// \brief Стандартный конструктор экземпляра класса List::Iterator.
    ///
    /// \param node Указатель на узел, на основе которого
    /// нужно создать итератор.
    template <class T>
    List<T>::Iterator::Iterator(const Node* node) noexcept :
            current_node(node) { }

    /// \brief Позволяет переназначить узел, на который ссылается итератор.
    ///
    /// \param node Указатель на новый узел.
    ///
    /// \return Объект-итератор.
    template <class T>
    List<T>::Iterator& List<T>::Iterator::operator = (const Node* node) noexcept {
        this->current_node = node;
        return *this;
    }

    /// \brief Перемещает итератор на след. элемент списка.
    ///
    /// \return Объект-итератор.
    template <class T>
    List<T>::Iterator& List<T>::Iterator::operator ++ () noexcept {
        if (this->current_node != nullptr)
            this->current_node = this->current_node->next_;
        return *this;
    }

    /// \brief Перемещает итератор на след. элемент списка.
    ///
    /// \return Объект-итератор.
    template <class T>
    List<T>::Iterator List<T>::Iterator::operator ++ (int) noexcept {
        Iterator iterator = *this;
        ++*this;
        return iterator;
    }

    /// \brief Перемещает итератор на пред. элемент списка.
    ///
    /// \return Объект-итератор.
    template <class T>
    List<T>::Iterator& List<T>::Iterator::operator -- () noexcept {
        if (this->current_node != nullptr)
            this->current_node = this->current_node->prev_;
        return *this;
    }

    /// \brief Перемещает итератор на пред. элемент списка.
    ///
    /// \return Объект-итератор.
    template <class T>
    List<T>::Iterator List<T>::Iterator::operator -- (int) noexcept {
        Iterator* iterator = *this;
        --*this;
        return iterator;
    }

    /// \brief Проверяет, что два объекта итератора не равны.
    ///
    /// \param iterator Объект-итератор для сравнения.
    ///
    /// \return Булево значение.
    template <class T>
    bool List<T>::Iterator::operator != (const Iterator& iterator) const noexcept {
        return this->current_node != iterator.current_node;
    }

    /// \brief Позволяет получить значение, хранящееся в узле.
    ///
    /// \return Значение узла списка указанного при создании типа.
    template <class T>
    T List<T>::Iterator::operator * () const {
        return this->current_node->value_;
    }

/* ================================== Node ================================== */
    
    /// \brief Стандартный конструктор экземпляра класса List::Node.
    ///
    /// \param value Значение указанного при создании списка типа
    /// данных, которое должен хранить узел.
    template <class T>
    List<T>::Node::Node(const T& value) noexcept :
            value_(value), next_(nullptr), prev_(nullptr) { }

/* ================================== List ================================== */
// PRIVATE

    /// \brief Позволяет получить узел списка по индексу.
    ///
    /// \param index Индекс узла.
    ///
    /// \return Указатель на искомый узел.
    ///
    /// \throw std::out_of_range Исключение возбуждается, если индекс
    /// выходит за границы списка.
    template <class T>
    List<T>::Node* List<T>::at(const size_t& index) {
        if (index > (this->length_ - 1)) {
            throw std::out_of_range("List index is out of range.");
        }
        size_t count{};
        Node* search_ptr;

        if (index < (this->length_ / 2)) {
            search_ptr = this->head_;
            while (count != index) {
                if (search_ptr == nullptr)
                    return nullptr;
                search_ptr = search_ptr->next_;
                count++;
            }
        }
        else {
            search_ptr = this->tail_;
            count = this->length_ - 1;
            while (count != index) {
                if (search_ptr == nullptr)
                    return nullptr;
                search_ptr = search_ptr->prev_;
                count--;
            }
        }
        return search_ptr;
    }

    /// \brief Делит список на части, упорядочивает его.
    ///
    /// \param low Нижний индекс списка.
    /// \param high Верхний индекс списка.
    /// \param comp Функция-компаратор.
    ///
    /// \return Точка привязки.
    template <class T>
    size_t List<T>::partition(const size_t& low, const size_t& high,
                              int8_t (*comp)(const void*, const void*)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distrib(low, high);
        // Случайный выбор опорной точки.
        auto pivot{ this->at(distrib(gen))->value_ };
        size_t i{ low }, j{ high };  // Левый и правый индекс-указатель.

        while (true) {
            auto i_val{ this->at(i)->value_ };
            auto j_val{ this->at(j)->value_ };

            // Двигаем левый указатель, пока значение по индексу меньше
            // опорной точки.
            while ((comp != nullptr) ? comp(&i_val, &pivot) == -1 : i_val < pivot)
                i_val = this->at(i += 1)->value_;
            // Двигаем правый указатель, пока значение по индексу больше
            // опорной точки.
            while ((comp != nullptr) ? comp(&j_val, &pivot) == 1 : j_val > pivot)
                j_val = this->at(j -= 1)->value_;

            // Если указатели пересеклись:
            if (i >= j)
                return j;

            // Обмен значениями.
            std::swap(this->at(i++)->value_,
                      this->at(j--)->value_);
        }
    }

// PUBLIC

    /// \brief Стандартный конструктор экземпляра класса List.
    template <class T>
    List<T>::List() noexcept :
            length_(0ULL), head_(nullptr), tail_(nullptr) { }

    /// \brief Делегирующий конструктор экземпляра класса List с поддержкой
    /// std::initializer_list.
    ///
    /// \param values Значения, которые нужно сохранить в список.
    template <class T>
    [[maybe_unused]]
    List<T>::List(std::initializer_list<T> values) : List() {
        // Занесение элементов из std::initializer_list в список.
        for (auto& value : values)
            this->push_back(value);
    }

    /// \brief Делегирующий конструктор копирования для экземпляра
    /// класса List.
    ///
    /// \param copy_list Список для копирования.
    template <class T>
    [[maybe_unused]]
    List<T>::List(const List<T>& copy_list) : List() {
        // Занесение значений из copy_list в новый список.
        for (auto it : copy_list)
            this->push_back(*it);
    }

    // Стандартный деструктор экземпляра.
    template <class T>
    List<T>::~List() {
        while (!this->empty())
            pop_front();
        delete this->head_;
        delete this->tail_;
    }

    /// \brief Извлекает элемент из начала списка и возвращает его значение.
    ///
    /// \return Значение, хранящееся в узле списка.
    template <class T>
    T List<T>::pop_front() {
        if (this->empty())
            return T{};

        this->length_--;
        Node* new_front_node{ this->head_->next_ };
        T popped_node_value{ this->head_->value_ };

        if (new_front_node != nullptr)
            new_front_node->prev_ = nullptr;
        else
            this->tail_ = new_front_node;

        delete this->head_;
        this->head_ = new_front_node;
        return popped_node_value;
    }

    /// \brief Извлекает элемент из конца списка и возвращает его значение.
    ///
    /// \return Значение, хранящееся в узле списка.
    template <class T>
    T List<T>::pop_back() {
        if (this->empty())
            return T{};

        this->length_--;
        Node* new_back_node{ this->tail_->prev_ };
        T popped_node_value{ this->tail_->value_ };

        if (new_back_node != nullptr)
            new_back_node->next_ = nullptr;
        else
            this->head_ = new_back_node;

        delete this->tail_;
        this->tail_ = new_back_node;
        return popped_node_value;
    }

    /// \brief Удаляет произвольный элемент списка.
    ///
    /// \param index Индекс узла.
    template <class T>
    void List<T>::erase(const size_t& index) {
        Node* erasing_node{ this->at(index) };

        if (erasing_node == nullptr)
            return;
        if (erasing_node->prev_ == nullptr) {
            pop_front();
            return;
        }
        if (erasing_node->next_ == nullptr) {
            pop_back();
            return;
        }

        this->length_--;
        Node* left_node{ erasing_node->prev_ };
        Node* right_node{ erasing_node->next_ };
        left_node->next_ = right_node;
        right_node->prev_ = left_node;

        delete erasing_node;
    }

    /// \brief Добавляет узел в начало списка.
    ///
    /// \param data Данные, которые нужно внести в узел.
    template <class T>
    void List<T>::push_front(const T& data) {
        this->length_++;
        auto* new_node{ new Node(data) };
        new_node->next_ = this->head_;

        if (this->head_ != nullptr)
            this->head_->prev_ = new_node;
        if (this->tail_ == nullptr)
            this->tail_ = new_node;

        this->head_ = new_node;
    }

    /// \brief Добавляет узел в конец списка.
    ///
    /// \param data Данные, которые нужно внести в узел.
    template <class T>
    void List<T>::push_back(const T& data) {
        this->length_++;
        auto* new_node{ new Node(data) };
        new_node->prev_ = this->tail_;

        if (this->tail_ != nullptr)
            this->tail_->next_ = new_node;
        if (this->head_ == nullptr)
            this->head_ = new_node;

        this->tail_ = new_node;
    }

    /// \brief Вставляет новый узел в произвольное место списка
    ///
    /// \param index Индекс, на который нужно вставить новый узел.
    /// \param value Данные, которые нужно внести в узел.
    template <class T>
    [[maybe_unused]]
    void List<T>::insert(const size_t& index, const T& value) {
        Node* right_node{ this->at(index) };
        if (right_node == nullptr)
            return this->push_back(value);

        Node* left_node{ right_node->prev_ };
        if (left_node == nullptr)
            return this->push_front(value);

        this->length_++;
        auto* new_node{ new Node(value) };
        new_node->prev_ = left_node;
        new_node->next_ = right_node;
        left_node->next_ = new_node;
        right_node->prev_ = new_node;
    }

    /// \brief Позволяет получить значение, хранящееся в указ. узле.
    ///
    /// \param index Индекс узла.
    ///
    /// \return Значение узла списка указанного при создании типа.
    template <class T>
    T& List<T>::operator [] (const size_t& index) {
        return this->at(index)->value_;
    }

    /// \brief Реализует алгоритм Хоара. Сортирует список или его часть,
    /// делит его на части, затем сортирует их.
    ///
    /// \param low Нижний индекс списка. Стандартное значение - начало списка.
    /// \param high Верхний индекс списка. Стандартное значение - конец списка.
    /// \param comp Функция-компаратор. Без нее пытается метод простым способом
    /// сравнить два значения.
    /// Функция должна возвращать -1, если A < B, 0 - если A == B, 1 - если
    /// A > B.
    template <class T>
    [[maybe_unused]]
    void List<T>::sort(const size_t& low, const size_t& high,
              int8_t (*comp)(const void*, const void*)) {
        size_t high_{ (high > this->length_ - 1) ? this->length_ - 1 : high };

        if (low < high_) {
            size_t pivot{ this->partition(low, high_, comp) };
            this->sort(low, pivot, comp);
            this->sort(pivot + 1, high_, comp);
        }
    }

    /// \brief Позволяет получить количество элементов списка.
    ///
    /// \return Значение кол-ва элементов.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline size_t List<T>::length() const noexcept {
        return this->length_;
    }

    /// \brief Создает итератор от начала списка.
    ///
    /// \return Объект-итератор.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline List<T>::Iterator List<T>::begin() const noexcept {
        return Iterator(this->head_);
    }

    /// \brief Создает итератор на конец списка (nullptr).
    ///
    /// \return Объект-итератор.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline List<T>::Iterator List<T>::end() const noexcept {
        return Iterator(nullptr);
    }

    /// \brief Создает итератор от конца списка (реверсивный перебор).
    ///
    /// \return Объект-итератор.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline List<T>::Iterator List<T>::rbegin() const noexcept {
        return Iterator(this->tail_);
    }

    /// \brief Создает итератор на начало списка (nullptr)
    /// (реверсивный перебор).
    ///
    /// \return Объект-итератор.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline List<T>::Iterator List<T>::rend() const noexcept {
        return Iterator(nullptr);
    }

    /// \brief Проверяет, пуст ли список.
    ///
    /// \return Булево значение.
    template <class T>
    [[maybe_unused]] [[nodiscard]]
    inline bool List<T>::empty() const noexcept {
        return this->length_ == 0;
    }
}

#endif
