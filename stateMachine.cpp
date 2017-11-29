#include <QDebug>

#include "stateMachine.h"
#include "logger.h"
#include "utils.h"

using namespace std::placeholders;

// Event

Event::Event(StateName targetState, StateName sourceState)
    : QEvent(QEvent::Type(targetState))
    , _sourceState(sourceState)
    , _targetState(targetState)
{
}

StateName Event::getTargetState() const
{
    return _targetState;
}

StateName Event::getSourceState() const
{
    return _sourceState;
}

// Transition

Transition::Transition(StateName sourceState, StateName targetState, std::function<void (QEvent *)> callback)
    : _sourceState(sourceState)
    , _targetState(targetState)
    , _callback(callback)
{
}

bool Transition::eventTest(QEvent* e)
{
    return (e->type() == _targetState);
}

void Transition::onTransition(QEvent* event)
{
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::STATE_TRANSITION_BEGIN, 0, 0
        , QStringList({Utils::getStateNameNumber(_sourceState), Utils::getStateNameNumber(_targetState)}));

    _callback(event);

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::STATE_TRANSITION_END);
}

StateName Transition::getTargetStateName() const
{
    return _targetState;
}

StateName Transition::getSourceStateName() const
{
    return _sourceState;
}

// State

State::State(StateName name)
    : _name(name)
{
}

StateName State::getName() const
{
    return _name;
}

// StateMachine

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

            qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::STATE_CHANGED, 0, 0
                , QStringList({Utils::getStateNameNumber(_currentState)}));
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

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::STATE_MACHINE_START);

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
