/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include "darshan-ruby.h"

extern VALUE cDarshanRecord;
extern VALUE mDarshan;

VALUE cDarshanPNETCDFRecord;
VALUE mDarshanPNETCDF;

static VALUE Darshan3rb_pnetcdf_get_rank(VALUE self)
{
	struct darshan_pnetcdf_file* c_record = NULL;
	Data_Get_Struct(self,struct darshan_fd_s,c_record);
	if(c_record) return LL2NUM(c_record->rank);
	else return Qnil;
}

static VALUE Darshan3rb_pnetcdf_get_counter(VALUE self, VALUE index)
{
	struct darshan_pnetcdf_file* c_record = NULL;
	Data_Get_Struct(self,struct darshan_fd_s,c_record);
	int i = NUM2INT(index);
	if((i < 0) || (c_record == NULL)) return Qnil;
	if(i < PNETCDF_NUM_INDICES) return LL2NUM(c_record->counters[i]);
	if(i < PNETCDF_NUM_INDICES+PNETCDF_F_NUM_INDICES) return rb_float_new(c_record->fcounters[i-PNETCDF_NUM_INDICES]);
	else return Qnil;
}

void Darshan3rb_init_pnetcdf()
{
	mDarshanPNETCDF = rb_define_module_under(mDarshan,"PNETCDF");

	VALUE cnames = rb_ary_new2(PNETCDF_NUM_INDICES+PNETCDF_F_NUM_INDICES);
	int i;
	for(i=0; i < PNETCDF_NUM_INDICES; i++) {
		rb_ary_store(cnames,i,rb_str_new2(pnetcdf_counter_names[i]));
		rb_define_const(mDarshanPNETCDF,pnetcdf_counter_names[i],INT2NUM(i));
	}
	for(i=0; i < PNETCDF_F_NUM_INDICES; i++) {
		int j = i+PNETCDF_NUM_INDICES;
		rb_ary_store(cnames,j,rb_str_new2(pnetcdf_f_counter_names[i]));
		rb_define_const(mDarshanPNETCDF,pnetcdf_f_counter_names[i],INT2NUM(j));
	}
	rb_define_const(mDarshanPNETCDF,"NAMES",cnames);

	cDarshanPNETCDFRecord = rb_define_class_under(mDarshanPNETCDF,"Record",cDarshanRecord);
	rb_define_method(cDarshanPNETCDFRecord,"rank",Darshan3rb_pnetcdf_get_rank,0);
	rb_define_method(cDarshanPNETCDFRecord,"counter",Darshan3rb_pnetcdf_get_counter,1);
}

VALUE Darshan3rb_get_pnetcdf_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct darshan_pnetcdf_file* c_record = (struct darshan_pnetcdf_file*)malloc(sizeof(struct darshan_pnetcdf_file));	
	int r = mod_logutils[DARSHAN_PNETCDF_MOD]->log_get_record(fd, (char*)c_record, rec_id);
	if(r != 1) return Qnil;

	VALUE rb_record = Data_Wrap_Struct(cDarshanPNETCDFRecord, NULL , free, c_record);
	return rb_record;
}
