/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include "darshan-ruby.h"

extern VALUE cDarshanRecord;
extern VALUE mDarshan;

VALUE cDarshanLustreRecord;
VALUE mDarshanLustre;

static VALUE Darshan3rb_lustre_get_counter(VALUE self, VALUE index)
{
	struct darshan_lustre_record* c_record = NULL;
	Data_Get_Struct(self,struct darshan_lustre_record, c_record);
	int i = NUM2INT(index);
	if((i < 0) || (c_record == NULL)) return Qnil;
	if(i < LUSTRE_NUM_INDICES) return LL2NUM(c_record->counters[i]);
	else return Qnil;
}

void Darshan3rb_init_lustre()
{
	mDarshanLustre = rb_define_module_under(mDarshan,"Lustre");

	VALUE cnames = rb_ary_new2(LUSTRE_NUM_INDICES);
	int i;
	for(i=0; i < LUSTRE_NUM_INDICES; i++) {
		rb_ary_store(cnames,i,rb_str_new2(mpiio_counter_names[i]));
		rb_define_const(mDarshanLustre,mpiio_counter_names[i],INT2NUM(i));
	}
	rb_define_const(mDarshanLustre,"NAMES",cnames);

	cDarshanLustreRecord = rb_define_class_under(mDarshanLustre,"Record",cDarshanRecord);
	rb_define_method(cDarshanLustreRecord,"counter",Darshan3rb_lustre_get_counter,1);
}

VALUE Darshan3rb_get_lustre_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct darshan_lustre_record* c_record = NULL;	
	int r = mod_logutils[DARSHAN_LUSTRE_MOD]->log_get_record(fd, (void**)&c_record);
	if(r != 1) return Qnil;
	*rec_id = c_record->base_rec.id;
	VALUE rb_record = Data_Wrap_Struct(cDarshanLustreRecord, NULL , free, c_record);
	return rb_record;
}