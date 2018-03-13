
#ifndef CPP_LONDON_UNI_POMODORO_H
#define CPP_LONDON_UNI_POMODORO_H

#include <QWidget>
#include <QMediaPlayer>
#include <functional>
#include <memory>

#include "timer.h"
#include "ui_pomodoro.h"

namespace CppLondonUni {

class Pomodoro : public QWidget {

    enum class TimerState {
        POMODORO,
        SHORT_BREAK,
        LONG_BREAK
    };

    Q_OBJECT

public:
    explicit Pomodoro(const uint64_t divisor = 1, QWidget* parent = nullptr);

private slots:
    // N.B. This slot is activated when the user clicks the "start" button in the UI
    void on_btnControl_clicked();

    void onTimerTick(std::chrono::seconds timeRemaining);
    void onTimerCompleted();

private:
    void formatTime(std::chrono::seconds remaining);
    void onStartClicked();
    void onPauseClicked();
    void onResumeClicked(const std::chrono::seconds&);
    void setControlStates(bool);

    std::unique_ptr<Ui::Pomodoro> ui;
    Timer timer;
    QMediaPlayer soundEffect;
    TimerState currentState;
    uint32_t pomodoroCount{0};
    std::function<void()> btnStateDispatcher;
};


}

#endif
