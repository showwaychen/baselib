#pragma once
#include <memory>
template <class T>
class CSingleTon
{
    static std::shared_ptr<T> s_Instance;

  public:
    static std::shared_ptr<T> GetInst()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = std::shared_ptr<T>(new T());
        }
        return s_Instance;
    }
};
template <class T>
std::shared_ptr<T> CSingleTon<T>::s_Instance = nullptr;

template <class T>
class CSingleTonDeprecated
{
    static T s_Instance;

  public:
    static T *GetInst()
    {
        return &s_Instance;
    }
};
template <class T>
T CSingleTonDeprecated<T>::s_Instance;