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
    SPLASH_SCREEN = 0
    , START
    , SPLASH_SCREEN_ETERNAL_SLEEP
    , SPLASH_SCREEN_NONCRITICAL_ERROR
    , PREPARING_FOR_PHOTO
    , PHOTO_TIMER
    , PHOTO_CONFIRMATION
    , PAY
    , NOT_ENOUGH_MONEY
    , MORE_MONEY_THAN_NEED
    , PAYMENT_CONFIRMED
    , ALCOTEST
    , DRUNKENESS_NOT_RECOGNIZED
    , ALCOTEST_INACTION
    , FINAL_PHOTO
    , PHOTO_PRINT
    , CRITICAL_ERROR
    , NON_CRITICAL_ERROR
};

// state machine transition names
enum TransitionName {
    TO_SPLASH_SCREEN_0_1 = QEvent::User + 1
    , TO_START_0_2 = QEvent::User + 2
    , TO_PREPARING_FOR_PHOTO_1_1 = QEvent::User + 3
    , TO_PHOTO_TIMER_1_2 = QEvent::User + 4
    , TO_PHOTO_CONFIRMATION_1_2 = QEvent::User + 5
    , TO_PAY_2_1 = QEvent::User + 6
    , TO_NOT_ENOUGH_MONEY_2_1_1 = QEvent::User + 7
    , TO_MORE_MONEY_THAN_NEED_2_1_2 = QEvent::User + 8
    , TO_PAYMENT_CONFIRMED_2_2 = QEvent::User + 9
    , TO_ALCOTEST_3_1 = QEvent::User + 10
    , TO_DRUNKENESS_NOT_RECOGNIZED_3_1_1 = QEvent::User + 11
    , TO_ALCOTEST_INACTIVITY_3_1_2 = QEvent::User + 12
    , TO_FINAL_FOTO_4_1 = QEvent::User + 13
    , TO_PRINT_PHOTO_4_2 = QEvent::User + 14
    , TO_CRITICAL_ERROR_5_1 = QEvent::User + 15
    , TO_NONCRITICAL_ERROR_5_2 = QEvent::User + 16
    , TO_SPLASH_SCREEN_ETERNAL_SLEEP_0_3 = QEvent::User + 17
    , TO_SPLASH_SCREEN_NONCRITICAL_ERROR_0_4 = QEvent::User + 18
};

// state machine event
class Event : public QEvent
{
public:
    // @param
    explicit Event(TransitionName type, StateName fromState)
        : QEvent(QEvent::Type(type))
        , _fromState(fromState)
    {
    }

    StateName getFromState() const
    {
        return _fromState;
    }

private:
    StateName _fromState;
};

// state machine transition
class Transition : public QAbstractTransition
{
public:
    // @param transition - transition name
    // @param callback - function called on transition
    explicit Transition(TransitionName transition, std::function<void(void)> callback)
    {
        _transition = transition;
        _callback = callback;
    }

    bool eventTest(QEvent *e) override
    {
        return (e->type() == _transition);
    }

    void onTransition(QEvent *event) override
    {
        _callback();
    }

private:
    TransitionName _transition;
    std::function<void(void)> _callback;
};

// used by pages to pass state transition to state machine
class TransitionPack {
public:
    // @param source - name of source state
    // @param target - name of target state
    TransitionPack(Transition* transition, StateName source, StateName target)
    {
        _transition = transition;
        _source = source;
        _target = target;
    }

    Transition* getTransition() const
    {
        return _transition;
    }

    StateName getSource() const
    {
        return _source;
    }

    StateName getTarget() const
    {
        return _target;
    }

private:
    Transition* _transition;
    StateName _source;
    StateName _target;
};

// application state machine
class StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit StateMachine(QObject *parent = 0);

    // add transitions to state machine
    void addTransitions(const QList<TransitionPack>& transitions);

    // run state machine
    void run();

    // post event to state machine
    void postEvent(Event* event);

private:
    QStateMachine _stateMachine;
    QMap<StateName, QState*> _states; // all application states (no need to delete QState's because _stateMachine
                                      // takes ownership of this QState's
};

