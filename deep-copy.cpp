#include <iostream>
#include <cstring>

template<class T>
class SmtPtr {
private:
    T *m_ptr{nullptr};

    void deepCopy(const SmtPtr &smtPtr) {

        if (!m_ptr) {
            std::cout << "release memory";
            delete m_ptr;
        }

        if (smtPtr.m_ptr) {
            m_ptr = new T;
            *m_ptr = *smtPtr.m_ptr;
        } else {
            m_ptr = nullptr;
        }
    }

public:
    explicit SmtPtr(T *ptr) : m_ptr{ptr} {
        std::cout << "SmtPtr initialization\n";
    }

    SmtPtr(const SmtPtr &smtPtr) : m_ptr{smtPtr.m_ptr} {
        std::cout << "copy initialization\n";
        deepCopy(smtPtr);
    }

    SmtPtr &operator=(const SmtPtr &smtPtr) {
        if (this != &smtPtr) {
            deepCopy(smtPtr);
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
};

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }

    ~Resource() { std::cout << "Resource destroyed\n"; }

    static std::string say() {
        return "hahaha\n";
    }
};

void passByValue(const SmtPtr<Resource> res) {
    std::cout << res->say();
}

int main() {
    Resource *res{new Resource{}};
    SmtPtr<Resource> res1(res);
    SmtPtr<Resource> res2(res1);
    passByValue(res1);
    passByValue(res2);
    return 0;
}
