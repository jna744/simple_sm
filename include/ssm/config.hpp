#ifndef SSM_CONFIG_HPP
#define SSM_CONFIG_HPP

#if __cplusplus >= 201703L
#define SSM_CPP_VERSION 17
#elif __cplusplus >= 201402L
#define SSM_CPP_VERSION 14
#else
#error "scl requires at least c++14"
#endif

#define SSM_IS_CPP(version) version == SSM_CPP_VERSION

#if SSM_IS_CPP(14)

#define SSM_INLINE_17
#define SSM_INLINE_14 inline

#define SSM_ANONYMOUS_NS_BEGIN namespace {

#define SSM_ANONYMOUS_NS_END }

#else

#define SSM_INLINE_17 inline
#define SSM_INLINE_14

#define SSM_ANONYMOUS_NS_BEGIN

#define SSM_ANONYMOUS_NS_END

#endif

#define SSM_INLINE(version) SSM_INLINE_##version

namespace ssm {

namespace detail {

template <typename T> struct static_const {
  static constexpr T value = {};
};

template <typename T> constexpr T static_const<T>::value;

} // namespace detail

} // namespace ssm

#define SSM_CPO_WRAP(cpo_t) ::ssm::detail::static_const<cpo_t>::value

#if defined(__GNUC__) || defined(__clang__)
#define SSM_UNREACHABLE() __builtin_unreachable()
#else
#define SSM_UNREACHABLE() (void)0
#endif

#if defined __GNUC__
#define SSM_LIKELY(EXPR) __builtin_expect(!!(EXPR), 1)
#else
#define SSM_LIKELY(EXPR) (!!(EXPR))
#endif

#if defined NDEBUG
#define SSM_ASSERT(CHECK) void(0)
#else
#include <cassert>
#define SSM_ASSERT(CHECK)                                                      \
  (SSM_LIKELY(CHECK) ? void(0) : [] { assert(!#CHECK); }())
#endif

#endif // SSM_CONFIG_HPP
