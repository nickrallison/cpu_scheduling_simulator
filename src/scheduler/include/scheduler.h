//
// Created by nickr on 2024-09-21.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

int fcfs_comp(const void *first, const void *second);
int sjn_comp(const void *first, const void *second);
int rr_comp(const void *first, const void *second);
int priority_comp(const void *first, const void *second);
int srt_comp(const void *first, const void *second);

pid_results_t main_runner(int argc, char *argv[]);

#endif //SCHEDULER_H