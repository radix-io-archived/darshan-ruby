Gem::Specification.new do |s|
  s.name        = 'darshan-ruby'
  s.version     = '3.0.0'
  s.date        = '2015-10-12'
  s.summary     = "Ruby binding to Darshan"
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
		   "ext/darshan/extconf.rb"]
  s.require_paths = [ 'lib', 'ext' ]
  s.extensions = %w[ext/darshan/extconf.rb]
  s.homepage    = 'http://www.mcs.anl.gov/research/projects/darshan/'
  s.license     = 'GOVERNMENT LICENSE'
  s.rubyforge_project = "nowarning"
end
