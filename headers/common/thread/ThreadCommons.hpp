//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_FUNCTIONS_HPP
#define CONCURRENT_SV_THREAD_FUNCTIONS_HPP

/**
 * Bad request -> id <= 0
 * Last 2 digits -> request_id
 * Request with param -> request_id / 100 = param_count
 */

enum CommandIDs {
    __BAD_REQUEST      = 0,        /* NOLINT(bugprone-reserved-identifier) */
    __NO_PARAM_REQUEST = -1,        /* NOLINT(bugprone-reserved-identifier) */
    __SIGNAL           = 101,       /* NOLINT(bugprone-reserved-identifier) */
    __GET_GP           = 2,         /* NOLINT(bugprone-reserved-identifier) */
    __ENABLE_S         = 3,         /* NOLINT(bugprone-reserved-identifier) */
    __ENABLE_W         = 4,         /* NOLINT(bugprone-reserved-identifier) */
    __EXIT             = 99,        /* NOLINT(bugprone-reserved-identifier) */


    __TIMED_EVENTS_UPDATE         = 5,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPORTS_UPDATE         = 6,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_WEATHER_UPDATE        = 7,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPEED_UPDATE          = 8,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPEED_LIMIT_UPDATE    = 9       /* NOLINT(bugprone-reserved-identifier) */
};

#endif //CONCURRENT_SV_THREAD_FUNCTIONS_HPP
