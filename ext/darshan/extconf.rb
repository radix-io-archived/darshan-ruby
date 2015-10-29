#
#  (C) 2015 by Argonne National Laboratory.
#      See COPYRIGHT in top-level directory.
#

require 'mkmf'

dir_config('darshan')
dir_config('z')
dir_config('bz2')

if(not have_header('darshan-log-format.h'))
	$stderr << "Error, could not find darshan-log-format.h\n"
	$stderr << "Please use --with-darshan-dir=...\n"
	abort "Missing darshan-log-format.h"
end

if(not have_library('z'))
	$stderr << "Error, could not locate zlib.\n"
	abort "Missing zlib"
end

if(not have_library('bz2'))
	$stderr << "Error, could not locate libbz2.\n"
	abort "Missing bz2"
end

if(not have_library('darshan-util'))
	$stderr << "Error, could not locate libdarshan-util.a\n"
	$stderr << "Please use --with-darshan-dir=...\n"
	abort "Missing libdarshan-util.a"
end

create_makefile("Darshan3rb")
