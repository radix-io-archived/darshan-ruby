/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include <ruby.h>

void Darshan3rb_init_dxt();

VALUE Darshan3rb_get_dxt_posix_record(darshan_fd fd, darshan_record_id* id);

VALUE Darshan3rb_get_dxt_mpiio_record(darshan_fd fd, darshan_record_id* id);
