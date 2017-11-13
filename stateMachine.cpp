#include "stateMachine.h"

StateMachine::StateMachine(QObject *parent) : QObject(parent)
{
    // create QState class for each state
    for (int stateName = SPLASH_SCREEN; stateName <= NON_CRITICAL_ERROR; stateName++) {
        _states.insert(static_cast<StateName>(stateName), new QState());
    }

    // add all state to _stateMachine, _stateMachine takes ownership of state
    // so no need to manual delete
    for (QState* state : _states.values()) {
        _stateMachine.addState(state);
    }

    _stateMachine.setInitialState(_states[SPLASH_SCREEN]);
}

void StateMachine::addTransitions(const QList<TransitionPack>& transitions)
{
    // iterate over packs, sets target state for transitions and
    // add transitions to source state
    for (const TransitionPack pack : transitions) {
        QState* state = _states[pack.getSource()];
        Transition* transition = pack.getTransition();

        transition->setTargetState(_states[pack.getTarget()]);
        state->addTransition(transition);
    }
}

void StateMachine::run()
{
    _stateMachine.start();
}

void StateMachine::postEvent(Event *event)
{
    _stateMachine.postEvent(event);
}
