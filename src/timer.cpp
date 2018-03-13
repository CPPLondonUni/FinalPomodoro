
#include "timer.h"

#include <QDebug>

#include <iostream>

using namespace CppLondonUni;
using namespace std::chrono_literals;

Timer::Timer(const uint64_t divisor, QObject* parent)
    : QObject{parent},
      divisor{divisor}
{
    connect(&qTimer, &QTimer::timeout, this, &Timer::onIntervalTimeout);
}

void Timer::start(std::chrono::seconds timeout)
{
    this->remaining = timeout;
    auto singleTick = 1000ms / divisor;
    std::cerr << "ticking once per: " << singleTick.count() << " - divisor: " << divisor << std::endl;
    qTimer.start(singleTick.count());
    emit(tick(timeout));
}

void Timer::stop()
{
    this->remaining = 0s;
    qTimer.stop();
}

std::chrono::seconds Timer::GetTimeRemaining() const {
    return this->remaining;
}

void Timer::onIntervalTimeout()
{
    --remaining;
    if (remaining == 0s) {
        qTimer.stop();
        emit(completed());
    } else {
        emit(tick(remaining));
    }
}
