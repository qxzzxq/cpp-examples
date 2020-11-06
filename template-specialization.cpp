#include <iostream>

class Object {
private:
    char m_data;
public:
    Object() : m_data{} {
    }

    explicit Object(char data) : m_data{data} {
    }

    friend std::ostream &operator<<(std::ostream &out, const Object &obj) {
        out << obj.m_data;
        return out;
    }
};

template<class T, int size>
class BaseStorage {
protected:
    T m_data[size];
    int m_length{size};

    BaseStorage() : m_data{} {
        std::cout << "Initialise BaseStorage of type \"" << typeid(T).name() << "\" with length " << m_length << '\n';
    }

    void checkIndex(int index) {
        if (index >= m_length || index < 0) {
            std::cout << "Invalid index " << index << '\n';
            exit(1);
        }
    }

public:

    virtual BaseStorage &set(T value, int index) {
        checkIndex(index);
        m_data[index] = value;
        return *this;
    }

    virtual ~BaseStorage() {
        std::cout << "Destruct BaseStorage\n";
    }

    T get(int index) {
        return m_data[index];
    }

    friend std::ostream &operator<<(std::ostream &out, const BaseStorage &str) {
        out << "BaseStorage data: [";
        for (int i{0}; i < str.m_length; ++i) {
            T data{str.m_data[i]};
            if (i == str.m_length - 1) {
                out << data;
            } else {
                out << data << ',';
            }
        }
        out << "]\n";
        return out;
    }
};

template<class T, int size>
class Storage : public BaseStorage<T, size> {
public:
    Storage() : BaseStorage<T, size>() {
        std::cout << "Initialise Storage\n";
    }

    ~Storage() override {
        std::cout << "Destruct Storage\n";
    }

    friend std::ostream &operator<<(std::ostream &out, const Storage &str) {
        out << "Storage data: [";
        for (int i{0}; i < str.m_length; ++i) {
            T data{str.m_data[i]};
            if (i == str.m_length - 1) {
                out << data;
            } else {
                out << data << ',';
            }
        }
        out << "]\n";
        return out;
    }

};

template<class T, int size>
class Storage<T *, size> : public BaseStorage<T *, size> {
public:
    Storage() : BaseStorage<T *, size>{} {
        std::cout << "Initialise Storage (pointer specialized)\n";
    }

    friend std::ostream &operator<<(std::ostream &out, const Storage &str) {
        out << "Storage data: [";
        for (int i{0}; i < str.m_length; ++i) {
            T *data{str.m_data[i]};
            if (i == str.m_length - 1) {
                out << *data;
            } else {
                out << *data << ',';
            }
        }
        out << "]\n";
        return out;
    }
};

// Object class specialized
template<int size>
class Storage<Object, size> : public BaseStorage<Object, size> {
public:
    friend std::ostream &operator<<(std::ostream &out, const Storage &str) {
        out << "Storage data: ";
        for (int i{0}; i < str.m_length; ++i) {
            Object data{str.m_data[i]};
            out << data;
        }
        out << '\n';
        return out;
    }
};

// Object pointer specialized
template<int size>
class Storage<Object *, size> : public BaseStorage<Object *, size> {
public:
    friend std::ostream &operator<<(std::ostream &out, const Storage &str) {
        out << "Storage data: ";
        for (int i{0}; i < str.m_length; ++i) {
            Object *data{str.m_data[i]};
            out << *data;
        }
        out << '\n';
        return out;
    }
};

int main() {

    Storage<int, 3> intStorage{};
    intStorage.set(1, 0);
    intStorage.set(2, 1);
    intStorage.set(3, 2);
    std::cout << intStorage << "==================\n";

    Storage<bool, 3> boolStorage{};
    boolStorage.set(true, 0);
    boolStorage.set(true, 1);
    boolStorage.set(true, 2);
    std::cout << boolStorage << "==================\n";

    Storage<Object, 5> objStorage{};
    objStorage.set(Object{'h'}, 0);
    objStorage.set(Object{'e'}, 1);
    objStorage.set(Object{'l'}, 2);
    objStorage.set(Object{'l'}, 3);
    objStorage.set(Object{'o'}, 4);
    std::cout << objStorage << "==================\n";

    Storage<Object *, 2> objStorage2{};
    objStorage2.set(new Object{'w'}, 0);
    objStorage2.set(new Object{'a'}, 1);
    std::cout << objStorage2 << "\n==================\n";

    // invalid index
    objStorage.set(Object{'o'}, 50);
}
