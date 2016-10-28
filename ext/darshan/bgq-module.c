/*
 * Copyright (C) 2015 University of Chicago.
 * See COPYRIGHT notice in top-level directory.
 *
 */

#include "darshan-ruby.h"

extern VALUE cDarshanRecord;
extern VALUE mDarshan;

VALUE cDarshanBGQRecord;
VALUE mDarshanBGQ;

static VALUE Darshan3rb_bgq_get_rank(VALUE self)
{
	struct darshan_bgq_record* c_record = NULL;
	Data_Get_Struct(self,struct darshan_bgq_record,c_record);
	if(c_record) return LL2NUM(c_record->base_rec.rank);
	else return Qnil;
}

static VALUE Darshan3rb_bgq_get_counter(VALUE self, VALUE index)
{
	struct darshan_bgq_record* c_record = NULL;
	Data_Get_Struct(self,struct darshan_bgq_record,c_record);
	int i = NUM2INT(index);
	if((i < 0) || (c_record == NULL)) return Qnil;
	if(i < BGQ_NUM_INDICES) return LL2NUM(c_record->counters[i]);
	if(i < BGQ_NUM_INDICES+BGQ_F_NUM_INDICES) return rb_float_new(c_record->fcounters[i-BGQ_NUM_INDICES]);
	else return Qnil;
}

void Darshan3rb_init_bgq()
{
	mDarshanBGQ = rb_define_module_under(mDarshan,"BGQ");

	VALUE cnames = rb_ary_new2(BGQ_NUM_INDICES+BGQ_F_NUM_INDICES);
	int i;
	for(i=0; i < BGQ_NUM_INDICES; i++) {
		rb_ary_store(cnames,i,rb_str_new2(bgq_counter_names[i]));
		rb_define_const(mDarshanBGQ,bgq_counter_names[i],INT2NUM(i));
	}
	for(i=0; i < BGQ_F_NUM_INDICES; i++) {
		int j = i+BGQ_NUM_INDICES;
		rb_ary_store(cnames,j,rb_str_new2(bgq_f_counter_names[i]));
		rb_define_const(mDarshanBGQ,bgq_f_counter_names[i],INT2NUM(j));
	}
	rb_define_const(mDarshanBGQ,"NAMES",cnames);

	cDarshanBGQRecord = rb_define_class_under(mDarshanBGQ,"Record",cDarshanRecord);
	rb_define_method(cDarshanBGQRecord,"rank",Darshan3rb_bgq_get_rank,0);
	rb_define_method(cDarshanBGQRecord,"counter",Darshan3rb_bgq_get_counter,1);
}

VALUE Darshan3rb_get_bgq_record(darshan_fd fd, darshan_record_id* rec_id)
{
	struct darshan_bgq_record* c_record = NULL;	
	int r = mod_logutils[DARSHAN_BGQ_MOD]->log_get_record(fd, (void**)&c_record);
	if(r != 1) return Qnil;
	*rec_id = c_record->base_rec.id;
	VALUE rb_record = Data_Wrap_Struct(cDarshanBGQRecord, NULL , free, c_record);
	return rb_record;
}
