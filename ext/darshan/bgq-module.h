/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include <ruby.h>

void Darshan3rb_init_bgq();

VALUE Darshan3rb_get_bgq_record(darshan_fd fd, darshan_record_id* rec_id);
