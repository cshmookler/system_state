// Standard includes
#include <iostream>
#include <thread>

// External includes
#include "../system_state/core.hpp"

int get_sound() {
    std::cout << std::boolalpha; // Pretty print boolean values.

    auto mixer = syst::sound_mixer_t::get();
    if (mixer.has_error()) {
        std::cerr << mixer.error() << std::endl;
        return 1;
    }

    auto controls = mixer->all_controls();

    for (auto& control : controls) {
        std::string name = control.name();
        std::cout << "Name: " << name << std::endl;

        if (control.has_playback_status()) {
            auto playback_status = control.get_playback_status();
            if (playback_status.has_value()) {
                std::cout << "Playback Status:";
                if (playback_status->front_left.has_value()) {
                    std::cout << "\n\tFront Left: "
                              << playback_status->front_left.value();
                }
                if (playback_status->front_right.has_value()) {
                    std::cout << "\n\tFront Right: "
                              << playback_status->front_right.value();
                }
                if (playback_status->rear_left.has_value()) {
                    std::cout << "\n\tRear Left: "
                              << playback_status->rear_left.value();
                }
                if (playback_status->rear_right.has_value()) {
                    std::cout << "\n\tRear Right: "
                              << playback_status->rear_right.value();
                }
                if (playback_status->front_center.has_value()) {
                    std::cout << "\n\tFront Center: "
                              << playback_status->front_center.value();
                }
                if (playback_status->woofer.has_value()) {
                    std::cout << "\n\tWoofer: "
                              << playback_status->woofer.value();
                }
                if (playback_status->side_left.has_value()) {
                    std::cout << "\n\tSide Left: "
                              << playback_status->side_left.value();
                }
                if (playback_status->side_right.has_value()) {
                    std::cout << "\n\tSide Right: "
                              << playback_status->side_right.value();
                }
                if (playback_status->rear_center.has_value()) {
                    std::cout << "\n\tRear Center: "
                              << playback_status->rear_center.value();
                }
                std::cout << std::endl;
            } else {
                std::cerr << playback_status.error() << std::endl;
            }
        }

        if (control.has_playback_volume()) {
            auto playback_volume = control.get_playback_volume();
            if (playback_volume.has_value()) {
                std::cout << "Playback Volume:";
                if (playback_volume->front_left.has_value()) {
                    std::cout << "\n\tFront Left: "
                              << playback_volume->front_left.value() << "%";
                }
                if (playback_volume->front_right.has_value()) {
                    std::cout << "\n\tFront Right: "
                              << playback_volume->front_right.value() << "%";
                }
                if (playback_volume->rear_left.has_value()) {
                    std::cout
                      << "\n\tRear Left: " << playback_volume->rear_left.value()
                      << "%";
                }
                if (playback_volume->rear_right.has_value()) {
                    std::cout << "\n\tRear Right: "
                              << playback_volume->rear_right.value() << "%";
                }
                if (playback_volume->front_center.has_value()) {
                    std::cout << "\n\tFront Center: "
                              << playback_volume->front_center.value() << "%";
                }
                if (playback_volume->woofer.has_value()) {
                    std::cout
                      << "\n\tWoofer: " << playback_volume->woofer.value()
                      << "%";
                }
                if (playback_volume->side_left.has_value()) {
                    std::cout
                      << "\n\tSide Left: " << playback_volume->side_left.value()
                      << "%";
                }
                if (playback_volume->side_right.has_value()) {
                    std::cout << "\n\tSide Right: "
                              << playback_volume->side_right.value() << "%";
                }
                if (playback_volume->rear_center.has_value()) {
                    std::cout << "\n\tRear Center: "
                              << playback_volume->rear_center.value() << "%";
                }
                std::cout << std::endl;
            } else {
                std::cerr << playback_volume.error() << std::endl;
            }
        }

        if (control.has_capture_status()) {
            auto capture_status = control.get_capture_status();
            if (capture_status.has_value()) {
                std::cout << "Capture Status:";
                if (capture_status->front_left.has_value()) {
                    std::cout << "\n\tFront Left: "
                              << capture_status->front_left.value();
                }
                if (capture_status->front_right.has_value()) {
                    std::cout << "\n\tFront Right: "
                              << capture_status->front_right.value();
                }
                if (capture_status->rear_left.has_value()) {
                    std::cout << "\n\tRear Left: "
                              << capture_status->rear_left.value();
                }
                if (capture_status->rear_right.has_value()) {
                    std::cout << "\n\tRear Right: "
                              << capture_status->rear_right.value();
                }
                if (capture_status->front_center.has_value()) {
                    std::cout << "\n\tFront Center: "
                              << capture_status->front_center.value();
                }
                if (capture_status->woofer.has_value()) {
                    std::cout << "\n\tWoofer: "
                              << capture_status->woofer.value();
                }
                if (capture_status->side_left.has_value()) {
                    std::cout << "\n\tSide Left: "
                              << capture_status->side_left.value();
                }
                if (capture_status->side_right.has_value()) {
                    std::cout << "\n\tSide Right: "
                              << capture_status->side_right.value();
                }
                if (capture_status->rear_center.has_value()) {
                    std::cout << "\n\tRear Center: "
                              << capture_status->rear_center.value();
                }
                std::cout << std::endl;
            } else {
                std::cerr << capture_status.error() << std::endl;
            }
        }

        if (control.has_capture_volume()) {
            auto capture_volume = control.get_capture_volume();
            if (capture_volume.has_value()) {
                std::cout << "Capture Volume:";
                if (capture_volume->front_left.has_value()) {
                    std::cout << "\n\tFront Left: "
                              << capture_volume->front_left.value() << "%";
                }
                if (capture_volume->front_right.has_value()) {
                    std::cout << "\n\tFront Right: "
                              << capture_volume->front_right.value() << "%";
                }
                if (capture_volume->rear_left.has_value()) {
                    std::cout
                      << "\n\tRear Left: " << capture_volume->rear_left.value()
                      << "%";
                }
                if (capture_volume->rear_right.has_value()) {
                    std::cout << "\n\tRear Right: "
                              << capture_volume->rear_right.value() << "%";
                }
                if (capture_volume->front_center.has_value()) {
                    std::cout << "\n\tFront Center: "
                              << capture_volume->front_center.value() << "%";
                }
                if (capture_volume->woofer.has_value()) {
                    std::cout
                      << "\n\tWoofer: " << capture_volume->woofer.value()
                      << "%";
                }
                if (capture_volume->side_left.has_value()) {
                    std::cout
                      << "\n\tSide Left: " << capture_volume->side_left.value()
                      << "%";
                }
                if (capture_volume->side_right.has_value()) {
                    std::cout << "\n\tSide Right: "
                              << capture_volume->side_right.value() << "%";
                }
                if (capture_volume->rear_center.has_value()) {
                    std::cout << "\n\tRear Center: "
                              << capture_volume->rear_center.value() << "%";
                }
                std::cout << std::endl;
            } else {
                std::cerr << capture_volume.error() << std::endl;
            }
        }

        std::cout << std::endl;
    }

    return 0;
}

