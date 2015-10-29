/*
 *  (C) 2009 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
#include "darshan-ruby.h"

VALUE mDarshan; 	// Darshan module in Ruby
VALUE cDarshanException;// Darshan::Exception class
VALUE cDarshanLogFile; 	// Darshan::LogFile class
VALUE cDarshanRecord;	// Darshan::Record class
VALUE cDarshanModule;	// Darshan::Module class
VALUE cDarshanHash;	// Darshan::Hash class

/* Within: Darshan module
 * Opens a log file using its name.
 * Returns an instance of Darshan::LogFile on sucess, nil on failure.
 */
static VALUE rb_darshan_open(VALUE self, VALUE name)
{
	const char* c_name = rb_string_value_cstr(&name);
	darshan_fd fd = darshan_log_open(c_name);
	char buffer[DARSHAN_JOB_METADATA_LEN];
	if(fd != NULL) {
		VALUE res = Data_Wrap_Struct(cDarshanLogFile, NULL ,NULL, fd);
		// set the name of the file
		rb_iv_set(res, "@name", name);//rb_str_new2(fd->name));

		// get the job struct
		struct darshan_job job;
		int err = darshan_log_getjob(fd,&job);

		if(err < 0) return Qnil;
		
		// set the version number
		rb_iv_set(res,"@version", rb_str_new2(fd->version));
		// set the uid
		rb_iv_set(res,"@uid", LL2NUM(job.uid));
		// set the start time
		rb_iv_set(res,"@start_time", LL2NUM(job.start_time));
		// set the end time
		rb_iv_set(res,"@end_time", LL2NUM(job.end_time));
		// set the job id
		rb_iv_set(res,"@job_id", LL2NUM(job.jobid));
		// set the number of processes
		rb_iv_set(res,"@nprocs", LL2NUM(job.nprocs));
		// set the metadata
		VALUE metadata = rb_hash_new();
		char *token;
		char *save;
		for(token=strtok_r(job.metadata, "\n", &save);
		    token != NULL;
		    token=strtok_r(NULL, "\n", &save)) {
			char *key;
			char *value;
			strcpy(buffer, token);
			key = buffer;
			value = index(buffer, '=');
			if(!value) continue;
			value[0] = '\0';
			value++;
			rb_hash_aset(metadata, rb_str_new2(key), rb_str_new2(value));
		}
		rb_iv_set(res,"@metadata", metadata);

		// set the executable name
		char exe[DARSHAN_EXE_LEN+2];
		err = darshan_log_getexe(fd, exe);

		if(err < 0) return Qnil;

                rb_iv_set(res,"@exe", rb_str_new2(exe));

		rb_iv_set(res,"@partial", fd->partial_flag ? Qtrue : Qfalse);

		rb_iv_set(res,"@current_module", INT2NUM(-1));

		// set the list of mount points
		char** mnt_pts = NULL;
		char** fs_types = NULL;
		int count = 0;
		err = darshan_log_getmounts(fd,&mnt_pts,&fs_types,&count);
		
		if(err < 0) {
			return Qnil;
		}

		VALUE path = ID2SYM(rb_intern("path"));
		VALUE type = ID2SYM(rb_intern("type"));
		VALUE mp = rb_ary_new2(count);
		int i;
		for(i=0; i<count; i++) {
			VALUE hash = rb_hash_new();
			rb_hash_aset(hash,path,rb_str_new2(mnt_pts[i]));
			rb_hash_aset(hash,type,rb_str_new2(fs_types[i]));
			rb_ary_store(mp,i,hash);
			if(mnt_pts != NULL && mnt_pts[i] != NULL) 
				free(mnt_pts[i]);
			if(fs_types != NULL && fs_types[i] != NULL)
				free(fs_types[i]);
		}

		if(mnt_pts != NULL) free(mnt_pts);
		if(fs_types != NULL) free(fs_types);
		rb_iv_set(res,"@mount_points", mp);

		// get the hash
		struct darshan_record_ref *rec_hash = NULL;
		err = darshan_log_gethash(fd, &rec_hash);
		if(err < 0) {
			return Qnil;
		}

		VALUE rb_hash = Data_Wrap_Struct(cDarshanHash, NULL ,NULL, rec_hash);
		rb_iv_set(res,"@hash",rb_hash);

		return res;
	} else {
		return Qnil;
	}
}

/* Within: Darshan module
 * Closes a file (instance of Darshan::LogFile)
 * Returns true on success, false on failure.
 */
static VALUE rb_darshan_close(VALUE self, VALUE fd)
{
	darshan_fd c_fd = NULL;
	Data_Get_Struct(fd,struct darshan_fd_s,c_fd);
	if(c_fd != NULL) {
		darshan_log_close(c_fd);
		return Qtrue;
	} else {
		return Qfalse;
	}
}

