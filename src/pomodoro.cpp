#include "pomodoro.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include <QFileInfo>

using namespace CppLondonUni;
using namespace std::chrono;
using namespace std::chrono_literals;

Pomodoro::Pomodoro(const uint64_t divisor, QWidget* parent)
    : QWidget{parent},
      ui{new Ui::Pomodoro},
      timer{divisor, this},
      btnStateDispatcher{[this] { this->onStartClicked(); }}
{
    ui->setupUi(this);

    connect(&timer, &Timer::tick, this, &Pomodoro::onTimerTick);
    connect(&timer, &Timer::completed, this, &Pomodoro::onTimerCompleted);
    connect(&timer, &Timer::completed, &soundEffect, &QMediaPlayer::play);
    connect(ui->btnControl, &QPushButton::clicked, &soundEffect, &QMediaPlayer::stop);
}

void Pomodoro::setControlStates(bool isEnabled) {
    ui->opt_pomodoro->setEnabled(isEnabled);
    ui->opt_shortbreak->setEnabled(isEnabled);
    ui->opt_longbreak->setEnabled(isEnabled);
    ui->breakInterval->setEnabled(isEnabled);
    ui->time_pomodoro->setEnabled(isEnabled);
    ui->time_short->setEnabled(isEnabled);
    ui->time_long->setEnabled(isEnabled);
}

static void SetMedia(QMediaPlayer& soundEffect, const char* file) {
    soundEffect.setMedia(QUrl::fromLocalFile(QFileInfo{file}.absoluteFilePath()));
}

void Pomodoro::onStartClicked() {
    if (ui->opt_pomodoro->isChecked()) {
        currentState = TimerState::POMODORO;
        SetMedia(soundEffect, "bensound-acousticbreeze.wav");
        timer.start(std::chrono::minutes{ui->time_pomodoro->time().minute()});
    } else if (ui->opt_shortbreak->isChecked()) {
        SetMedia(soundEffect, "Annoying_Alarm_Clock.wav");
        currentState = TimerState::SHORT_BREAK;
        timer.start(std::chrono::minutes{ui->time_short->time().minute()});
    } else if (ui->opt_longbreak->isChecked()) {
        SetMedia(soundEffect, "Annoying_Alarm_Clock.wav");
        currentState = TimerState::LONG_BREAK;
        timer.start(std::chrono::minutes{ui->time_long->time().minute()});
    }
    setControlStates(false);
    ui->btnControl->setText("Pause");
    btnStateDispatcher = [this] { this->onPauseClicked(); };
}

void Pomodoro::onPauseClicked() {
    auto timeRemaining = timer.GetTimeRemaining();
    timer.stop();
    ui->btnControl->setText("Resume");
    btnStateDispatcher = [this, timeRemaining] { this->onResumeClicked(timeRemaining); };
}

void Pomodoro::onResumeClicked(const std::chrono::seconds& remaining) {
    timer.start(remaining);
    ui->btnControl->setText("Pause");
    btnStateDispatcher = [this] { this->onPauseClicked(); };
}

void Pomodoro::on_btnControl_clicked()
{
    btnStateDispatcher();
}

void Pomodoro::onTimerTick(std::chrono::seconds timeRemaining)
{
    formatTime(timeRemaining);
}

void Pomodoro::onTimerCompleted()
{
    std::cout << "Completed" << std::endl;
    switch (currentState) {
        case TimerState::POMODORO:
            if (++pomodoroCount % (ui->breakInterval->value() + 1) == 0) {
                ui->opt_longbreak->setChecked(true);
            } else {
                ui->opt_shortbreak->setChecked(true);
            }
            break;
        case TimerState::SHORT_BREAK:
        case TimerState::LONG_BREAK:
            ui->opt_pomodoro->setChecked(true);
            break;
    }
    ui->btnControl->setText("Start");
    btnStateDispatcher = [this] { this->onStartClicked(); };
    formatTime(std::chrono::seconds{0});
    //setControlStates(true);
    //Did this hack because the stupid product manager wanted it automatic.
    on_btnControl_clicked();
}

void Pomodoro::formatTime(std::chrono::seconds remaining)
{
    auto secs = remaining.count();
    const auto mins = secs / 60;
    secs = secs % 60;

    std::ostringstream ss;
    ss << mins << ':' << std::setw(2) << std::setfill('0') << secs;

    ui->curTime->setText(ss.str().c_str());
}