int set_sound() {
    std::cout << std::boolalpha; // Pretty print boolean values.

    auto mixer = syst::sound_mixer_t::get();
    if (mixer.has_error()) {
        std::cerr << mixer.error() << std::endl;
        return 1;
    }

    auto controls = mixer->all_controls();

    syst::sound_control_t::status_t new_status;
    new_status.front_left = false;
    new_status.front_right = false;
    new_status.rear_left = false;
    new_status.rear_right = false;
    new_status.front_center = false;
    new_status.woofer = false;
    new_status.side_left = false;
    new_status.side_right = false;
    new_status.rear_center = false;

    syst::sound_control_t::volume_t new_volume;
    new_volume.front_left = 0;
    new_volume.front_right = 0;
    new_volume.rear_left = 0;
    new_volume.rear_right = 0;
    new_volume.front_center = 0;
    new_volume.woofer = 0;
    new_volume.side_left = 0;
    new_volume.side_right = 0;
    new_volume.rear_center = 0;

    for (auto& control : controls) {
        if (control.has_playback_status()) {
            auto old_playback_status = control.get_playback_status();
            if (old_playback_status.has_error()) {
                std::cerr << old_playback_status.error() << std::endl;
                return 1;
            }

            auto result = control.set_playback_status(new_status);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.toggle_playback_status();
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, resetting the status too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_playback_status(old_playback_status.value());
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }
        }

        if (control.has_playback_volume()) {
            auto old_playback_volume = control.get_playback_volume();
            if (old_playback_volume.has_error()) {
                std::cerr << old_playback_volume.error() << std::endl;
                return 1;
            }
            auto result = control.set_playback_volume(new_volume);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, setting the volume too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_playback_volume_all(0);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, resetting the volume too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_playback_volume(old_playback_volume.value());
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }
        }

        if (control.has_capture_status()) {
            auto old_capture_status = control.get_capture_status();
            if (old_capture_status.has_error()) {
                std::cerr << old_capture_status.error() << std::endl;
                return 1;
            }
            auto result = control.set_capture_status(new_status);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.toggle_capture_status();
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, resetting the status too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_capture_status(old_capture_status.value());
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }
        }

        if (control.has_capture_volume()) {
            auto old_capture_volume = control.get_capture_volume();
            if (old_capture_volume.has_error()) {
                std::cerr << old_capture_volume.error() << std::endl;
                return 1;
            }
            auto result = control.set_capture_volume(new_volume);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, setting the volume too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_capture_volume_all(0);
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }

            // For some reason, resetting the volume too quickly fails but does
            // not provide a reason why.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            result = control.set_capture_volume(old_capture_volume.value());
            if (result.failure()) {
                std::cerr << result.error() << std::endl;
                return 1;
            }
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    // If there are no arguments, get sound information.
    // If there is at least one argument, set sound status and volume.

    if (argc == 1) {
        return get_sound();
    }

    return set_sound();
}
