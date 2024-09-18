#include "pid_record.h"
#include "simulator.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fcfs_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->arrival_time - second_pid_record->arrival_time;
}

int sjn_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->actual_cpu_burst- second_pid_record->actual_cpu_burst;
}

int rr_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->added_to_queue - second_pid_record->added_to_queue;
}

int priority_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return second_pid_record->pid - first_pid_record->pid;
}

int srt_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->exp_time_remaining_chart[first_pid_record->pid] - second_pid_record->exp_time_remaining_chart[second_pid_record->pid];
}

int main(int argc, char *argv[]) {
  // Checking if the number of arguments is correct
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [FCFS,SJF,RR,Priority,SRT] < [inputfile]\n",
            argv[0]);
    return 1;
  }

  // Finding which scheduling algorithm was chosen
  char *scheduling_algorithm = argv[1];
  char *scheduling_algorithms[] = {"FCFS", "SJF","RR", "Priority", "SRT"};
  int num_scheduling_algorithms =
      sizeof(scheduling_algorithms) / sizeof(char *);
  int algorithm_chosen = 0;

  while (algorithm_chosen < num_scheduling_algorithms) {
    if (!strcmp(scheduling_algorithm,
                scheduling_algorithms[algorithm_chosen])) {
      break;
    }
    algorithm_chosen++;
  }
  // erroring if none of the valid choices are used
  if (algorithm_chosen == num_scheduling_algorithms) {
    fprintf(stderr,
            "Invalid scheduling algorithm: %s, please choose one of the "
            "following: ",
            scheduling_algorithm);
    int i = 0;

    while (i < num_scheduling_algorithms - 1) {
      fprintf(stderr, "%s, ", scheduling_algorithms[i]);
      i++;
    }
    fprintf(stderr, "%s\n", scheduling_algorithms[i]);
    return 1;
  }
  pid_records_t pid_records = create_pid_records();

  simulator_t simulator;
  if (algorithm_chosen == 0) {
    simulator = simulator_new(&pid_records, &fcfs_comp, 0, 0);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&pid_records, &sjn_comp, 0, 0);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&pid_records, &rr_comp, 3, 0);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&pid_records, &priority_comp, 1, 0);
  } else if (algorithm_chosen == 4) {
    simulator = simulator_new(&pid_records, &srt_comp, 1, 0.1);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  pid_records_t *pid_completion_records = simulator_run(&simulator);
  pid_completion_records_print(pid_completion_records);
  return 0;
}
