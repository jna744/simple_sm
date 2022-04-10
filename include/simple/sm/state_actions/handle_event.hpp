#ifndef SIMPLE_SM_ACTIONS_HANDLE_EVENT_HPP
#define SIMPLE_SM_ACTIONS_HANDLE_EVENT_HPP

#include <simple/sm/config.hpp>
#include <simple/sm/state_actions/fwd.hpp>

#include <simple/mp.hpp>

namespace simple
{

namespace sm
{

namespace state_actions
{

namespace handle_event_impl
{

namespace m = mp;

template <typename State, typename Event>
using handle_event_m0_detected_t = decltype(m::m_declval<State>().handle_event(m::m_declval<Event>()));

template <typename State, typename Event, typename Machine>
using handle_event_m1_detected_t =
    decltype(m::m_declval<State>().handle_event(m::m_declval<Event>(), m::m_declval<Machine>()));

template <typename State, typename Event>
using handle_event_f0_detected_t = decltype(handle_event(m::m_declval<State>(), m::m_declval<Event>()));

template <typename State, typename Event, typename Machine>
using handle_event_f1_detected_t =
    decltype(handle_event(m::m_declval<State>(), m::m_declval<Event>(), m::m_declval<Machine>()));

void handle_event();

template <typename State, typename... Args>
inline constexpr bool invoke_mem_event_handler(m::m_true, State& state, Args&&... args)
{
  return state.handle_event(std::forward<Args>(args)...);
}

template <typename State, typename... Args>
inline constexpr bool invoke_mem_event_handler(m::m_false, State& state, Args&&... args)
{
  state.handle_event(std::forward<Args>(args)...);
  return true;
}

template <typename State, typename... Args>
inline constexpr bool invoke_free_event_handler(m::m_true, State& state, Args&&... args)
{
  return handle_event(state, std::forward<Args>(args)...);
}

template <typename State, typename... Args>
inline constexpr bool invoke_free_event_handler(m::m_false, State& state, Args&&... args)
{
  handle_event(state, std::forward<Args>(args)...);
  return true;
}

template <template <typename...> class Detector, typename... Args>
using with_bool_return = m::m_same<Detector<Args...>, bool>;

#define SSM_HANDLE_EVENT_REQUIRED(type, ...)                                                                           \
  typename = m::m_if<m::m_is_valid<handle_event_##type##_detected_t, __VA_ARGS__>, void>

#define SSM_HANDLE_EVENT_RETURNS_BOOL(type, ...)                                                                       \
  with_bool_return<handle_event_##type##_detected_t, __VA_ARGS__> {}

template <
    typename State,
    typename Event,
    typename Machine,
    SSM_HANDLE_EVENT_REQUIRED(m1, State&, Event const&, Machine&)>
inline constexpr bool handle_event_impl(State& state, Event const& event, Machine& machine, m::m_tag_c<3>) noexcept(
    noexcept(state.handle_event(event, machine)))
{
  return invoke_mem_event_handler(
      SSM_HANDLE_EVENT_RETURNS_BOOL(m1, State&, Event const&, Machine&), state, event, machine);
}

template <typename State, typename Event, typename Machine, SSM_HANDLE_EVENT_REQUIRED(m0, State&, Event const&)>
inline constexpr bool handle_event_impl(State& state, Event const& event, Machine&, m::m_tag_c<2>) noexcept(
    noexcept(state.handle_event(event)))
{
  return invoke_mem_event_handler(SSM_HANDLE_EVENT_RETURNS_BOOL(m0, State&, Event const&), state, event);
}

template <
    typename State,
    typename Event,
    typename Machine,
    SSM_HANDLE_EVENT_REQUIRED(f1, State&, Event const&, Machine&)>
inline constexpr bool handle_event_impl(State& state, Event const& event, Machine& machine, m::m_tag_c<1>) noexcept(
    noexcept(handle_event(state, event, machine)))
{
  return invoke_free_event_handler(
      SSM_HANDLE_EVENT_RETURNS_BOOL(f1, State&, Event const&, Machine&), state, event, machine);
}

template <
    typename State,
    typename Event,
    typename Machine,
    SSM_HANDLE_EVENT_REQUIRED(f0, State&, Event const&, Machine&)>
inline constexpr bool handle_event_impl(State& state, Event const& event, Machine&, m::m_tag_c<0>) noexcept(
    noexcept(handle_event(state, event)))
{
  return invoke_free_event_handler(SSM_HANDLE_EVENT_RETURNS_BOOL(f0, State&, Event const&), state, event);
}

#undef SSM_HANDLE_EVENT_REQUIRED
#undef SSM_HANDLE_EVENT_RETURNS_BOOL

template <typename State, typename Event, typename Machine>
using handle_event_detected_t =
    decltype(handle_event_impl(m::m_declval<State>(), m::m_declval<Event>(), m::m_declval<Machine>(), m::m_tag_c<3>{}));

template <typename State, typename Event, typename Machine>
using handle_event_is_nothrow_t = m::m_bool<noexcept(
    handle_event_impl(m::m_declval<State>(), m::m_declval<Event>(), m::m_declval<Machine>(), m::m_tag_c<3>{}))>;

struct handle_event_fn {

  template <
      typename State,
      typename Event,
      typename Machine,
      typename = m::m_if<m::m_is_valid<handle_event_detected_t, State&, Event const&, Machine&>, void>>
  constexpr bool operator()(State& state, Event const& event, Machine& machine) const
      noexcept(handle_event_is_nothrow_t<State&, Event const&, Machine&>{})
  {
    return handle_event_impl(state, event, machine, m::m_tag_c<3>{});
  }
};

}  // namespace handle_event_impl

template <typename State, typename Event, typename Machine>
struct is_event_handler : mp::m_is_valid<
                              handle_event_impl::handle_event_detected_t,
                              mp::m_remove_cvref<State>&,
                              mp::m_remove_cvref<Event> const&,
                              mp::m_remove_cvref<Machine>&> {
};

template <typename State, typename Event, typename Machine>
struct is_nothrow_event_handler : mp::m_eval_or<
                                      mp::m_false,
                                      handle_event_impl::handle_event_is_nothrow_t,
                                      mp::m_remove_cvref<State>&,
                                      mp::m_remove_cvref<Event> const&,
                                      mp::m_remove_cvref<Machine>&> {
};

inline namespace cpos
{

SSM_ANONYMOUS_NS_BEGIN

SSM_INLINE(17)
constexpr auto& handle_event = SSM_CPO_WRAP(handle_event_impl::handle_event_fn);

SSM_ANONYMOUS_NS_END

}  // namespace cpos

}  // namespace state_actions

}  // namespace sm

}  // namespace simple

#endif  // SSM_STATE_ACTIONS_HANDLE_EVENT_HPP
