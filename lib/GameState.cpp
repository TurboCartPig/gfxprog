#include <glove/GameState.h>
#include <utility>

Core::Core(std::unique_ptr<IGameState> initial_state) {
	m_gamestates.push_back(std::move(initial_state));

	// Create the window
	m_window = std::make_unique<Window>("Change Me", 1280, 720);

	// Register callback that passes control to current game state.
	m_window->registerFramebufferResizeCallback(
	    [&]([[maybe_unused]] GLFWwindow *window, int width, int height) {
		    m_gamestates.back()->resized(width, height);
	    });
}

void Core::run() {
	auto input = m_window->getInputQueue();

	// Setup the initial state
	setupState();

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
		setupState();
		continue;

	transition_state:
		m_gamestates[current] = std::move(std::get<Transition>(trans).state);
		setupState();
		continue;
	}
}

void Core::setupState() {
	auto &top = m_gamestates.back();

	// Initialize the state assuming it is new
	top->initialize();

	// Set properties from the manifest
	const auto manifest = top->manifest();
	m_window->setTitle(manifest.title);
}
