//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_COMMONS_HPP
#define CONCURRENT_SV_THREAD_COMMONS_HPP

/**
 * Bad request -> id <= 0
 * Last 2 digits -> request_id
 * Request with param -> request_id / 100 = param_count
 */

enum CommandIDs : sint16 {
    __BAD_REQUEST            = 0,          /* NOLINT(bugprone-reserved-identifier) */
    __NO_PARAM_REQUEST       = -1,         /* NOLINT(bugprone-reserved-identifier) */
    __SIGNAL                 = 20,         /* NOLINT(bugprone-reserved-identifier) */
    __GET_GS                 = 21,         /* NOLINT(bugprone-reserved-identifier) */
    __ENABLE_SPORTS          = 22,         /* NOLINT(bugprone-reserved-identifier) */
    __DISABLE_SPORTS         = 23,         /* NOLINT(bugprone-reserved-identifier) */
    __GET_WEATHER            = 24,         /* NOLINT(bugprone-reserved-identifier) */
    __EXIT                   = 99,         /* NOLINT(bugprone-reserved-identifier) */


    __TIMED_EVENTS_UPDATE         = 1,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPORTS_UPDATE         = 2,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPEED_UPDATE          = 3,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_SPEED_LIMIT_UPDATE    = 4,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_EVENT_STILL_PRESENT   = 5,      /* NOLINT(bugprone-reserved-identifier) */
    __TIMED_POSITION_UPDATE       = 6,      /* NOLINT(bugprone-reserved-identifier) */

    __INITIALIZING_POSITION_REQUEST = 9000,     /* NOLINT(bugprone-reserved-identifier) */
    __INITIALIZING_SPEED_REQUEST    = 9001,     /* NOLINT(bugprone-reserved-identifier) */

    __EVENT_MISSING = 1000     /* NOLINT(bugprone-reserved-identifier) */
};

#endif //CONCURRENT_SV_THREAD_COMMONS_HPP
