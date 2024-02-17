#pragma once

#include <chrono>

namespace stdtime = ::std::chrono;

namespace plmidi::process_bar
{

class MidiProcessBar {
    static constexpr int _length = 50;
    const time_t _start_time = stdtime::system_clock::to_time_t(stdtime::system_clock::now());
    int _status = 0;
    time_t _now_time = 0;
    int _midi_duration = 0;
    float _unit_time = 0;
    char print_cache[_length + 1];
public:
    MidiProcessBar(float midi_duration) {
        this->_unit_time = midi_duration / this->_length;
        this->_midi_duration = static_cast<int>(midi_duration);
    }

    ~MidiProcessBar() {
        ::std::puts("\n");
    }

    bool is_not_end() const {
        return this->_status <= this->_length;
    }

    int status() const {
        return this->_status;
    }

    int length() const {
        return this->_length;
    }

    void update() {
        this->_now_time = stdtime::system_clock::to_time_t(stdtime::system_clock::now()) - this->_start_time;
        this->_status = static_cast<int>(this->_now_time / this->_unit_time);
    }

    void print() {
        int i{0};
        for (; i < this->_status; ++i) {
            this->print_cache[i] = '-';
        }
        for (; i < this->_length; ++i) {
            this->print_cache[i] = ' ';
        }
        this->print_cache[this->_length] = '\0';
        ::std::printf("%s %lld/%d\r", this->print_cache, this->_now_time, this->_midi_duration);
    }
};

} // namespace plmidi::process_bar