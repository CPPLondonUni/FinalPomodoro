
#include "pomodoro.h"

#include <sstream>
#include <iomanip>
#include <iostream>

using namespace CppLondonUni;
using namespace std::chrono;
using namespace std::chrono_literals;

Pomodoro::Pomodoro(const uint64_t divisor, QWidget* parent)
    : QWidget{parent},
      ui{new Ui::Pomodoro},
      timer{divisor, this}
{
    ui->setupUi(this);

    soundEffect.setSource(QUrl::fromLocalFile("Annoying_Alarm_Clock.wav"));
    soundEffect.setLoopCount(1);

    connect(&timer, &Timer::tick, this, &Pomodoro::onTimerTick);
    connect(&timer, &Timer::completed, this, &Pomodoro::onTimerCompleted);
    connect(&timer, &Timer::completed, &soundEffect, &QSoundEffect::play);
    connect(ui->btnControl, &QPushButton::clicked, &soundEffect, &QSoundEffect::stop);
}

void Pomodoro::on_btnControl_clicked()
{
    //TODO: check radio buttons instead and choose duration.
    timer.start(25min);
}

void Pomodoro::onTimerTick(std::chrono::seconds timeRemaining)
{
    formatTime(timeRemaining);
}

void Pomodoro::onTimerCompleted()
{
    soundEffect.play();
    std::cout << "Completed" << std::endl;
    formatTime(std::chrono::seconds{0});
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
