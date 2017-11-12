#pragma once

#include <QObject>
#include <QEvent>
#include <QStateMachine>
#include <QState>
#include <QAbstractTransition>
#include <QMap>

// application states
enum StateName {
    SPLASH_SCREEN
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

// application state machine transitions
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

class Event : public QEvent
{
public:
    explicit Event(TransitionName type)
        : QEvent(QEvent::Type(type)){}
};

class Transition : public QAbstractTransition
{
public:
    explicit Transition(TransitionName transition)
    {
        _transition = transition;
    }

    bool eventTest(QEvent *e) override
    {
        return (e->type() == _transition);
    }


private:
    TransitionName _transition;
};

// application state machine
class StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit StateMachine(QObject *parent = 0);

private:
    QStateMachine _stateMachine;
    QMap<StateName, QState*> states; // all application states (no need to delete QState's because _stateMachine
                                 // takes ownership of this QState's
};

