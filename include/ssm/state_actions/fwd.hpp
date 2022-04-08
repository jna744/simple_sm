#ifndef SSM_STATE_ACTIONS_FWD_HPP
#define SSM_STATE_ACTIONS_FWD_HPP

#include <ssm/config.hpp>

namespace ssm
{

namespace state_actions
{

namespace detail
{

struct dummy_machine {
};

}  // namespace detail

template <typename State, typename Machine = detail::dummy_machine>
struct is_enterable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_enterable_v = is_enterable<State, Machine>::value;

template <typename State, typename Machine = detail::dummy_machine>
struct is_nothrow_enterable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_nothrow_enterable_v = is_nothrow_enterable<State, Machine>::value;

template <typename State, typename Machine = detail::dummy_machine>
struct is_exitable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_exitable_v = is_exitable<State, Machine>::value;

template <typename State, typename Machine = detail::dummy_machine>
struct is_nothrow_exitable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_nothrow_exitable_v = is_nothrow_exitable<State, Machine>::value;

template <typename State, typename Event, typename Machine = detail::dummy_machine>
struct is_event_handler;

template <typename State, typename Event, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_event_handler_v = is_event_handler<State, Event, Machine>::value;

template <typename State, typename Event, typename Machine = detail::dummy_machine>
struct is_nothrow_event_handler;

template <typename State, typename Event, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_nothrow_event_handler_v = is_nothrow_event_handler<State, Event, Machine>::value;

template <typename State, typename Machine = detail::dummy_machine>
struct is_updatable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_updatable_v = is_updatable<State, Machine>::value;

template <typename State, typename Machine = detail::dummy_machine>
struct is_nothrow_updatable;

template <typename State, typename Machine = detail::dummy_machine>
SSM_INLINE(17)
constexpr auto is_nothrow_updatable_v = is_nothrow_updatable<State, Machine>::value;

}  // namespace state_actions

}  // namespace ssm

#endif  // SSM_STATE_ACTIONS_FWD_HPP
