Darshan-Ruby is a binding to the Darshan-util library.
Darshan is a lightweight I/O characterization tool that transparently
captures I/O access pattern information from HPC applications.
Darshan can be used to tune applications for increased scientific
productivity or to gain insight into trends in large-scale computing
systems.

Please see the 
[Darshan web page](http://www.mcs.anl.gov/research/projects/darshan)
for more in-depth news and documentation about Darshan.

Darshan-Ruby versions x.y.z.v are designed to support Darshan versions x.y.z
(for instance Darshan-Ruby version 3.1.1.x support Darshan 3.1.1).

NOTE
----
 
The source present on this repository is not necessary to install Darshan-Ruby, 
as Darshan-Ruby is available as a ruby gem on [rubygems.org](http://rubygems.org).

INSTALLING 
----------

Darshan must be installed with bzip2 support, and with the --enable-shared 
build option. Assuming the path to Darshan is "path/to/darshan", install 
Darshan-Ruby can be done as follows:

> gem install darshan-ruby -- --with-darshan-dir=path/to/darshan

If you use [RVM](https://rvm.io/) to manage your packages:

> rvm gemset create darshan

> rvm gemset use darshan

> gem install darshan-ruby -- --with-darshan-dir=path/to/darshan

QUARSHAN
--------

Starting from version 3.1.1.1, Darshan-Ruby comes with the **Quarshan** tool.
[Click here to learn more about Quarshan.](quarshan)

TESTING
-------

A comprehensive example program can be found 
[here](https://xgitlab.cels.anl.gov/darshan/darshan-ruby/blob/master/test/test.rb).
