#######################################################################
# Author:       Matthieu Dorier
# Institution:  Argonne National Laboratory
# Mail:         mdorier [at] anl.gov
# Date:         October 2015
#######################################################################

$LOAD_PATH << '../lib' # to find darshan.rb
$LOAD_PATH << '../ext/darshan' # to find Darshan3rb.so

require "rubygems" # required since darshan-ruby 1.1.0 (gem)
require "darshan"  # go look for darshan.rb

if(ARGV.size != 1)
	print "Usage: ruby test.rb <logfile.bz2>\n"
	exit
end

def print_line
	col = `tput cols`.to_i
	print '-'*col+"\n"
end

def print_counter(record, names, i)
	s = 64-names[i].size
	print "     #{names[i]}"+(' '*s)+"= #{record.counter(i)}\n"
end

print "This is Darshan version #{Darshan::VERSION}\n"
print_line

# open the logfile
Darshan::LogFile.open(ARGV[0]) do | logfile |

	print "Reading log file #{logfile.name}\n"
	print_line
	print "version      = #{logfile.version}\n"
	print "exe          = #{logfile.exe}\n"
	print "uid          = #{logfile.uid}\n"
	print "start time   = #{logfile.start_time}\n"
	print "end time     = #{logfile.end_time}\n"
	print "job id       = #{logfile.job_id}\n"
	print "job size     = #{logfile.nprocs}\n"
	print "partial      = #{logfile.partial?}\n"
	print "metadata     = #{logfile.metadata}\n"

	print_line
	logfile.mount_points.each do | mp |
		print "mount point #{mp[:path]} (#{mp[:type]})\n"
	end

	print_line
	logfile.each_module do | mod |
		print "Module #{mod.name} (#{mod.length} bytes)\n"
		mod.each_record do | record |
			case record
				when Darshan::POSIX::Record
					print "  #{record.name}\t accessed by\t #{record.rank}\n"
					for i in 0...Darshan::POSIX::NAMES.size
						print_counter(record,Darshan::POSIX::NAMES,i)
					end
				when Darshan::MPIIO::Record
					print "  #{record.name}\t accessed by\t #{record.rank}\n"
					for i in 0...Darshan::MPIIO::NAMES.size
						print_counter(record,Darshan::MPIIO::NAMES,i)
					end
				when Darshan::BGQ::Record
					print "  #{record.name}\n"
					for i in 0...Darshan::BGQ::NAMES.size
						print_counter(record,Darshan::BGQ::NAMES,i)
					end
				when Darshan::DXT::Record
					print " #{record.name}\t accessed by\t #{record.rank}\n"
					print "    hostname = #{record.hostname}\n"
					print "    write_count = #{record.write_count}\n"
					print "    read_count  = #{record.read_count}\n"
			end
		end
	end
end
