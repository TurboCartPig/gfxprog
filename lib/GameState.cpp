#include <glove/GameState.h>
#include <utility>

Core::Core(std::unique_ptr<IGameState> initial_state) {
	m_gamestates.push_back(std::move(initial_state));
}

void Core::run() {
	m_window   = std::make_unique<Window>("Change Me", 1280, 720);
	auto input = m_window->getInputQueue();

	// Initialize all the game states
	for (auto &state : m_gamestates) {
		state->initialize();
	}

	auto prev_time_point = std::chrono::steady_clock::now();

	// Main game loop
	while (!m_window->pollEvents()) {
		/*
		 * # Assumptions for this function.
		 * - I assume that there is a game state on the stack
		 *
		 * # Argumentation for using gotos.
		 * - The main problem that calls for gotos is that I need to break an
		 * inner loop.
		 * - The input loop has to be a nested loop within the
		 * gameloop as I see it.
		 * - The behavior for Popping, pushing and
		 * transitioning is shared and needs to be able to break and continue
		 * the gameloop.
		 */

		StateTransition trans;
		const auto      current = m_gamestates.size() - 1;

		const auto now  = std::chrono::steady_clock::now();
		const auto dt   = (now - prev_time_point).count() / 1000000000.0f;
		prev_time_point = now;

		while (!input->empty()) {
			auto key = input->back();
			input->pop_back();
			trans = m_gamestates[current]->input(key);
			if (std::holds_alternative<Pop>(trans)) {
				goto pop_state;
			} else if (std::holds_alternative<Push>(trans)) {
				goto push_state;
			} else if (std::holds_alternative<Transition>(trans)) {
				goto transition_state;
			} // Else None -> nop
		}

		trans = m_gamestates[current]->update(dt);
		if (std::holds_alternative<Pop>(trans)) {
			goto pop_state;
		} else if (std::holds_alternative<Push>(trans)) {
			goto push_state;
		} else if (std::holds_alternative<Transition>(trans)) {
			goto transition_state;
		} // Else None -> nop

		m_gamestates[current]->render();

		m_window->swapBuffers();

		continue;

	pop_state:
		m_gamestates.pop_back();
		if (m_gamestates.empty())
			break;
		else
			continue;

	push_state:
		m_gamestates.push_back(std::move(std::get<Push>(trans).state));
		continue;

	transition_state:
		m_gamestates[current].reset(
		    std::get<Transition>(trans).state.release());
		continue;
	}
}
