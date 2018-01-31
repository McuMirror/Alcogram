#pragma once

#include <QObject>
#include <QEvent>
#include <QStateMachine>
#include <QState>
#include <QAbstractTransition>
#include <QMap>
#include <functional>

// state names
enum StateName {
    SPLASH_SCREEN = QEvent::User + 1
    , START = QEvent::User + 2
    , SPLASH_SCREEN_ETERNAL_SLEEP = QEvent::User + 3
    , SPLASH_SCREEN_NONCRITICAL_ERROR = QEvent::User + 4
    , PREPARING_FOR_PHOTO = QEvent::User + 5
    , PHOTO_TIMER = QEvent::User + 6
    , PHOTO_CONFIRMATION = QEvent::User + 7
    , PAY = QEvent::User + 8
    , NOT_ENOUGH_MONEY = QEvent::User + 9
    , MORE_MONEY_THAN_NEED = QEvent::User + 10
    , PAYMENT_CONFIRMED = QEvent::User + 11
    , ALCOTEST = QEvent::User + 12
    , DRUNKENESS_NOT_RECOGNIZED = QEvent::User + 13
    , ALCOTEST_INACTION = QEvent::User + 14
    , FINAL_PHOTO = QEvent::User + 15
    , PHOTO_PRINT = QEvent::User + 16
    , CRITICAL_ERROR = QEvent::User + 17
    , NON_CRITICAL_ERROR = QEvent::User + 18
    , NONE = QEvent::User + 19
};

// state machine event
class Event : public QEvent
{
public:
    explicit Event(StateName targetState, StateName sourceState);

    StateName getTargetState() const;

    StateName getSourceState() const;

private:
    StateName _sourceState;
    StateName _targetState;
};

// state machine transition
class Transition : public QAbstractTransition
{
public:
    // @param transition - transition name
    // @param callback - function called on transition
    explicit Transition(StateName sourceState, StateName targetState, std::function<void(QEvent*)> callback);

    bool eventTest(QEvent* e) override;

    void onTransition(QEvent* event) override;

    StateName getTargetStateName() const;

    StateName getSourceStateName() const;

private:
    StateName _sourceState;
    StateName _targetState;
    std::function<void(QEvent*)> _callback;
};

// state
class State : public QState
{
public:
    State(StateName name);

    StateName getName() const;

private:
    StateName _name;
};

// application state machine
class StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit StateMachine(QObject *parent = 0);

    // add transitions to state machine
    void addTransitions(const QList<Transition*>& transitions);

    // run state machine
    void run();

    // post event to state machine
    void postEvent(Event* event);

    StateName getCurrentStateName() const;

signals:
    void criticalError(StateName fromState);

private:
    // adding transitions to critical error and non critical error states
    void addErrorTransitions(State* state);

    void addErrorStatesTransitions();
    void handleCriticalError(QEvent* event);
    void handleNonCriticalError(QEvent* event);

    QStateMachine _stateMachine;
    QMap<StateName, QState*> _states; // all application states (no need to delete QState's because _stateMachine
                                      // takes ownership of this QState's
    StateName _currentState;
};

