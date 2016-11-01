/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include "darshan-ruby.h"

extern VALUE cDarshanRecord;
extern VALUE mDarshan;

VALUE cDarshanLUSTRERecord;
VALUE mDarshanLUSTRE;

static VALUE Darshan3rb_lustre_get_counter(VALUE self, VALUE index)
{
	struct darshan_lustre_record* c_record = NULL;
	Data_Get_Struct(self,struct darshan_lustre_record, c_record);
	int i = NUM2INT(index);
	if((i < 0) || (c_record == NULL)) return Qnil;
	if(i < LUSTRE_NUM_INDICES) return LL2NUM(c_record->counters[i]);
	else return Qnil;
}

static VALUE Darshan3rb_lustre_get_osts(VALUE self)
{
	struct darshan_lustre_record* c_record = NULL;
	Data_Get_Struct(self,struct darshan_lustre_record, c_record);
	size_t num_osts = c_record->counters[LUSTRE_STRIPE_WIDTH];
	VALUE res = rb_ary_new2(num_osts);
	int i;
	for(i=0; i<num_osts; i++) {
		rb_ary_store(res,i,LL2NUM(c_record->ost_ids[i]));
	}
	return res;
}

void Darshan3rb_init_lustre()
{
	mDarshanLUSTRE = rb_define_module_under(mDarshan,"LUSTRE");

	VALUE cnames = rb_ary_new2(LUSTRE_NUM_INDICES);
	int i;
	for(i=0; i < LUSTRE_NUM_INDICES; i++) {
		rb_ary_store(cnames,i,rb_str_new2(mpiio_counter_names[i]));
		rb_define_const(mDarshanLUSTRE,mpiio_counter_names[i],INT2NUM(i));
	}
	rb_define_const(mDarshanLUSTRE,"NAMES",cnames);

	cDarshanLUSTRERecord = rb_define_class_under(mDarshanLUSTRE,"Record",cDarshanRecord);
	rb_define_method(cDarshanLUSTRERecord,"counter",Darshan3rb_lustre_get_counter,1);
	rb_define_method(cDarshanLUSTRERecord,"osts",Darshan3rb_lustre_get_osts,0);
}

VALUE Darshan3rb_get_lustre_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct darshan_lustre_record* c_record = NULL;	
	int r = mod_logutils[DARSHAN_LUSTRE_MOD]->log_get_record(fd, (void**)&c_record);
	if(r != 1) return Qnil;
	*rec_id = c_record->base_rec.id;
	VALUE rb_record = Data_Wrap_Struct(cDarshanLUSTRERecord, NULL , free, c_record);
	return rb_record;
}
