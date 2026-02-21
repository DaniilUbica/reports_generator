#include <memory>
#include <type_traits>
#include <utility>

namespace rg {
namespace cpplib {

template<typename SmartPtr>
class out_ptr_t {
private:
    SmartPtr& m_ptr;
    typename SmartPtr::pointer m_raw_ptr;

public:
    explicit out_ptr_t(SmartPtr& ptr) noexcept : m_ptr(ptr), m_raw_ptr(nullptr) {
        m_ptr.reset();
    }

    ~out_ptr_t() {
        if (m_raw_ptr) {
            m_ptr.reset(m_raw_ptr);
        }
    }

    template<typename T>
    operator T**() noexcept {
        static_assert(std::is_pointer_v<T*>, "T must be a pointer type");
        return reinterpret_cast<T**>(&m_raw_ptr);
    }

    out_ptr_t(const out_ptr_t&) = delete;
    out_ptr_t& operator=(const out_ptr_t&) = delete;
};

template<typename SmartPtr>
out_ptr_t<SmartPtr> out_ptr(SmartPtr& ptr) noexcept {
    return out_ptr_t<SmartPtr>(ptr);
}

}
}
