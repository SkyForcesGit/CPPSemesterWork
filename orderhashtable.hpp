/// \file orderhashtable.hpp.
/// \author Лошкарев Дмитрий.
/// \date 08.02.2024.
///
/// \brief Содержит собственную реализацию хеш-таблицы.
///
/// \namespaces
/// • DataStructures
/// \classes
/// • OrderedHashTable

#ifndef CPPPROJECT_ORDERHASHTABLE_H
#define CPPPROJECT_ORDERHASHTABLE_H

#include <iostream>
#include <cmath>
#include <format>
#include <cstdint>
#include <utility>

#include "list.hpp"

// Макросы для функции хеширования.
#define HASH_CONST_D ((2.23606797749978969 - 1) / 2)
#define HASH_CONST_I 5381
#define HASH_CONST_I_2 33

/// \namespace Пространство имен DataStructures содержит в себе
/// классы-реализации двух структур данных: двусвязного списка в виде
/// DataStructures::List и упорядоченной динамически расширяемой хеш-таблицы
/// DataStructures::OrderedHashTable.
///
/// Доступные классы:
/// \n • OrderedHashTable;
/// \n • List.
namespace DataStructures {
// Объявление классов.

    /// \class Класс OrderedHashTable предоставляет реализацию структуры
    /// данных "хеш-таблица", позволяет эффективно хранить пары "ключ-значение"
    /// и обращаться к ним.
    /// Также класс предоставляет возможность итерироваться
    /// по хеш-таблице в порядке добавления ключей.
    ///
    /// Публичные методы:
    /// \n • void insert(const std::string& key, const HashType& value);
    /// \n • void erase(const std::string& key);
    /// \n • HashType pop();
    /// \n • HashType get(const std::string& key);
    /// \n • HashType& operator [] (const std::string& key).
    ///
    /// \tparam HashType Тип данных, который предполагается для использования
    /// в качестве "контейнера" для считываемой и обрабатываемой информации.
    template <class HashType>
    class OrderedHashTable {
        private:
            /// \class Класс KeyException описывает тип исключения,
            /// возбуждаемого при попытке доступа к несуществующему элементу
            /// хеш-таблицы с помощью перегруженного оператора [].
            ///
            /// Публичные методы:
            /// \n • std::string what() noexcept
            class KeyException : public std::exception {
                private:
                    std::string key_;
                public:
                    explicit KeyException(std::string) noexcept;

                    using std::exception::what;
                    [[maybe_unused]]
                    std::string what() noexcept;

                friend class OrderedHashTable;
            };

            /// \class Класс Record описывает объект записи хеш-таблицы,
            /// состоящий из строки-ключа и значения типа, указанного в
            /// качестве "контейнера" для данных при создании хеш-таблицы.
            class Record {
                private:
                    std::string key_;
                    HashType value_;
                public:
                    explicit Record(std::string, const HashType&)
                        noexcept;

                friend class OrderedHashTable;
            };

            // Статические константы класса.
            static inline constexpr size_t MIN_TABLE_SIZE{ 64 };    ///< \brief Минимальный размер хеш-таблицы.
            static inline constexpr uint32_t GROWTH_RATE{ 2 };       ///< \brief Коэффициент расширения хеш-таблицы.
            static inline constexpr double MAX_UTIL_PERCENT{ 0.5 };  ///< \brief Коэффициент заполнения.
                                                                     ///<
                                                                     ///< Отношение количества ключей
                                                                     ///< к физическому размеру таблицы.

            size_t size_{ MIN_TABLE_SIZE };  ///< \brief "Физический" размер хеш-таблицы.
            uint32_t record_count_;
            List<Record*>** record_arr_;     ///< \brief Массив двусвязных списков для пар
                                             ///< "ключ-значение".
            List<std::string>* key_list_;    ///< \brief Список ключей в порядке их добавления.
                                             ///<
                                             ///< Благодаря ему хеш-таблицу можно
                                             ///< назвать упорядоченной.

