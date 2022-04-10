#ifndef SIMPLE_SM_SM_WE_HPP
#define SIMPLE_SM_SM_WE_HPP

#include <simple/sm/config.hpp>
#include <simple/sm/state_actions.hpp>

#include <simple/mp.hpp>

#include <tuple>

namespace simple
{

namespace sm
{

template <typename... Ts>
using state_list = mp::m_list<Ts...>;

template <typename States>
class sm_woe;

template <typename... States>
class sm_woe<state_list<States...>>
{

  using states_t = mp::m_make_unique<state_list<States...>>;

  static_assert(mp::m_size<states_t>::value > 0, "state list must not be empty");

  template <typename State>
  using state_index = mp::m_find<states_t, State>;

  template <typename State>
  using is_valid_state = mp::m_not<mp::m_same<state_index<State>, mp::m_size<State>>>;

  template <typename State>
  using can_enter = state_actions::is_enterable<State, sm_woe>;

  template <typename State>
  using can_exit = state_actions::is_exitable<State, sm_woe>;

  template <typename State, typename Event>
  using can_handle_event = state_actions::is_event_handler<State, Event, sm_woe>;

  template <typename State>
  using can_update = state_actions::is_updatable<State, sm_woe>;

  using m_false = mp::m_false;

  using m_true = mp::m_true;

  template <typename State>
  using can_enter_nothrow = mp::m_eval_if<can_enter<State>, m_true, state_actions::is_nothrow_enterable, State, sm_woe>;

public:

  constexpr sm_woe() noexcept(
      mp::m_all<std::is_nothrow_default_constructible<States>..., can_enter_nothrow<mp::m_first<states_t>>>{})
    : idx_{0}, states_{}
  {
    enter_state<mp::m_first<states_t>>();
  }

  template <typename State>
  constexpr void set_state()
  {
    static_assert(is_valid_state<State>{}, "not a valid sm state - all states should be put in the state list");
    exit_state();          // if this throws the sm remains in its current state
    enter_state<State>();  // if this throws the sm has entered the new state
  }

private:

  template <typename State>
  constexpr void do_enter_state(m_true, State& state)
  {
    state_actions::enter(state, *this);
  }

  template <typename State>
  constexpr void do_enter_state(m_false, State& state)
  {
  }

  template <typename State>
  constexpr void enter_state() noexcept(can_enter_nothrow<State>{})
  {
    constexpr auto I = state_index<State>::value;
    idx_ = I;
    do_enter_state(can_enter<State>{}, std::get<I>(states_));
  }

  template <typename State>
  constexpr void do_exit_state(m_true, State& state)
  {
    state_actions::exit(state, *this);
  }

  template <typename State>
  constexpr void do_exit_state(m_false, State& state)
  {
  }

  struct exit_state_idx_fn {
    sm_woe* this_;
    template <typename I>
    constexpr void operator()(I)
    {
      using state_type = mp::m_at<states_t, I>;
      this_->do_exit_state(can_enter<state_type>{}, std::get<I::value>(this_->states_));
    }
  };

  constexpr void exit_state() { mp::m_vtable_invoke<sizeof...(States)>(idx_, exit_state_idx_fn{this}); }

  using state_tuple = mp::m_apply<std::tuple, states_t>;

  std::size_t idx_;
  state_tuple states_;
};

}  // namespace sm

}  // namespace simple

#endif  // SIMPLE_SM_SM_WE_HPP
