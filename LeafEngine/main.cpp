#include <iostream>

class Singleton
{
    Singleton(const std::string value) : value_(value)
    {
    }
    static Singleton* singleton_;
    std::string value_;
public:
    Singleton(Singleton& other) = delete;
    void operator=(const Singleton&) = delete;
    static Singleton* GetInstance(const std::string& value);
    void SomeBusinessLogic()
    {
        std::cout << "aidsjidjasd\n";
    }
    std::string value() const {
        return value_;
    }
};

Singleton* Singleton::singleton_ = nullptr;

Singleton* Singleton::GetInstance(const std::string& value)
{
    if (Singleton::singleton_ == nullptr) {
        Singleton::singleton_ = new Singleton(value);
    }
    return Singleton::singleton_;
}

int main(void) {
    Singleton* sngl = Singleton::GetInstance("sadjisidj");
    Singleton sn = Singleton();
    std::cout << sngl->value() << std::endl;
	return 0;
}