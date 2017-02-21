/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include "darshan-ruby.h"

extern VALUE cDarshanRecord;
extern VALUE mDarshan;

VALUE cDarshanDXTRecord;
VALUE mDarshanDXT;

static VALUE Darshan3rb_dxt_get_rank(VALUE self)
{
	struct dxt_file_record* c_record = NULL;
	Data_Get_Struct(self,struct dxt_file_record, c_record);
	if(c_record) return LL2NUM(c_record->base_rec.rank);
	else return Qnil;
}

static VALUE Darshan3rb_dxt_get_hostname(VALUE self)
{
	struct dxt_file_record* c_record = NULL;
	Data_Get_Struct(self,struct dxt_file_record, c_record);
	if(c_record) return rb_str_new2(c_record->hostname);
	else return Qnil;
}

static VALUE Darshan3rb_dxt_get_write_count(VALUE self)
{
	struct dxt_file_record* c_record = NULL;
	Data_Get_Struct(self,struct dxt_file_record, c_record);
	if(c_record) return LL2NUM(c_record->write_count);
	else return Qnil;
}

static VALUE Darshan3rb_dxt_get_read_count(VALUE self)
{
	struct dxt_file_record* c_record = NULL;
	Data_Get_Struct(self,struct dxt_file_record, c_record);
	if(c_record) return LL2NUM(c_record->read_count);
	else return Qnil;
}

void Darshan3rb_init_dxt()
{
	mDarshanDXT = rb_define_module_under(mDarshan,"DXT");

	cDarshanDXTRecord = rb_define_class_under(mDarshanDXT,"Record",cDarshanRecord);
	rb_define_method(cDarshanDXTRecord,"rank",Darshan3rb_dxt_get_rank,0);
	rb_define_method(cDarshanDXTRecord,"hostname",Darshan3rb_dxt_get_hostname,0);
	rb_define_method(cDarshanDXTRecord,"write_count",Darshan3rb_dxt_get_write_count,0);
	rb_define_method(cDarshanDXTRecord,"read_count",Darshan3rb_dxt_get_read_count,0);
}

VALUE Darshan3rb_get_dxt_posix_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct dxt_file_record* c_record = NULL;
	int r = mod_logutils[DXT_POSIX_MOD]->log_get_record(fd, (void**)&c_record);
	if(r != 1) return Qnil;
	*rec_id = c_record->base_rec.id;
	VALUE rb_record = Data_Wrap_Struct(cDarshanDXTRecord, NULL , free, c_record);
	return rb_record;
}

VALUE Darshan3rb_get_dxt_mpiio_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct dxt_file_record* c_record = NULL;
	int r = mod_logutils[DXT_MPIIO_MOD]->log_get_record(fd, (void**)&c_record);
	if(r != 1) return Qnil;
	*rec_id = c_record->base_rec.id;
	VALUE rb_record = Data_Wrap_Struct(cDarshanDXTRecord, NULL , free, c_record);
	return rb_record;
}
