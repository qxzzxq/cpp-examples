#include <iostream>
#include <cstring>
#include <string>

template<class T>
class SmtPtr {
private:
    T *m_ptr{nullptr};

public:
    SmtPtr() = default;

    explicit SmtPtr(T *ptr) : m_ptr{ptr} {
        std::cout << "SmtPtr initialization. " << (isNull() ? "Null" : "Not null") << '\n';
    }

    SmtPtr(SmtPtr &smtPtr) {
        if (!smtPtr.isNull()) {
            m_ptr = smtPtr.m_ptr;
            std::cout << "move initialization\n";
            smtPtr.m_ptr = nullptr;
        }
    }


    SmtPtr &operator=(SmtPtr &smtPtr) {
        std::cout << "transfer ownership\n";
        if (&smtPtr != this) {
            delete m_ptr;
            m_ptr = smtPtr.m_ptr;
            smtPtr.m_ptr = nullptr;
            return *this;
        }
        return *this;
    }

    ~SmtPtr() {
        delete m_ptr;
    }

    T *operator->() const {
        return m_ptr;
    }

    T &operator*() const {
        return *m_ptr;
    }

    [[nodiscard]] bool isNull() const {
        return m_ptr == nullptr;
    }
};

class Resource {
private:
    int id{std::rand() % 100};

public:
    Resource() {
        std::cout << "Resource acquired\n";
    }

    Resource(const Resource &res) {
        std::cout << "Copy initialize resource\n";
    }

    Resource &operator=(const Resource &res) {
        std::cout << "Assign resource\n";
        return *this;
    }

    ~Resource() { std::cout << "Resource destroyed\n"; }

    void say() const {
        std::cout << "resource id: " + std::to_string(id) + '\n';
    }
};

/*
 * If we pass the argument by value (res instead of &res), this will cause the resource be destroyed
 * at the end of the function.
 */
void checkSmartPointer(const SmtPtr<Resource> &res) {
    if (res.isNull()) {
        std::cout << "null pointer\n";
    } else {
        res->say();
    }
}

int main() {
    Resource *res{new Resource{}};
    res->say();

    SmtPtr<Resource> smtPtr1(res);

    std::cout << "1.-------------\n";
    checkSmartPointer(smtPtr1);
    checkSmartPointer(smtPtr1);

    std::cout << "2.-------------\n";
    SmtPtr<Resource> smtPtr2(smtPtr1);
    checkSmartPointer(smtPtr2);
    checkSmartPointer(smtPtr1);  // null pointer

    std::cout << "3.-------------\n";
    SmtPtr<Resource> smtPtr3;
    smtPtr3 = smtPtr2;
    checkSmartPointer(smtPtr3);
    checkSmartPointer(smtPtr2);  // null pointer

    return 0;
}