/* Within: Darshan::Module LogFile class
 * Returns the next Darshan::Module entry from the log file,
 * or nil if there is no more modules to read.
 */
static VALUE rb_darshan_next_module(VALUE self) 
{
	darshan_fd fd = NULL;
	Data_Get_Struct(self,struct darshan_fd_s, fd);
	if(fd == NULL) return Qnil;
	int i = NUM2INT(rb_iv_get(self,"@current_module"));
	i +=1;

	while(fd->mod_map[i].len == 0 && i < DARSHAN_MAX_MODS) i++;
	if(i >= DARSHAN_MAX_MODS || (!mod_logutils[i])) {
		rb_iv_set(self,"@current_module",INT2NUM(i));
		return Qnil;
	}
	// Creat a Darshan::Module object
	VALUE argv[0];
	VALUE res = rb_class_new_instance(0, argv, cDarshanModule);
	VALUE name = rb_str_new2(darshan_module_names[i]);
	VALUE length = LL2NUM(fd->mod_map[i].len);

	rb_iv_set(res,"@name", name);
	rb_iv_set(res,"@length", length);

	rb_iv_set(res,"@fd",self);
	rb_iv_set(res,"@index",	INT2NUM(i));

	rb_iv_set(self,"@current_module",INT2NUM(i));

	return res;
}

/* Within: Darshan::Module class
 * Returns the next Darshan::Record entry from the module,
 * or nil if there is no more such an entry.
 */
static VALUE rb_darshan_next_record(VALUE self)
{
	// get the index of the module to access its functions
	int i = NUM2INT(rb_iv_get(self,"@index"));

	// get the parent log file descriptor
	VALUE rfd = rb_iv_get(self,"@fd");
	darshan_fd fd = NULL;
	Data_Get_Struct(rfd,struct darshan_fd_s, fd);
	if(fd == NULL) return Qnil;

	VALUE rbhash = rb_iv_get(rfd,"@hash");
	struct darshan_record_ref* rec_hash = NULL;
	Data_Get_Struct(rbhash,struct darshan_record_ref, rec_hash);
	if(rec_hash == NULL) return Qnil;

	darshan_record_id rec_id;

	VALUE res = Qnil;
	switch(i) {
	case DARSHAN_NULL_MOD:
		res = Qnil;
		break;
	case DARSHAN_POSIX_MOD:
		res = Darshan3rb_get_posix_record(fd,&rec_id);
		break;
	case DARSHAN_MPIIO_MOD:
		res = Darshan3rb_get_mpiio_record(fd,&rec_id);
		break;
	case DARSHAN_HDF5_MOD:
		res = Darshan3rb_get_hdf5_record(fd,&rec_id);
		break;
	case DARSHAN_PNETCDF_MOD:
		res = Darshan3rb_get_pnetcdf_record(fd,&rec_id);
		break;
	case DARSHAN_BGQ_MOD:
		res = Darshan3rb_get_bgq_record(fd,&rec_id);
		break;
	}

	if(res == Qnil) return Qnil;

	struct darshan_record_ref* ref;
	HASH_FIND(hlink, rec_hash, &rec_id, sizeof(darshan_record_id), ref);

	rb_iv_set(res,"@name",rb_str_new2(ref->rec.name));

	return res;
}

/* Initialize the Darshan Ruby library
 */
void Init_Darshan3rb() {
  	mDarshan = rb_define_module("Darshan");

	cDarshanLogFile 	= rb_define_class_under(mDarshan,"LogFile",	rb_cObject);
	cDarshanException 	= rb_define_class_under(mDarshan,"Exception",	rb_cObject);
	cDarshanModule 		= rb_define_class_under(mDarshan,"Module",	rb_cObject);
	cDarshanRecord		= rb_define_class_under(mDarshan,"Record",	rb_cObject);
	cDarshanHash		= rb_define_class_under(mDarshan,"Hash",	rb_cObject);

	rb_define_module_function(mDarshan,"open",	rb_darshan_open,	1);
	rb_define_module_function(mDarshan,"close",	rb_darshan_close,	1);

	rb_define_const(mDarshan,"VERSION",rb_str_new2(DARSHAN_LOG_VERSION));

	Darshan3rb_init_posix();
	Darshan3rb_init_mpiio();
	Darshan3rb_init_hdf5();
	Darshan3rb_init_pnetcdf();
	Darshan3rb_init_bgq();

	rb_define_method(cDarshanLogFile,"next_module",
			rb_darshan_next_module,0);
	rb_define_method(cDarshanModule,"next_record",
			rb_darshan_next_record,0);
}

