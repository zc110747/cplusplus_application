
#pragma once

#include <memory>

class ModelImpl;
class Model 
{
public:
    Model();
    ~Model();
    void load(void);
private:
    std::shared_ptr<ModelImpl> m_impl;
};
