#ifndef SIMPLE_SM_STATE_ACTIONS_ENTER_HPP
#define SIMPLE_SM_STATE_ACTIONS_ENTER_HPP

#include <simple/sm/config.hpp>
#include <simple/sm/state_actions/fwd.hpp>

#include <simple/mp.hpp>

namespace simple
{

namespace sm
{

namespace state_actions
{

namespace enter_impl
{

namespace m = mp;

template <typename State>
using on_enter_m0_detected_t = decltype(m::m_declval<State>().on_enter());

template <typename State, typename Machine>
using on_enter_m1_detected_t = decltype(m::m_declval<State>().on_enter(m::m_declval<Machine>()));

template <typename State>
using on_enter_f0_detected_t = decltype(on_enter(m::m_declval<State>()));

template <typename State, typename Machine>
using on_enter_f1_detected_t = decltype(on_enter(m::m_declval<State>(), m::m_declval<Machine>()));

void on_enter();

#define SSM_ON_ENTER_REQUIRED(type, ...)                                                                               \
  typename = m::m_if<m::m_is_valid<on_enter_##type##_detected_t, __VA_ARGS__>, void>

template <typename State, typename Machine, SSM_ON_ENTER_REQUIRED(m1, State&, Machine&)>
inline constexpr void
on_enter_impl(State& state, Machine& machine, m::m_tag_c<3>) noexcept(noexcept(state.on_enter(machine)))
{
  state.on_enter(machine);
}

template <typename State, typename Machine, SSM_ON_ENTER_REQUIRED(m0, State&)>
inline constexpr void on_enter_impl(State& state, Machine&, m::m_tag_c<2>) noexcept(noexcept(state.on_enter()))
{
  state.on_enter();
}
template <typename State, typename Machine, SSM_ON_ENTER_REQUIRED(f1, State&, Machine&)>
inline constexpr void
on_enter_impl(State& state, Machine& machine, m::m_tag_c<1>) noexcept(noexcept(on_enter(state, machine)))
{
  on_enter(state, machine);
}

template <typename State, typename Machine, SSM_ON_ENTER_REQUIRED(f0, State&)>
inline constexpr void on_enter_impl(State& state, Machine&, m::m_tag_c<0>) noexcept(noexcept(on_enter(state)))
{
  on_enter(state);
}

#undef SSM_ON_ENTER_REQUIRED

template <typename State, typename Machine>
using on_enter_detected_t = decltype(on_enter_impl(m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}));

template <typename State, typename Machine>
using on_enter_is_nothrow_t =
    m::m_bool<noexcept(on_enter_impl(m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}))>;

struct enter_fn {

  template <
      typename State,
      typename Machine,
      typename = m::m_if<m::m_is_valid<on_enter_detected_t, State&, Machine&>, void>>
  constexpr void operator()(State& state, Machine& machine) const noexcept(on_enter_is_nothrow_t<State&, Machine&>{})
  {
    on_enter_impl(state, machine, m::m_tag_c<3>{});
  }
};

}  // namespace enter_impl

template <typename State, typename Machine>
struct is_enterable
  : mp::m_is_valid<enter_impl::on_enter_detected_t, mp::m_remove_cvref<State>&, mp::m_remove_cvref<Machine>&> {
};

template <typename State, typename Machine>
struct is_nothrow_enterable : mp::m_eval_or<
                                  mp::m_false,
                                  enter_impl::on_enter_is_nothrow_t,
                                  mp::m_remove_cvref<State>&,
                                  mp::m_remove_cvref<Machine>&> {
};

inline namespace cpos
{

SSM_ANONYMOUS_NS_BEGIN

SSM_INLINE(17)
constexpr auto& enter = SSM_CPO_WRAP(enter_impl::enter_fn);

SSM_ANONYMOUS_NS_END

}  // namespace cpos

}  // namespace state_actions

}  // namespace sm

}  // namespace simple

#endif  // SIMPLE_SM_STATE_ACTIONS_ENTER_HPP
