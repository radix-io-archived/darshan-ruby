Darshan-Ruby is a binding to the Darshan-util library.
Darshan is a lightweight I/O characterization tool that transparently
captures I/O access pattern information from HPC applications.
Darshan can be used to tune applications for increased scientific
productivity or to gain insight into trends in large-scale computing
systems.

Please see the 
[Darshan web page](http://www.mcs.anl.gov/research/projects/darshan)
for more in-depth news and documentation about Darshan.

Darshan-Ruby is designed to support Darshan 3.0.0 and above.

NOTE
----
 
This source is not necessary to install Darshan-Ruby, as
Darshan-Ruby is available as a ruby gem on rubygems.org.

INSTALLING 
----------

Darshan must be installed with bzip2 support, and with the --enable-shared 
build option. Assuming the path to Darshan is "path/to/darshan", install 
Darshan-Ruby can be done as follows:

> gem install darshan-ruby -- --with-darshan-dir=path/to/darshan


