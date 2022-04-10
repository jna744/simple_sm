#ifndef SIMPLE_SM_STATE_ACTIONS_UPDATE_HPP
#define SIMPLE_SM_STATE_ACTIONS_UPDATE_HPP

#include <simple/sm/config.hpp>
#include <simple/sm/state_actions/fwd.hpp>

#include <simple/mp.hpp>

namespace simple
{

namespace sm
{

namespace state_actions
{

namespace update_impl
{

namespace m = mp;

template <typename State>
using update_m0_detected_t = decltype(m::m_declval<State>().update());

template <typename State, typename Machine>
using update_m1_detected_t = decltype(m::m_declval<State>().update(m::m_declval<Machine>()));

template <typename State>
using update_f0_detected_t = decltype(update(m::m_declval<State>()));

template <typename State, typename Machine>
using update_f1_detected_t = decltype(update(m::m_declval<State>(), m::m_declval<Machine>()));

#define SSM_UPDATE_REQUIRED(type, ...) typename = m::m_if<m::m_is_valid<update_##type##_detected_t, __VA_ARGS__>, void>

template <typename State, typename Machine, SSM_UPDATE_REQUIRED(m1, State&, Machine&)>
inline constexpr void
update_impl(State& state, Machine& machine, m::m_tag_c<3>) noexcept(noexcept(state.update(machine)))
{
  state.update(machine);
}

template <typename State, typename Machine, SSM_UPDATE_REQUIRED(m0, State&)>
inline constexpr void update_impl(State& state, Machine&, m::m_tag_c<2>) noexcept(noexcept(state.update()))
{
  state.update();
}

template <typename State, typename Machine, SSM_UPDATE_REQUIRED(f1, State&)>
inline constexpr void
update_impl(State& state, Machine& machine, m::m_tag_c<1>) noexcept(noexcept(update(state, machine)))
{
  update(state, machine);
}

template <typename State, typename Machine, SSM_UPDATE_REQUIRED(f0, State&)>
inline constexpr void update_impl(State& state, Machine&, m::m_tag_c<0>) noexcept(noexcept(update(state)))
{
  update(state);
}

#undef SSM_UPDATE_REQUIRED

template <typename State, typename Machine>
using update_detected_t = decltype(update_impl(m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}));

template <typename State, typename Machine>
using update_is_nothrow_t =
    m::m_bool<noexcept(update_impl(m::m_declval<State>(), m::m_declval<Machine>(), m::m_tag_c<3>{}))>;

struct update_fn {

  template <
      typename State,
      typename Machine,
      typename = m::m_if<m::m_is_valid<update_detected_t, State&, Machine&>, void>>
  constexpr void operator()(State& state, Machine& machine) const noexcept(update_is_nothrow_t<State&, Machine&>{})
  {
    update_impl(state, machine, m::m_tag_c<3>{});
  }
};

}  // namespace update_impl

template <typename State, typename Machine>
struct is_updatable
  : mp::m_is_valid<update_impl::update_detected_t, mp::m_remove_cvref<State>&, mp::m_remove_cvref<Machine>&> {
};

template <typename State, typename Machine>
struct is_nothrow_updatable : mp::m_eval_or<
                                  mp::m_false,
                                  update_impl::update_is_nothrow_t,
                                  mp::m_remove_cvref<State>&,
                                  mp::m_remove_cvref<Machine>&> {
};

inline namespace cpos
{

SSM_ANONYMOUS_NS_BEGIN

SSM_INLINE(17) constexpr auto& update = SSM_CPO_WRAP(update_impl::update_fn);

SSM_ANONYMOUS_NS_END

}  // namespace cpos

}  // namespace state_actions

}  // namespace sm

}  // namespace simple

#endif  // SIMPLE_SM_STATE_ACTIONS_UPDATE_HPP
