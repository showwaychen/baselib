#pragma once
#include <memory>
#include <functional>
#include <list>
template <class... ARGS>
class slot_base
{
  public:
    virtual void Call(ARGS... args) = 0;
};
template <class T, class... ARGS>
class slot_imp : public slot_base<ARGS...>
{
    std::weak_ptr<T> m_wpIns;
    std::function<void(T *, ARGS...)> m_pFun;

  public:
    slot_imp(std::shared_ptr<T> ins, std::function<void(T *, ARGS...)> fun) : m_wpIns(ins), m_pFun(fun)
    {
    }
    virtual void Call(ARGS... args)
    {
        auto sptr = m_wpIns.lock();
        // mfun(sptr.get(), std::forward<ARGS>(args)...);
        if (sptr != nullptr)
        {
            m_pFun(sptr.get(), args...);
        }
    }
};
template <class... ARGS>
class WeakSigSlot
{
    // using connectortype = std::pair<std::weak_ptr<T>, std::function<void(T *, ARGS...)>>;
    std::list<std::shared_ptr<slot_base<ARGS...>>> m_cConnectors;
    // std::list<slot_base<ARGS...> *> m_cConnectors;

  public:
    template <class T>
    void connect(std::shared_ptr<T> sptr, std::function<void(T *, ARGS...)> mfun)
    {
        std::shared_ptr<slot_imp<T, ARGS...>> slot = std::make_shared<slot_imp<T, ARGS...>>(sptr, mfun);
        // slot_imp<T, ARGS...> *slot = new slot_imp<T, ARGS...>(sptr, mfun);
        m_cConnectors.push_back(slot);
        // m_cConnectors.push_back(std::make_pair(sptr, mfun));
    }
    void operator()(ARGS... args)
    {
        auto ite = m_cConnectors.begin();
        while (ite != m_cConnectors.end())
        {
            auto sig = *ite;
            sig->Call(args...);
            ite++;
        }
    }
};