#pragma once
#include <memory>
#include <functional>
#include <list>
#include <utility>
template <class T, class... ARGS>
class CSigSlot
{
    using connectortype = std::pair<std::shared_ptr<T>, std::function<void(T *, ARGS...)>>;
    std::list<connectortype> m_cConnectors;

  public:
    void connect(std::shared_ptr<T> sptr, std::function<void(T *, ARGS...)> mfun)
    {
        m_cConnectors.push_back(std::make_pair(sptr, mfun));
    }
    void disconnect(std::shared_ptr<T> lsptr)
    {
        auto ite = m_cConnectors.begin();
        while (ite != m_cConnectors.end())
        {
            auto sptr = ite->first;
            if (sptr == lsptr)
            {
                m_cConnectors.erase(ite);
            }
            ite++;
        }
    }
    void operator()(ARGS &&... args)
    {
        auto ite = m_cConnectors.begin();
        while (ite != m_cConnectors.end())
        {
            auto ptr = ite->first;
            auto mfun = ite->second;
            mfun(ptr.get(), std::forward<ARGS>(args)...);
            ite++;
        }
    }
};
template <class T, class... ARGS>
class CWeakSigSlot
{
    using connectortype = std::pair<std::weak_ptr<T>, std::function<void(T *, ARGS...)>>;
    std::list<connectortype> m_cConnectors;

  public:
    void connect(std::shared_ptr<T> sptr, std::function<void(T *, ARGS...)> mfun)
    {
        m_cConnectors.push_back(std::make_pair(sptr, mfun));
    }
    void operator()(ARGS &&... args)
    {
        auto ite = m_cConnectors.begin();
        while (ite != m_cConnectors.end())
        {
            auto wptr = ite->first;
            auto mfun = ite->second;
            auto sptr = wptr.lock();
            if (sptr != nullptr)
            {
                mfun(sptr.get(), std::forward<ARGS>(args)...);
            }
            else
            {
                m_cConnectors.erase(ite);
            }
            ite++;
        }
    }
};