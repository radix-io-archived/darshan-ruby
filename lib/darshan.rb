#######################################################################
# Author: 	Matthieu Dorier
# Institution: 	Argonne National Laboratory
# Mail: 	mdorier [at] anl.gov
# Date: 	October 2015
#######################################################################
require "Darshan3rb"

#=====================================================================#
# Darshan module, contains the functions to open and close a LogFile,
# as well as all the classes (LogFile, Job and File). Some of these
# classes are defined through the C code, and are thus not shown here.
#=====================================================================#
module Darshan

	#=============================================================#
	# LogFile class, represents a binary log file (compressed).
	# Open it by using
	#	logfile = LogFile.open("filename")
	#	...
	#	logfile.close
	# or
	#	LogFile.open("filename") do | logfile |
	#		...
	#	end
	#=============================================================#
	class LogFile

		attr_reader :name
		attr_reader :version
		attr_reader :exe
		attr_reader :uid
		attr_reader :job_id
		attr_reader :nprocs
		attr_reader :metadata
		attr_reader :mount_points

		#=====================================================#
		# Opens a logfile, calls the given block if present.
		# The file is open for reading only.
		#=====================================================#
		def self.open(filename)
			lf = Darshan.open(filename)
			if block_given? and lf != nil
				yield lf
				lf.close
				lf = nil
			end
			return lf
		end

		#=====================================================#
		# Creates a logfile, calls the given block if present.
		# The file is open for writing.
		#=====================================================#
		def self.create(filename)
			lf = Darshan.create(filename)
			if block_given? and lf != nil
				yield lf
				lf.close
				lf = nil
			end
			return lf
		end

		#======================================================#
		# Close the file.
		#======================================================#
		def close
			Darshan.close(self)
		end

		#======================================================#
		# Iterates through all the file entries described in
		# the log file.
		#======================================================#
		def each_module
			if not block_given?
				return nil
			end
			mod = nil
			while((mod = next_module()) != nil)
				yield mod
			end
			return nil
		end

		def partial?
			@partial
		end

		def start_time
			Time.at(@start_time)
		end

		def end_time
			Time.at(@end_time)
		end
	end

	class Module
		
		attr_reader :name
		attr_reader :length
		attr_reader :index

		def each_record
			return nil if not block_given?
			rec = nil
			while((rec = next_record()) != nil)
				yield rec
			end
			return nil
		end
	end

	class Record
		attr_reader :name
	end
end
