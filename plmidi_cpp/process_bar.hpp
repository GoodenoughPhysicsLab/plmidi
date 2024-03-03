#pragma once

#include <chrono>
#include <cstring>

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
        if (this->_status < this->_length) {
            this->_now_time = stdtime::system_clock::to_time_t(stdtime::system_clock::now()) - this->_start_time;
            this->_status = static_cast<int>(this->_now_time / this->_unit_time);
        }
    }

    void print() const {
        char print_cache[this->_length + 1];
        memset(print_cache, '-', this->_status);
        memset(print_cache + this->_status, ' ', this->_length - this->_status);

        print_cache[this->_length] = '\0';
        auto time = this->_now_time > this->_midi_duration ? this->_midi_duration : this->_now_time;
        ::std::printf("%s %lld/%d\r", print_cache, time, this->_midi_duration);
    }
};

} // namespace plmidi::process_bar