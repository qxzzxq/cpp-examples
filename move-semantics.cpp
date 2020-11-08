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

    //copy constructor
    SmtPtr(SmtPtr &smtPtr) {
        std::cout << "copy initialization\n";
        if (!smtPtr.isNull()) {
            m_ptr = new T;
            *m_ptr = *smtPtr.m_ptr;
        }
    }

    // copy assignment
    SmtPtr &operator=(const SmtPtr &smtPtr) {
        std::cout << "copy assignment\n";
        if (&smtPtr != this) {
            delete m_ptr;
            m_ptr = new T;
            *m_ptr = *smtPtr.m_ptr;
        }
        return *this;
    }

    // move constructor
    SmtPtr(SmtPtr &&smtPtr) noexcept {
        std::cout << "move initialization\n";
        if (!smtPtr.isNull()) {
            m_ptr = smtPtr.m_ptr;
            smtPtr.m_ptr = nullptr;
        }
    }

    // move assignment
    SmtPtr &operator=(SmtPtr &&smtPtr) noexcept {
        std::cout << "move assignment\n";
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
        std::cout << "Resource " << std::to_string(id) << " acquired\n";
    }

    Resource(const Resource &res) {
        std::cout << "Copy initialize resource\n";
    }

    Resource &operator=(const Resource &res) {
        std::cout << "Assign resource\n";
        return *this;
    }

    ~Resource() {
        std::cout << "Resource " << std::to_string(id) << " destroyed\n";
    }

    void say() const {
        std::cout << "resource id: " + std::to_string(id) + '\n';
    }
};

void checkSmartPointer(const SmtPtr<Resource> &res) {
    if (res.isNull()) {
        std::cout << "null pointer\n";
    } else {
        res->say();
    }
}

void checkSmartPointerCP(const SmtPtr<Resource> res) {
    if (res.isNull()) {
        std::cout << "null pointer\n";
    } else {
        res->say();
    }
}

SmtPtr<Resource> generateResource() {
    SmtPtr<Resource> out{new Resource};
    return out;
}

int main() {
    Resource *res{new Resource{}};
    res->say();

    SmtPtr<Resource> smtPtr1(res);

    std::cout << "1.-------------\n";
    checkSmartPointer(smtPtr1);
    checkSmartPointerCP(smtPtr1);

    std::cout << "2.-------------\n";
    SmtPtr<Resource> smtPtr2(smtPtr1);
    checkSmartPointer(smtPtr2);
    checkSmartPointer(smtPtr1);

    std::cout << "3.-------------\n";
    SmtPtr<Resource> smtPtr3;
    smtPtr3 = smtPtr2;
    checkSmartPointer(smtPtr3);
    checkSmartPointer(smtPtr2);

    std::cout << "4.-------------\n";
    auto res2 = generateResource();  // use move constructor
    res2->say();

    std::cout << "end.-------------\n";
    return 0;
}
