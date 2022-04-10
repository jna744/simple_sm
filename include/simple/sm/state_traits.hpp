#ifndef SIMPLE_SM_STATE_TRAITS_HPP
#define SIMPLE_SM_STATE_TRAITS_HPP

#include <simple/sm/config.hpp>
#include <simple/sm/state_actions.hpp>

#include <simple/mp.hpp>

namespace simple
{

namespace sm
{

template <typename State, typename Machine>
struct state_traits {

  using is_enterable = state_actions::is_enterable<State, Machine>;

  using is_exitable = state_actions::is_exitable<State, Machine>;

  template <typename Event>
  using is_event_handler = state_actions::is_event_handler<State, Event, Machine>;

  using is_updatable = state_actions::is_updatable<State, Machine>;

  constexpr static void
  enter(State& state, Machine& machine) noexcept(noexcept(do_enter(is_enterable{}, state, machine)))
  {
    do_enter(is_enterable{}, state, machine);
  }

  using is_nothrow_enterable = mp::m_bool<noexcept(enter(mp::m_declval<State&>(), mp::m_declval<Machine&>()))>;

  constexpr static void exit(State& state, Machine& machine) noexcept(noexcept(do_exit(is_exitable{}, state, machine)))
  {
    do_exit(is_exitable{}, state, machine);
  }

  using is_nothrow_exitable = mp::m_bool<noexcept(exit(mp::m_declval<State&>(), mp::m_declval<Machine&>()))>;

  template <typename Event>
  constexpr static bool handle_event(State& state, Event const& event, Machine& machine) noexcept(
      noexcept(do_handle_event(is_event_handler<Event>{}, state, event, machine)))
  {
    return do_handle_event(is_event_handler<Event>{}, state, event, machine);
  }

  template <typename Event>
  using is_nothrow_event_handler = mp::m_bool<noexcept(
      handle_event(mp::m_declval<State&>(), mp::m_declval<Event const&>(), mp::m_declval<Machine&>()))>;

  constexpr static void
  update(State& state, Machine& machine) noexcept(noexcept(do_update(is_updatable{}, state, machine)))
  {
    do_update(is_updatable{}, state, machine);
  }

  using is_nothrow_updatable = mp::m_bool<noexcept(update(mp::m_declval<State&>(), mp::m_declval<Machine&>()))>;

private:

  constexpr static void do_enter(mp::m_false, State& state, Machine& machine) noexcept {}

  constexpr static void
  do_enter(mp::m_true, State& state, Machine& machine) noexcept(state_actions::is_nothrow_enterable_v<State, Machine>)
  {
    state_actions::enter(state, machine);
  }

  constexpr static void do_exit(mp::m_false, State& state, Machine& machine) noexcept {}

  constexpr static void
  do_exit(mp::m_true, State& state, Machine& machine) noexcept(state_actions::is_nothrow_exitable_v<State, Machine>)
  {
    state_actions::exit(state, machine);
  }

  template <typename Event>
  constexpr static bool do_handle_event(mp::m_false, State&, Event const&, Machine&) noexcept
  {
    return true;
  }

  template <typename Event>
  constexpr static void do_handle_event(mp::m_true, State& state, Event const& event, Machine& machine) noexcept(
      state_actions::is_nothrow_event_handler_v<State, Event, Machine>)
  {
    return state_actions::handle_event(state, event, machine);
  }

  constexpr static void do_update(mp::m_false, State&, Machine&) noexcept {}

  constexpr static void
  do_update(mp::m_true, State& state, Machine& machine) noexcept(state_actions::is_nothrow_updatable_v<State, Machine>)
  {
    state_actions::update(state, machine);
  }
};

}  // namespace sm

}  // namespace simple

#endif  // SSM_STATE_TRAITS_HPP
