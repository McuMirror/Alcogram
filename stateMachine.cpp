#include "stateMachine.h"

using namespace std::placeholders;

StateMachine::StateMachine(QObject *parent)
    : QObject(parent)
{
    // create QState class for each state
    for (int stateName = SPLASH_SCREEN; stateName <= NON_CRITICAL_ERROR; stateName++) {
        StateName name = static_cast<StateName>(stateName);
        State* state = new State(name);

        _states.insert(name, state);
        QObject::connect(state, QState::entered, [=]{
            _currentState = state->getName();
        });
    }

    // add all state to _stateMachine, _stateMachine takes ownership of state
    // so no need to manual delete
    for (QState* state : _states.values()) {
        addErrorTransitions(static_cast<State*>(state));
        _stateMachine.addState(state);
    }

    _stateMachine.setInitialState(_states[SPLASH_SCREEN]);
}

void StateMachine::addTransitions(const QList<Transition*>& transitions)
{
    // iterate over transitions
    // sets target state for transition and add transition to source state
    for (Transition* t : transitions) {
        QState* state = _states[t->getSourceStateName()];

        t->setTargetState(_states[t->getTargetStateName()]);
        state->addTransition(t);
    }
}

void StateMachine::run()
{
    _currentState = NONE;
    _stateMachine.start();
}

void StateMachine::postEvent(Event *event)
{
    _stateMachine.postEvent(event);
}

StateName StateMachine::getCurrentStateName() const
{
    return _currentState;
}

void StateMachine::addErrorTransitions(State* state)
{
    // add transition to critical error state
    QState* errorState = _states[CRITICAL_ERROR];
    Transition* transition = new Transition(state->getName(), CRITICAL_ERROR
                                           , std::bind(&StateMachine::handleCriticalError, this, _1));

    transition->setTargetState(errorState);
    state->addTransition(transition);

    // add transition to non critical error state
    errorState = _states[NON_CRITICAL_ERROR];
    transition = new Transition(state->getName(), NON_CRITICAL_ERROR
                                , std::bind(&StateMachine::handleNonCriticalError, this, _1));

    transition->setTargetState(errorState);
    state->addTransition(transition);
}

void StateMachine::addErrorStatesTransitions()
{
    //QState* errorState = _states[CRITICAL_ERROR];
    //Transition* transition = new Transition(TO_SPLASH_SCREEN_ETERNAL_SLEEP_0_3)
}

void StateMachine::handleCriticalError(QEvent* event)
{
    // TODO: handle it
}

void StateMachine::handleNonCriticalError(QEvent* event)
{
    // TODO: handle it
}