            [[nodiscard]]
            uint64_t hash_function(const std::string&,
                                   const size_t& = 0) const noexcept;
            void expand();
        public:
            explicit OrderedHashTable() noexcept;
            [[maybe_unused]]
            explicit OrderedHashTable(const size_t&) noexcept;
            ~OrderedHashTable();

            [[nodiscard]] [[maybe_unused]]
            inline const List<std::string>* keys() const noexcept;
            [[nodiscard]] [[maybe_unused]]
            inline const uint32_t& length() const noexcept;

            [[maybe_unused]]
            void insert(const std::string& key, const HashType& value);
            [[maybe_unused]]
            void erase(const std::string& key);
            [[maybe_unused]]
            HashType pop();
            HashType get(const std::string& key);
            HashType& operator [] (const std::string& key);
    };

// Определения методов классов.
/* ============================== KeyException ============================== */

    /// \brief Стандартный конструктор экземпляра класса
    /// OrderedHashTable::KeyException.
    ///
    /// Задает сообщение об ошибке и сразу же выводит в поток
    /// std::cerr.
    ///
    /// \param key Строковый ключ, который возбудил исключение.
    template <class T>
    OrderedHashTable<T>::KeyException::KeyException(std::string key) noexcept :
            key_(std::move(key)) {
        std::cerr << this->what() << std::endl;
    }

    /// \brief Формирует сообщение о произошедшей ошибке.
    ///
    /// \return Строку с пояснением ошибки и советом.
    template <class T>
    [[maybe_unused]]
    std::string OrderedHashTable<T>::KeyException::what() noexcept {
        std::string msg{ std::format("Key (\"{}\") not found. Use "
                                     ".get() method if you not "
                                     "sure that record exits.",
                                     this->key_) };
        return msg;
    }

/* ================================= Record ================================= */

    /// \brief Стандартный конструктор экземпляра класса
    /// OrderedHashTable::Record.
    ///
    /// Позволяет создать объект записи, указав сразу ключ и
    /// значение.
    ///
    /// \param key Строковый ключ записи.
    /// \param value Значение записи.
    template <class T>
    OrderedHashTable<T>::Record::Record(std::string key, const T& value) noexcept :
            key_(std::move(key)), value_(value) { }

/* ============================ OrderedHashTable ============================ */
// PRIVATE

    /// \brief Метод, высчитывающий хеш.
    ///
    /// Преобразует ключ элемента в уникальную (почти) цифровую
    /// последовательность - хеш. Этот хеш определяет местоположение
    /// элемента в хеш-таблице..
    ///
    /// \param key Строковый ключ, который необходимо захешировать.
    /// \param arr_size Размерная переменная, отображающая текущий размер
    /// хеш-таблицы. По умолчанию используется размер из экземпляра класса,
    /// но при необходимости для расчета хеша может использоваться стороннее
    /// значение.
    ///
    /// \return uint64-значение хеша - индекса элемента в хеш-таблице.
    template <class T>
    uint64_t OrderedHashTable<T>::hash_function(const std::string& key,
                                                const size_t& arr_size)
    const noexcept {
        size_t size_arr_{ (arr_size == 0) ? this->size_ : arr_size };
        uint32_t key_int{ HASH_CONST_I };
        uint64_t hash;
        // Хеширование строки в число.
        for (const char& i : key)
            key_int = HASH_CONST_I_2 * key_int + static_cast<unsigned char>(i);

        // Хеширование числового представления строки в зависимости от размера
        // HASH_CONST_D - константа для наилучшего распределения ключей,
        // описана Кнутом.
        hash = static_cast<uint64_t>(ceil(static_cast<double>(size_arr_) *
                                          fmod((key_int * HASH_CONST_D), 1)) - 1);
        return hash;
    }

