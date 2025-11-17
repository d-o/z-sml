/*
 * Copyright (c) 2025 Dean Sellers (dean@sellers.id.au)
 * SPDX-License-Identifier: MIT
 */

#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <vector>
#include <variant>

/* Plantuml for the dimmable light
    [*] -> off
    off -> on : e_ON /bright=5
    off -> on : e_UP /last_bright?bright=last_bright:bright=1
    on -> on : e_UP [bright != 5]/bright+=1
    on -> off : e_OFF /bright=0
    on -> on : e_DN [bright > 1]/bright-=1
    on -> off : e_DN [bright == 1]
    off: entry bright=0
    on: exit last_bright=bright
*/

namespace sml = boost::sml;

// Four events
namespace {
	struct e_ON { static constexpr std::string_view name = "On"; };
	struct e_UP { static constexpr std::string_view name = "Up"; };
	struct e_DN { static constexpr std::string_view name = "Down"; };
	struct e_OFF { static constexpr std::string_view name = "Off"; };

// Data for each light context
struct lt_ctx {
	int bright = 100000;
	int last_bright = 0;
};

struct dimmable_machine {

    auto operator()() const noexcept {
	using namespace sml;

	// clang-format off
	return make_transition_table(
	 *"Off"_s + event<e_ON> / [](lt_ctx& lc){ lc.bright = 5; } = "On"_s
	, "Off"_s + event<e_UP> / [](lt_ctx& lc){ lc.bright = lc.last_bright?lc.last_bright:1;} = "On"_s
	, "On"_s + event<e_UP> [!![](lt_ctx& lc){return lc.bright < 5; }] / [](lt_ctx& lc){ lc.bright++; }
	, "On"_s + event<e_DN> [!![](lt_ctx& lc){return lc.bright > 1; }] / [](lt_ctx& lc){ lc.bright--; }
	, "On"_s + event<e_DN> [!![](lt_ctx& lc){return lc.bright == 1; }] / [] {} = "Off"_s
	, "On"_s + event<e_OFF> / [] {} = "Off"_s
	, "Off"_s + sml::on_entry<_> / [](lt_ctx& lc){ lc.bright = 0; }
	, "On"_s + sml::on_exit<_> / [](lt_ctx& lc){ lc.last_bright = lc.bright; }
	);
	// clang-format on
    }
};
}  // namespace

int main() {
	using namespace std;
	// Event instances
	e_ON ON;
	e_OFF OFF;
	e_UP UP;
	e_DN DN;

	// A vector of events to run
	vector< variant<e_ON, e_OFF, e_UP, e_DN> > evts = {
		ON, OFF, UP, /* 5, 0, 5 */
		DN, DN, DN, /* 4, 3, 2 */
		OFF, UP, UP, UP, UP, UP, /* 0, 2, 3, 4, 5, 5 */
		DN, DN, DN, DN, DN, DN, /* 4, 3, 2, 1, 0, 0 */
		UP, OFF, ON, OFF /* 1, 0, 5, 0*/
    	};

    	// Context data for a dimmable light
    	lt_ctx ct{};
    	//Create a light with the context data
    	sml::sm<dimmable_machine> sm{ct};

    	// Run through the states ...
    	sm.visit_current_states([](auto state) { cout << "Light started @ " << state.c_str() << std::endl; });
    	for(auto& v: evts) {
		visit([&sm](const auto& e){sm.process_event(e); cout << "Event \"" << e.name; }, v);
		cout << "\"\tLevel " << ct.bright << "\tState ";
		sm.visit_current_states([](auto state) { cout << state.c_str() << std::endl; });
    	}
}
