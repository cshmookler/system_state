#pragma once

/*****************************************************************************/
/*  Copyright (c) 2024 Caden Shmookler                                       */
/*                                                                           */
/*  This software is provided 'as-is', without any express or implied        */
/*  warranty. In no event will the authors be held liable for any damages    */
/*  arising from the use of this software.                                   */
/*                                                                           */
/*  Permission is granted to anyone to use this software for any purpose,    */
/*  including commercial applications, and to alter it and redistribute it   */
/*  freely, subject to the following restrictions:                           */
/*                                                                           */
/*  1. The origin of this software must not be misrepresented; you must not  */
/*     claim that you wrote the original software. If you use this software  */
/*     in a product, an acknowledgment in the product documentation would    */
/*     be appreciated but is not required.                                   */
/*  2. Altered source versions must be plainly marked as such, and must not  */
/*     be misrepresented as being the original software.                     */
/*  3. This notice may not be removed or altered from any source             */
/*     distribution.                                                         */
/*****************************************************************************/

/**
 * @file core.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Core utilities for fetching and modifying the system state.
 * @date 2024-09-26
 */

// Standard includes
#include <memory>
#include <optional>
#include <list>

namespace syst {

class System {
    struct Impl;

    std::unique_ptr<Impl> inst_;

    System();

  public:
    System(const System&) = delete;
    System(System&&) noexcept = default;
    System& operator=(const System&) = delete;
    System& operator=(System&&) noexcept = default;
    ~System();

    /**
     * @brief Attempt to create a new instance of this object.
     */
    [[nodiscard]] static std::optional<System> get();

    /**
     * @return the number of seconds since the system booted.
     */
    [[nodiscard]] uint64_t get_uptime() const;

    /**
     * @return the system load average over the last one minute.
     */
    [[nodiscard]] double get_load_1() const;

    /**
     * @return the system load average over the last five minutes.
     */
    [[nodiscard]] double get_load_5() const;

    /**
     * @return the system load average over the last fifteen minutes.
     */
    [[nodiscard]] double get_load_15() const;

    /**
     * @return total addressable RAM in bytes (minus reserved bits and the
     * kernel binary code).
     */
    [[nodiscard]] uint64_t get_ram_total() const;

    /**
     * @return free addressable RAM in bytes.
     */
    [[nodiscard]] uint64_t get_ram_free() const;

    /**
     * @return shared addressable RAM in bytes.
     */
    [[nodiscard]] uint64_t get_ram_shared() const;

    /**
     * @return buffered addressable RAM in bytes.
     */
    [[nodiscard]] uint64_t get_ram_buffered() const;

    /**
     * @return total swap space in bytes.
     */
    [[nodiscard]] uint64_t get_swap_total() const;

    /**
     * @return free swap space in bytes.
     */
    [[nodiscard]] uint64_t get_swap_free() const;

    /**
     * @return the number of currently running processes.
     */
    [[nodiscard]] unsigned short get_procs() const;

    /**
     * @return the ratio of free RAM to total RAM multiplied by 100.
     */
    [[nodiscard]] double get_ram_perc() const;

    /**
     * @return the ratio of free swap to total swap multiplied by 100.
     */
    [[nodiscard]] double get_swap_perc() const;
};

} // namespace syst