    /// \brief Метод, динамически расширяющий хеш-таблицу по мере ее заполнения.
    ///
    /// Начинают свою работу, когда заполненность хеш-таблицы ключами
    /// превышает определенный процент от общей емкости (MAX_UTIL_PERCENT) таблицы.
    /// Таблица увеличивается в GROWTH_RATE раза.
    template <class T>
    void OrderedHashTable<T>::expand() {
        // Создание новой таблицы и ее заполнение двусвязными списками.
        auto** temp{ new List<Record*>*[this->size_ * GROWTH_RATE] };

        for (size_t i{}; i < this->size_ * GROWTH_RATE; i++)
            temp[i] = new List<Record*>;

        // Перехеширование ключей по новому размеру, запись ключей в новую
        // таблицу.
        for (auto it{ this->key_list_->begin() }; it != this->key_list_->end();
             ++it) {
            uint64_t index{ hash_function((*it), this->size_ *
                                                 GROWTH_RATE) };
            T value{ this->get((*it)) };
            auto* record{ new Record((*it), value) };
            temp[index]->push_back(record);
        }

        // Удаление старой таблицы, изменение значение размера таблицы в
        // экземпляре класса.
        for (size_t item{}; item < this->size_ / GROWTH_RATE; item++)
            delete this->record_arr_[item];
        delete[] this->record_arr_;
        this->record_arr_ = temp;
        this->size_ *= GROWTH_RATE;
    }

// PUBLIC

    /// \brief Стандартный конструктор экземпляра класса OrderedHashTable.
    ///
    /// Инициализирует хеш-таблицу
    /// со стандартным размером MIN_TABLE_SIZE.
    template <class T>
    OrderedHashTable<T>::OrderedHashTable() noexcept :
            size_(MIN_TABLE_SIZE), record_count_(0),
            record_arr_(new List<Record*>*[this->size_]),
            key_list_(new List<std::string>())
    {
        for (size_t item{}; item < this->size_; item++)
            this->record_arr_[item] = new List<Record*>;
    }

    /// \brief Конструктор экземпляра класса с возможностью указать
    /// размер.
    ///
    /// Инициализирует хеш-таблицу с пользовательским размером,
    /// не меньшим, чем минимальный размер MIN_TABLE_SIZE.
    ///
    /// \param size Физический размер хеш-таблицы.
    template <class T>
    [[maybe_unused]]
    OrderedHashTable<T>::OrderedHashTable(const size_t& size) noexcept :
            size_((MIN_TABLE_SIZE > size) ? MIN_TABLE_SIZE : size),
            record_count_(0), record_arr_(new List<Record*>*[this->size_]),
            key_list_(new List<std::string>())
    {
        for (size_t item{}; item < this->size_; item++)
            this->record_arr_[item] = new List<Record*>;
    }

    // Стандартный деструктор экземпляра.
    template <class T>
    OrderedHashTable<T>::~OrderedHashTable() {
        for (size_t item{}; item < this->size_; item++) {
            auto temp{ this->record_arr_[item] };
            delete temp;
        }
        delete[] this->record_arr_;
        delete this->key_list_;
    }

    /// \brief Предоставляет доступ к ключам хеш-таблицы.
    ///
    /// Возвращает список ключей таблицы в порядке их добавления,
    /// предоставляет возможность итерации по ним.
    ///
    /// \return Список из строковых ключей хеш-таблицы.
    template <class T>
    [[nodiscard]] [[maybe_unused]]
    inline const List<std::string>* OrderedHashTable<T>::keys() const noexcept {
        return this->key_list_;
    }

    /// \brief Предоставляет доступ к количеству элементов таблицы.
    ///
    /// \return Ссылку на переменную, хранящую кол-во ключей.
    template <class T>
    [[nodiscard]] [[maybe_unused]]
    inline const uint32_t& OrderedHashTable<T>::length() const noexcept {
        return this->record_count_;
    }

    /// \brief Метод, добавляющий элемент.
    ///
    /// Добавляет в хеш-таблицу новую пару "ключ - значение" или
    /// изменяет значение по уже существующему ключу.
    ///
    /// \param key Строковый ключ элемента для вставки/изменения.
    /// \param value Значение элемента для вставки/изменения.
    template <class T>
    [[maybe_unused]]
    void OrderedHashTable<T>::insert(const std::string& key, const T& value) {
        size_t index{ hash_function(key) };
        List<Record*>* table_cell{ this->record_arr_[index] };
        // Проверка, что ключ существует в списке по указанному хешу.
        for (auto it{ table_cell->begin() }; it != table_cell->end(); ++it) {
            if ((*it)->key_ == key) {
                (*it)->value_ = value;
                return;
            }
        }
        // Создание новой записи и ее пуш в список по указанному хешу.
        auto* new_record{ new Record(key, value) };
        this->key_list_->push_back(key);
        this->record_count_++;
        table_cell->push_back(new_record);

        // Если ключей уже многовато - пора расширить таблицу.
        if ((this->record_count_ / static_cast<double>(this->size_)) >=
            MAX_UTIL_PERCENT)
            this->expand();
    }

