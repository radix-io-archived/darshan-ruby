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

```
gem install darshan-ruby -- --with-darshan-dir=path/to/darshan
```

If you use [RVM](https://rvm.io/) to manage your packages:

```
rvm gemset create darshan
rvm gemset use darshan
gem install darshan-ruby -- --with-darshan-dir=path/to/darshan
```

QUARSHAN
--------

Starting from version 3.1.1.1, Darshan-Ruby comes with the **Quarshan** tool.
Quarshan is a command-line tool to query darshan files.

Once Darshan-Ruby is installed, you can call Quarshan as follows:

```
quarshan -o COUNTER file.darshan
```

For example:

```
quarshan -o POSIX_BYTES_WRITTEN ior1.darshan
```

Will output

```
/home/shane/software/darshan/testFile.00000000  1048576
/home/shane/software/darshan/testFile.00000001  1048576
/home/shane/software/darshan/testFile.00000002  1048576
/home/shane/software/darshan/testFile.00000003  1048576
```

#### Displaying a header

You can obtain a header using the **-v** (or **--verbose**) option:

```
> quarshan -o POSIX_BYTES_WRITTEN ior1.darshan -v
# records       POSIX_BYTES_WRITTEN
/home/shane/software/darshan/testFile.00000000  1048576
/home/shane/software/darshan/testFile.00000001  1048576
/home/shane/software/darshan/testFile.00000002  1048576
/home/shane/software/darshan/testFile.00000003  1048576
```

#### Working with multiple counters

You can ask for multiple counters by separating them by commas (without spaces).
These counters must all be part of the same module (here POSIX, for example):

```
> quarshan -o POSIX_BYTES_WRITTEN,POSIX_SIZE_WRITE_100K_1M ior1.darshan -v
# records       POSIX_BYTES_WRITTEN     POSIX_SIZE_WRITE_100K_1M
/home/shane/software/darshan/testFile.00000000  1048576 4
/home/shane/software/darshan/testFile.00000001  1048576 4
/home/shane/software/darshan/testFile.00000002  1048576 4
/home/shane/software/darshan/testFile.00000003  1048576 4
```

#### Computing statistics automatically

You can provide reduction operations to directly compute statistics 
using **-r** (or **--reductions**):

```
> quarshan -o POSIX_BYTES_WRITTEN,POSIX_SIZE_WRITE_100K_1M -r avg,min ior1.darshan -v
# records       avg(POSIX_BYTES_WRITTEN)        min(POSIX_SIZE_WRITE_100K_1M)
4       1048576.0       4
```
Instead of printing the name of the record on the first column, the number of
records will be printed.

Available reduction operators are: **min** (minimum), **max** (maximum), **avg** (average)
**var** (variance), **std** (standard deviation), **med** (median), **sum** (sum).
When providing reduction operations, if you provide only one reduction
operation, it will be applied to all requested counters. If you provide multiple
reduction operations, you must make sure to provide the same number as the number
of counters.

#### Conditionally selecting records

You can select only records that satisfy a particular condition using
**-s** (or **--select**).
For example to consider only records for which POSIX_F_META_TIME > 5.0e-5
and display the POSIX_BYTES_WRITTEN counter for them, use:

```
> quarshan -o POSIX_BYTES_WRITTEN ior1.darshan -v -s "POSIX_F_META_TIME > 5.0e-5"
# records       POSIX_BYTES_WRITTEN
/home/shane/software/darshan/testFile.00000001  1048576
/home/shane/software/darshan/testFile.00000002  1048576
/home/shane/software/darshan/testFile.00000003  1048576
```

Selection operations include all valid Ruby arithmetic and boolean operators.
Note that the counters used in the selection query must be part of the same module
as the counters requested for display.

Note also that the query must be in between double-quotes.

You can of course mix selection queries with reduction operators!

#### Selecting based on path

You can select only the records that have a path with a specified prefix,
using the **-p** (or **--prefix**) option. For example to select only the
records that belonged to /home, use the following command:

```
> quarshan -o POSIX_BYTES_WRITTEN -p /home ior1.darshan
# records       POSIX_BYTES_WRITTEN
/home/shane/software/darshan/testFile.00000001  1048576
/home/shane/software/darshan/testFile.00000002  1048576
/home/shane/software/darshan/testFile.00000003  1048576
```

#### Working with multiple darshan files

You can process a series of Darshan log files at once the same way you process
a single file. You can even process an entire directory, for example if you want
to know the total amount of data written for a set of jobs for which you have
the log files in a "log" directory:

```
> quarshan -o POSIX_BYTES_WRITTEN log/* -v -r sum
# records       POSIX_BYTES_WRITTEN
245     10485761024
```

#### Displaying log information

The **-i** (or **--info**) option can be used to provide more information about
the log files being processed. If only this option is provided, quarsahn will
display information about the log file(s). For example:

```
> quarshan -i ior1.darshan
# ----- NEW LOG FILE ----
# name: ior1.darshan
# darshan version: 3.00
# executable: ./ior -F
# uid: 1000
# job id: 22418
# nprocs: 4
# start time: 2015-12-16 14:46:13 -0600
# end time: 2015-12-16 14:46:13 -0600
# metadata: {"lib_ver"=>"3.0.0-pre3", "h"=>"romio_no_indep_rw=true;cb_nodes=4"}
# mount points: 
#	/run/user/1000/gvfs (fuse.gvfsd-fuse)
#	/sys/fs/pstore (pstore)
#	/dev (devtmpfs)
#	/ (ext4)
# modules: POSIX
```

This option can be used in conjunction with the **-o** option, in which case the
log file information will precede the list of records extracted from the file.
This option also works with reduction operations, in which case the list of
log file information will appear all before the resulting reduction.


TESTING
-------

A comprehensive example program can be found 
[here](https://xgitlab.cels.anl.gov/darshan/darshan-ruby/blob/master/test/test.rb).
