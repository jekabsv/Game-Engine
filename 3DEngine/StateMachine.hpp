#pragma once

#include <memory>
#include <stack>
#include "InputManager.hpp"
#include "State.hpp"

namespace game
{
	typedef std::unique_ptr<State> StateRef;
	class StateMachine
	{
	public:
		StateMachine() {};
		~StateMachine() {};
		void AddState(StateRef newState, bool isReplacing);
		void RemoveState();
		void ProcessStateChanges();
		StateRef& GetActiveState();
	private:
		std::stack<StateRef> _states;
		StateRef _newState;
		InputManager inputManager;
		bool _isRemoving;
		bool _isAdding;
		bool _isreplacing;


	};
}