    /// \brief Метод, стирающий из хеш-таблицы элемент с указанным ключом.
    ///
    /// \param key Строковый ключ элемента, который требуется удалить.
    template <class T>
    [[maybe_unused]]
    void OrderedHashTable<T>::erase(const std::string& key) {
        size_t index{ hash_function(key) };
        size_t list_ind{};
        List<Record*>* table_cell{ this->record_arr_[index] };
        // Поиск ключа в списке по указанному хешу.
        for (auto it{ table_cell->begin() }; it != table_cell->end();
                ++it, list_ind++) {
            if ((*it)->key_ == key) {
                table_cell->erase(list_ind);
                this->record_count_--;
                break;
            }
        }
        // Поиск ключа в общем списке ключей.
        list_ind = 0;
        for (auto it{ this->key_list_->begin() };
                it != this->key_list_->end() ; ++it, ++list_ind) {
            if ((*it) == key) {
                this->key_list_->erase(list_ind);
                return;
            }
        }
    }

    /// \brief Удаляет элемент и возвращает его.
    ///
    /// Метод извлекает из хеш-таблицы последний добавленный элемент и
    /// возвращает записанное в него значение.
    ///
    /// \return Значение извлеченного элемента указанного типа данных.
    template <class T>
    [[maybe_unused]]
    T OrderedHashTable<T>::pop() {
        // Извлечение последнего элемента из списка ключей и получение
        // элемента для извлечения по нему.
        std::string key{ this->key_list_->pop_back() };
        size_t index{ hash_function(key) };
        List<Record*>* table_cell{ this->record_arr_[index] };
        Record *popped_record{ table_cell->pop_back() };
        T ret_val{ popped_record->value_ };

        // Удаление извлеченного элемента.
        delete popped_record;
        this->record_count_--;
        return ret_val;
    }

    /// \brief Метод, позволяющий получить значение элемента по ключу.
    ///
    /// В случае ненахождения элемента будет возвращено стандартное значение.
    ///
    /// \param key Строковый ключ, значение по которому нужно найти.
    ///
    /// \return Найденное значение ключа или стандартное значение.
    template <class T>
    T OrderedHashTable<T>::get(const std::string& key) {
        size_t index{ hash_function(key) };
        List<Record*>* table_cell{ this->record_arr_[index] };
        // Поиск элемента в списке по указанному хешу.
        for (auto it{ table_cell->begin() }; it != table_cell->end(); ++it) {
            if ((*it)->key_ == key) {
                T ret_val = (*it)->value_;
                return ret_val;
            }
        }
        // Дефолтное значение.
        return T{};
    }

    /// \brief Перегрузка оператора [] для получения доступа к элементам
    /// хеш-таблицы.
    ///
    /// \param key Строковый ключ элемента для доступа.
    ///
    /// \return Ссылка на значение указанного типа.
    ///
    /// \throw DataStructures::OrderedHashTable::KeyException Возбуждается,
    /// если элемент с указанным ключом не найден. Рекомендуется использовать
    /// .get(), если присутствие ключа не точно.
    template <class T>
    T& OrderedHashTable<T>::operator [] (const std::string& key) {
        size_t index{ hash_function(key) };
        List<Record*>* table_cell{ this->record_arr_[index] };
        // Поиск элемента в списке по указанному хешу.
        for (auto it{ table_cell->begin() }; it != table_cell->end(); ++it) {
            if ((*it)->key_ == key) {
                return (*it)->value_;
            }
        }
        throw KeyException(key);
    }
}

#endif