#ifndef SSM_EXIT_HPP
#define SSM_EXIT_HPP

#include <ssm/config.hpp>

#include <smp/smp.hpp>

namespace ssm {

namespace exit_impl {

namespace m = smp;

template <typename State>
using on_exit_m0_detected_t = decltype(m::m_declval<State>().on_exit());

template <typename State, typename Machine>
using on_exit_m1_detected_t =
    decltype(m::m_declval<State>().on_exit(m::m_declval<Machine>()));

template <typename State>
using on_exit_f0_detected_t = decltype(on_exit(m::m_declval<State>()));

template <typename State, typename Machine>
using on_exit_f1_detected_t =
    decltype(on_exit(m::m_declval<State>(), m::m_declval<Machine>()));

void on_exit();

#define SSM_ON_EXIT_REQUIRED(type, ...)                                        \
  typename =                                                                   \
      m::m_if<m::m_is_valid<on_exit_##type##_detected_t, __VA_ARGS__>, void>

template <typename State, typename Machine,
          SSM_ON_EXIT_REQUIRED(m1, State &, Machine &)>
inline constexpr void
on_exit_impl(State &state, Machine &machine,
             m::m_tag_c<3>) noexcept(noexcept(state.on_exit(machine))) {
  state.on_exit(machine);
}

template <typename State, typename Machine, SSM_ON_EXIT_REQUIRED(m0, State &)>
inline constexpr void
on_exit_impl(State &state, Machine &,
             m::m_tag_c<2>) noexcept(noexcept(state.on_exit())) {
  state.on_exit();
}

template <typename State, typename Machine,
          SSM_ON_EXIT_REQUIRED(f1, State &, Machine &)>
inline constexpr void
on_exit_impl(State &state, Machine &machine,
             m::m_tag_c<1>) noexcept(noexcept(on_exit(state, machine))) {
  on_exit(state, machine);
}

template <typename State, typename Machine, SSM_ON_EXIT_REQUIRED(f0, State &)>
inline constexpr void
on_exit_impl(State &state, Machine &,
             m::m_tag_c<0>) noexcept(noexcept(on_exit(state))) {
  on_exit(state);
}

#undef SSM_ON_EXIT_REQUIRED

template <typename State, typename Machine>
using on_exit_detected_t = decltype(on_exit_impl(
    m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}));

template <typename State, typename Machine>
using on_exit_is_nothrow_t = m::m_bool<noexcept(on_exit_impl(
    m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}))>;

struct exit_fn {

  template <typename State, typename Machine,
            typename = m::m_if<
                m::m_is_valid<on_exit_detected_t, State &, Machine &>, void>>
  constexpr void operator()(State &state, Machine &machine) const
      noexcept(on_exit_is_nothrow_t<State &, Machine &>{}) {
    on_exit_impl(state, machine, m::m_tag_c<3>{});
  }
};

struct dummy_machine {};

} // namespace exit_impl

template <typename State, typename Machine = exit_impl::dummy_machine>
struct is_exitable
    : smp::m_is_valid<exit_impl::on_exit_detected_t, State &, Machine &> {};

template <typename State, typename Machine = exit_impl::dummy_machine>
SSM_INLINE(17)
constexpr auto is_exitable_v = is_exitable<State, Machine>::value;

template <typename State, typename Machine = exit_impl::dummy_machine>
struct is_nothrow_exitable
    : smp::m_eval_if<is_exitable<State, Machine>, smp::m_false,
                     exit_impl::on_exit_is_nothrow_t, State &, Machine &> {};

template <typename State, typename Machine = exit_impl::dummy_machine>
SSM_INLINE(17)
constexpr auto is_nothrow_exitable_v =
    is_nothrow_exitable<State, Machine>::value;

inline namespace cpos {

SSM_ANONYMOUS_NS_BEGIN

SSM_INLINE(17)
constexpr auto &exit = SSM_CPO_WRAP(exit_impl::exit_fn);

SSM_ANONYMOUS_NS_END

} // namespace cpos

} // namespace ssm

#endif // SSM_EXIT_HPP
