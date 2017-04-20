Gem::Specification.new do |s|
  s.name        = 'darshan-ruby'
  s.version     = '3.1.4.2'
  s.date        = '2017-04-20'
  s.summary     = "Ruby binding to Darshan version 3.1.4 and above"
  s.description = "Ruby binding to ANL's Darshan library for HPC I/O tracing and analysis"
  s.authors     = ["Matthieu Dorier"]
  s.email       = 'mdorier@anl.gov'
  s.files       = ["lib/darshan.rb",
		   "ext/darshan/darshan-ruby.c", "ext/darshan/darshan-ruby.h",
		   "ext/darshan/bgq-module.c", "ext/darshan/bgq-module.h",
		   "ext/darshan/hdf5-module.c", "ext/darshan/hdf5-module.h",
		   "ext/darshan/mpiio-module.c", "ext/darshan/mpiio-module.h",
		   "ext/darshan/pnetcdf-module.c", "ext/darshan/pnetcdf-module.h",
		   "ext/darshan/posix-module.c", "ext/darshan/posix-module.h",
		   "ext/darshan/stdio-module.c", "ext/darshan/stdio-module.h",
		   "ext/darshan/lustre-module.c", "ext/darshan/lustre-module.h",
		   "ext/darshan/dxt-module.c", "ext/darshan/dxt-module.h",
		   "ext/darshan/extconf.rb"]
  s.require_paths = [ 'lib', 'ext' ]
  s.executables << 'quarshan'
  s.extensions = %w[ext/darshan/extconf.rb]
  s.homepage    = 'http://www.mcs.anl.gov/research/projects/darshan/'
  s.license     = 'Nonstandard'
  s.rubyforge_project = "nowarning"
end
