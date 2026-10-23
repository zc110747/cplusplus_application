
#include <iostream>
#include "models.hpp"

class ModelImpl 
{
public:
    ModelImpl() 
    {
        std::cout << "ModelImpl created." << std::endl;
    }

    ~ModelImpl() 
    {
        std::cout << "ModelImpl destroyed." << std::endl;
    }

    void load(void) {
        std::cout << "ModelImpl loaded." << std::endl;
    }
private:
    // Add any private members or methods for ModelImpl here
};

Model::Model() : m_impl(std::make_shared<ModelImpl>()) 
{
    std::cout << "Model created." << std::endl;
}

Model::~Model() 
{
    std::cout << "Model destroyed." << std::endl;
}

void Model::load(void) 
{
    m_impl->load();
}
