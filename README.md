<h1>dirsize - a simple utility to count file sizes in a directory</h1>

Version 0.1a<p/>

<h2>What is this?</h2>

dirsize is a simple utility that totals the sizes of files in a particular
directory. Unlike the standard <code>du</code>, it is not concerned with 
working out the total disk space usage, but the actual size of the file
contents. dirsize will always report a smaller figure than <code>du</code>,
because it will ignore the storage of inodes, etc. More importantly,
dirsize should give the same results when applied to the same files
on different systems. This can not be said for <code>du</code>, because
the overheads involved in maintaining the directory structure are
system-dependent.
<p/>
The main intended application of dirsize is to act as a quick check that
directory trees have been copied correctly from one system to another using,
e.g., <code>rsync</code>. <code>du</code> is often unhelpful here, because
it is not expected to report exactly the same total size on different
filesystems.

<h2>Basic usage</h2>

Run dirsize with one or more directories as arguments.

<pre class="codeblock">
$ dirsize [directories...]
</pre>

If no directories are specified, dirsize works on the current directory.

<h2>Building and installing</h2>

Unpack the source code bundle (see the Downloads section at the end)
and do

<pre class="codeblock"/>
$ make
$ sudo make install
</pre>

as ususal.

<h2>Command line</h2>

<b>-a</b><br/>
Include hidden files. The default is to ignore them.
<p/>

<b>-d</b><br/>
Directory only; do not recurse into any subdirectories.
<p/>

<b>-f</b><br/>
Format file size in kilobytes, etc. See notes below about how these
figures should be interpreted. The default is to output the exact number
of bytes counted.
<p/>

<b>-h</b><br/>
Show the help text.
<p/>

<b>-s</b><br/>
Summarize. Only show the totals for top-level directories. This does not
prevent lower-level directories contributing to those totals; to eliminate
subdirectories altogether, use <b>-d</b>.
<p/>

<b>-v</b><br/>
Show the version and quit.


<p/>

<h2>Notes</h2>

<ul>
<li>dirsize does not interpret symbolic links in any way. They are completely 
ignored, and neither the link nor the target contributes to the total. 
This is because <code>dirsize</code> is concerned with actual file sizes,
not filesytem organization</li>
<li>For the same reason, <code>dirsize</code> also ignores pipes, 
special files, etc</li>
<li>For the purposes of this utility, a megabyte is 1024 kB, and a gigabyte is 1024 Mb</li>
</ul>

<h2>Legal and copying</h2>

dirsize is maintained by Kevin Boone (kevin at railwayterrace dot com)
and is distributed under the terms of the GNU Public Licence, version 3.
In short, you may use it however you wish other than to claim it as your own work
(as if you would want to) but there is no warranty of any kind.


