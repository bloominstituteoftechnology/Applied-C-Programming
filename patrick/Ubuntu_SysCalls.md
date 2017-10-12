{::nomarkdown}

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en" dir="ltr">
    <head profile="http://a9.com/-/spec/opensearch/1.1/">
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link rel="search"
            type="application/opensearchdescription+xml"
            href="/ubuntu-manpage-search.xml"
            title="Ubuntu Manpage Search" />
        <title>Ubuntu Manpage:

       syscalls - Linux system calls
</title>
        <link rel="stylesheet" type="text/css" href="/assets/light/css/reset.css"/>
        <link rel="stylesheet" type="text/css" href="/assets/light/css/styles.css"/>
        <link rel="stylesheet" type="text/css" href="/manpages.css"/>
        <link rel="shortcut icon" href="/assets/light/images/favicon.ico" type="image/x-icon" />
        <script language="JavaScript" src="/functions.js"></script>
    </head>
    <body>
        <div id="container">
            <div id="container-inner">
                <div id="header">
                    <h1 id="ubuntu-header"><a href="/">Ubuntu manuals</a></h1>
                </div>
                <div id="subheader">
                    <div id="subheader-search">
                        <form method="get" action="/cgi-bin/search.py" id="cse-search-box">
                            <input type="text" name="q" tabindex="1" id="search-box-input" />
                            <button type="submit" name="op" id="search-box-button"><span>go</span></button>
                            <input type="hidden" name="cx" value="003883529982892832976:5zl6o8w6f0s" />
                            <input type="hidden" name="cof" value="FORID:9" />
                            <input type="hidden" name="ie" value="UTF-8" />
                            <script type="text/javascript" src="http://www.google.com/coop/cse/brand?form=cse-search-box&lang=en"></script>
                        </form>
                    </div>
                    <div class="subheader-menu">
                        <script>navbar();</script>
                    </div>
                </div>
                <div id="content" class="clearfix content-area">
                    <div class="level-4-nav" id="toc"></div>
                    <script>distroAndSection();</script>

Provided by: <a href='https://launchpad.net/ubuntu/zesty/+package/manpages-dev'>manpages-dev_4.09-1_all</a> <a href='https://bugs.launchpad.net/ubuntu/+source/manpages/+filebug-advanced' title='Report a bug in the content of this documentation'><img src='/img/bug.png' alt='bug' border=0></a><br><br><pre>
</pre><h4><b>NAME</b></h4><pre>
       syscalls - Linux system calls

</pre><h4><b>SYNOPSIS</b></h4><pre>
       Linux system calls.

</pre><h4><b>DESCRIPTION</b></h4><pre>
       The system call is the fundamental interface between an application and
       the Linux kernel.

   <b>System</b> <b>calls</b> <b>and</b> <b>library</b> <b>wrapper</b> <b>functions</b>
       System calls are generally not invoked directly, but rather via wrapper
       functions  in  glibc  (or  perhaps some other library).  For details of
       direct invocation of a system  call,  see  <b><a href="../man2/intro.2.html">intro</a></b>(2).   Often,  but  not
       always, the name of the wrapper function is the same as the name of the
       system call that it invokes.  For example, glibc  contains  a  function
       <b>truncate</b>() which invokes the underlying "truncate" system call.

       Often the glibc wrapper function is quite thin, doing little work other
       than copying arguments to  the  right  registers  before  invoking  the
       system call, and then setting <u>errno</u> appropriately after the system call
       has returned.   (These  are  the  same  steps  that  are  performed  by
       <b><a href="../man2/syscall.2.html">syscall</a></b>(2),  which  can  be  used  to  invoke system calls for which no
       wrapper function is provided.)  Note: system calls indicate  a  failure
       by  returning a negative error number to the caller; when this happens,
       the wrapper function negates the returned  error  number  (to  make  it
       positive),  copies  it  to  <u>errno</u>,  and returns -1 to the caller of the
       wrapper.

       Sometimes, however, the wrapper function does some  extra  work  before
       invoking the system call.  For example, nowadays there are (for reasons
       described  below)   two   related   system   calls,   <b><a href="../man2/truncate.2.html">truncate</a></b>(2)   and
       <b><a href="../man2/truncate64.2.html">truncate64</a></b>(2),  and  the glibc <b>truncate</b>() wrapper function checks which
       of those system calls are provided by the kernel and  determines  which
       should be employed.

   <b>System</b> <b>call</b> <b>list</b>
       Below  is  a  list  of the Linux system calls.  In the list, the <u>Kernel</u>
       column indicates the kernel version for those system  calls  that  were
       new in Linux 2.2, or have appeared since that kernel version.  Note the
       following points:

       *  Where no kernel version is indicated, the system  call  appeared  in
          kernel 1.0 or earlier.

       *  Where  a  system  call  is  marked  "1.2" this means the system call
          probably appeared in a 1.1.x kernel version, and first appeared in a
          stable  kernel  with  1.2.   (Development  of  the  1.2  kernel  was
          initiated from a branch of  kernel  1.0.6  via  the  1.1.x  unstable
          kernel series.)

       *  Where  a  system  call  is  marked  "2.0" this means the system call
          probably appeared in a 1.3.x kernel version, and first appeared in a
          stable  kernel  with  2.0.   (Development  of  the  2.0  kernel  was
          initiated from a branch of kernel 1.2.x,  somewhere  around  1.2.10,
          via the 1.3.x unstable kernel series.)

       *  Where  a  system  call  is  marked  "2.2" this means the system call
          probably appeared in a 2.1.x kernel version, and first appeared in a
          stable  kernel  with  2.2.0.   (Development  of  the  2.2 kernel was
          initiated from a branch of kernel  2.0.21  via  the  2.1.x  unstable
          kernel series.)

       *  Where  a  system  call  is  marked  "2.4" this means the system call
          probably appeared in a 2.3.x kernel version, and first appeared in a
          stable  kernel  with  2.4.0.   (Development  of  the  2.4 kernel was
          initiated from a branch of  kernel  2.2.8  via  the  2.3.x  unstable
          kernel series.)

       *  Where  a  system  call  is  marked  "2.6" this means the system call
          probably appeared in a 2.5.x kernel version, and first appeared in a
          stable  kernel with 2.6.0.  (Development of kernel 2.6 was initiated
          from a branch  of  kernel  2.4.15  via  the  2.5.x  unstable  kernel
          series.)

       *  Starting  with  kernel 2.6.0, the development model changed, and new
          system calls may appear in each 2.6.x release.  In  this  case,  the
          exact  version number where the system call appeared is shown.  This
          convention continues with the 3.x kernel series, which  followed  on
          from  kernel  2.6.39,  and  the 4.x kernel series, which followed on
          from kernel 3.19.

       *  In some cases, a system call was added to  a  stable  kernel  series
          after  it  branched from the previous stable kernel series, and then
          backported into the earlier stable kernel series.  For example  some
          system  calls  that  appeared  in  2.6.x were also backported into a
          2.4.x release after 2.4.15.  When this is so, the version where  the
          system call appeared in both of the major kernel series is listed.

       The  list  of system calls that are available as at kernel 4.9 (or in a
       few cases only on older kernels) is as follows:

       <b>System</b> <b>call</b>                <b>Kernel</b>        <b>Notes</b>
       ────────────────────────────────────────────────────────────────────

       <b><a href="../man2/_llseek.2.html">_llseek</a></b>(2)                 1.2
       <b><a href="../man2/_newselect.2.html">_newselect</a></b>(2)              2.0
       <b><a href="../man2/_sysctl.2.html">_sysctl</a></b>(2)                 2.0
       <b><a href="../man2/accept.2.html">accept</a></b>(2)                  2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/accept4.2.html">accept4</a></b>(2)                 2.6.28
       <b><a href="../man2/access.2.html">access</a></b>(2)                  1.0
       <b><a href="../man2/acct.2.html">acct</a></b>(2)                    1.0
       <b><a href="../man2/add_key.2.html">add_key</a></b>(2)                 2.6.11
       <b><a href="../man2/adjtimex.2.html">adjtimex</a></b>(2)                1.0
       <b><a href="../man2/alarm.2.html">alarm</a></b>(2)                   1.0
       <b><a href="../man2/alloc_hugepages.2.html">alloc_hugepages</a></b>(2)         2.5.36        Removed in 2.5.44
       <b><a href="../man2/bdflush.2.html">bdflush</a></b>(2)                 1.2           Deprecated (does nothing)
                                                since 2.6
       <b><a href="../man2/bind.2.html">bind</a></b>(2)                    2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/bpf.2.html">bpf</a></b>(2)                     3.18
       <b><a href="../man2/brk.2.html">brk</a></b>(2)                     1.0
       <b><a href="../man2/cacheflush.2.html">cacheflush</a></b>(2)              1.2           Not on x86
       <b><a href="../man2/capget.2.html">capget</a></b>(2)                  2.2
       <b><a href="../man2/capset.2.html">capset</a></b>(2)                  2.2
       <b><a href="../man2/chdir.2.html">chdir</a></b>(2)                   1.0
       <b><a href="../man2/chmod.2.html">chmod</a></b>(2)                   1.0
       <b><a href="../man2/chown.2.html">chown</a></b>(2)                   2.2           See <b><a href="../man2/chown.2.html">chown</a></b>(2) for
                                                version details
       <b><a href="../man2/chown32.2.html">chown32</a></b>(2)                 2.4
       <b><a href="../man2/chroot.2.html">chroot</a></b>(2)                  1.0
       <b><a href="../man2/clock_adjtime.2.html">clock_adjtime</a></b>(2)           2.6.39
       <b><a href="../man2/clock_getres.2.html">clock_getres</a></b>(2)            2.6
       <b><a href="../man2/clock_gettime.2.html">clock_gettime</a></b>(2)           2.6
       <b><a href="../man2/clock_nanosleep.2.html">clock_nanosleep</a></b>(2)         2.6
       <b><a href="../man2/clock_settime.2.html">clock_settime</a></b>(2)           2.6
       <b><a href="../man2/clone.2.html">clone</a></b>(2)                   1.0
       <b><a href="../man2/close.2.html">close</a></b>(2)                   1.0

       <b><a href="../man2/connect.2.html">connect</a></b>(2)                 2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/copy_file_range.2.html">copy_file_range</a></b>(2)         4.5
       <b><a href="../man2/creat.2.html">creat</a></b>(2)                   1.0
       <b><a href="../man2/create_module.2.html">create_module</a></b>(2)           1.0           Removed in 2.6
       <b><a href="../man2/delete_module.2.html">delete_module</a></b>(2)           1.0
       <b><a href="../man2/dup.2.html">dup</a></b>(2)                     1.0
       <b><a href="../man2/dup2.2.html">dup2</a></b>(2)                    1.0
       <b><a href="../man2/dup3.2.html">dup3</a></b>(2)                    2.6.27
       <b><a href="../man2/epoll_create.2.html">epoll_create</a></b>(2)            2.6
       <b><a href="../man2/epoll_create1.2.html">epoll_create1</a></b>(2)           2.6.27
       <b><a href="../man2/epoll_ctl.2.html">epoll_ctl</a></b>(2)               2.6
       <b><a href="../man2/epoll_pwait.2.html">epoll_pwait</a></b>(2)             2.6.19
       <b><a href="../man2/epoll_wait.2.html">epoll_wait</a></b>(2)              2.6
       <b><a href="../man2/eventfd.2.html">eventfd</a></b>(2)                 2.6.22
       <b><a href="../man2/eventfd2.2.html">eventfd2</a></b>(2)                2.6.27
       <b><a href="../man2/execve.2.html">execve</a></b>(2)                  1.0
       <b><a href="../man2/execveat.2.html">execveat</a></b>(2)                3.19
       <b><a href="../man2/exit.2.html">exit</a></b>(2)                    1.0
       <b><a href="../man2/exit_group.2.html">exit_group</a></b>(2)              2.6
       <b><a href="../man2/faccessat.2.html">faccessat</a></b>(2)               2.6.16
       <b><a href="../man2/fadvise64.2.html">fadvise64</a></b>(2)               2.6
       <b><a href="../man2/fadvise64_64.2.html">fadvise64_64</a></b>(2)            2.6
       <b><a href="../man2/fallocate.2.html">fallocate</a></b>(2)               2.6.23
       <b><a href="../man2/fanotify_init.2.html">fanotify_init</a></b>(2)           2.6.37
       <b><a href="../man2/fanotify_mark.2.html">fanotify_mark</a></b>(2)           2.6.37
       <b><a href="../man2/fchdir.2.html">fchdir</a></b>(2)                  1.0
       <b><a href="../man2/fchmod.2.html">fchmod</a></b>(2)                  1.0
       <b><a href="../man2/fchmodat.2.html">fchmodat</a></b>(2)                2.6.16
       <b><a href="../man2/fchown.2.html">fchown</a></b>(2)                  1.0
       <b><a href="../man2/fchown32.2.html">fchown32</a></b>(2)                2.4
       <b><a href="../man2/fchownat.2.html">fchownat</a></b>(2)                2.6.16
       <b><a href="../man2/fcntl.2.html">fcntl</a></b>(2)                   1.0
       <b><a href="../man2/fcntl64.2.html">fcntl64</a></b>(2)                 2.4
       <b><a href="../man2/fdatasync.2.html">fdatasync</a></b>(2)               2.0
       <b><a href="../man2/fgetxattr.2.html">fgetxattr</a></b>(2)               2.6; 2.4.18
       <b><a href="../man2/finit_module.2.html">finit_module</a></b>(2)            3.8
       <b><a href="../man2/flistxattr.2.html">flistxattr</a></b>(2)              2.6; 2.4.18
       <b><a href="../man2/flock.2.html">flock</a></b>(2)                   2.0
       <b><a href="../man2/fork.2.html">fork</a></b>(2)                    1.0
       <b><a href="../man2/free_hugepages.2.html">free_hugepages</a></b>(2)          2.5.36        Removed in 2.5.44
       <b><a href="../man2/fremovexattr.2.html">fremovexattr</a></b>(2)            2.6; 2.4.18
       <b><a href="../man2/fsetxattr.2.html">fsetxattr</a></b>(2)               2.6; 2.4.18
       <b><a href="../man2/fstat.2.html">fstat</a></b>(2)                   1.0
       <b><a href="../man2/fstat64.2.html">fstat64</a></b>(2)                 2.4
       <b><a href="../man2/fstatat64.2.html">fstatat64</a></b>(2)               2.6.16
       <b><a href="../man2/fstatfs.2.html">fstatfs</a></b>(2)                 1.0
       <b><a href="../man2/fstatfs64.2.html">fstatfs64</a></b>(2)               2.6
       <b><a href="../man2/fsync.2.html">fsync</a></b>(2)                   1.0
       <b><a href="../man2/ftruncate.2.html">ftruncate</a></b>(2)               1.0
       <b><a href="../man2/ftruncate64.2.html">ftruncate64</a></b>(2)             2.4
       <b><a href="../man2/futex.2.html">futex</a></b>(2)                   2.6
       <b><a href="../man2/futimesat.2.html">futimesat</a></b>(2)               2.6.16
       <b><a href="../man2/get_kernel_syms.2.html">get_kernel_syms</a></b>(2)         1.0           Removed in 2.6
       <b><a href="../man2/get_mempolicy.2.html">get_mempolicy</a></b>(2)           2.6.6
       <b><a href="../man2/get_robust_list.2.html">get_robust_list</a></b>(2)         2.6.17
       <b><a href="../man2/get_thread_area.2.html">get_thread_area</a></b>(2)         2.6
       <b><a href="../man2/getcpu.2.html">getcpu</a></b>(2)                  2.6.19
       <b><a href="../man2/getcwd.2.html">getcwd</a></b>(2)                  2.2
       <b><a href="../man2/getdents.2.html">getdents</a></b>(2)                2.0
       <b><a href="../man2/getdents64.2.html">getdents64</a></b>(2)              2.4
       <b><a href="../man2/getegid.2.html">getegid</a></b>(2)                 1.0
       <b><a href="../man2/getegid32.2.html">getegid32</a></b>(2)               2.4
       <b><a href="../man2/geteuid.2.html">geteuid</a></b>(2)                 1.0
       <b><a href="../man2/geteuid32.2.html">geteuid32</a></b>(2)               2.4
       <b><a href="../man2/getgid.2.html">getgid</a></b>(2)                  1.0

       <b><a href="../man2/getgid32.2.html">getgid32</a></b>(2)                2.4
       <b><a href="../man2/getgroups.2.html">getgroups</a></b>(2)               1.0
       <b><a href="../man2/getgroups32.2.html">getgroups32</a></b>(2)             2.4
       <b><a href="../man2/getitimer.2.html">getitimer</a></b>(2)               1.0
       <b><a href="../man2/getpeername.2.html">getpeername</a></b>(2)             2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/getpagesize.2.html">getpagesize</a></b>(2)             2.0           Not on x86
       <b><a href="../man2/getpgid.2.html">getpgid</a></b>(2)                 1.0
       <b><a href="../man2/getpgrp.2.html">getpgrp</a></b>(2)                 1.0
       <b><a href="../man2/getpid.2.html">getpid</a></b>(2)                  1.0
       <b><a href="../man2/getppid.2.html">getppid</a></b>(2)                 1.0
       <b><a href="../man2/getpriority.2.html">getpriority</a></b>(2)             1.0
       <b><a href="../man2/getrandom.2.html">getrandom</a></b>(2)               3.17
       <b><a href="../man2/getresgid.2.html">getresgid</a></b>(2)               2.2
       <b><a href="../man2/getresgid32.2.html">getresgid32</a></b>(2)             2.4
       <b><a href="../man2/getresuid.2.html">getresuid</a></b>(2)               2.2
       <b><a href="../man2/getresuid32.2.html">getresuid32</a></b>(2)             2.4
       <b><a href="../man2/getrlimit.2.html">getrlimit</a></b>(2)               1.0
       <b><a href="../man2/getrusage.2.html">getrusage</a></b>(2)               1.0
       <b><a href="../man2/getsid.2.html">getsid</a></b>(2)                  2.0
       <b><a href="../man2/getsockname.2.html">getsockname</a></b>(2)             2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/getsockopt.2.html">getsockopt</a></b>(2)              2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/gettid.2.html">gettid</a></b>(2)                  2.4.11
       <b><a href="../man2/gettimeofday.2.html">gettimeofday</a></b>(2)            1.0
       <b><a href="../man2/getuid.2.html">getuid</a></b>(2)                  1.0
       <b><a href="../man2/getuid32.2.html">getuid32</a></b>(2)                2.4
       <b><a href="../man2/getxattr.2.html">getxattr</a></b>(2)                2.6; 2.4.18
       <b><a href="../man2/init_module.2.html">init_module</a></b>(2)             1.0
       <b><a href="../man2/inotify_add_watch.2.html">inotify_add_watch</a></b>(2)       2.6.13
       <b><a href="../man2/inotify_init.2.html">inotify_init</a></b>(2)            2.6.13
       <b><a href="../man2/inotify_init1.2.html">inotify_init1</a></b>(2)           2.6.27
       <b><a href="../man2/inotify_rm_watch.2.html">inotify_rm_watch</a></b>(2)        2.6.13
       <b><a href="../man2/io_cancel.2.html">io_cancel</a></b>(2)               2.6
       <b><a href="../man2/io_destroy.2.html">io_destroy</a></b>(2)              2.6
       <b><a href="../man2/io_getevents.2.html">io_getevents</a></b>(2)            2.6
       <b><a href="../man2/io_setup.2.html">io_setup</a></b>(2)                2.6
       <b><a href="../man2/io_submit.2.html">io_submit</a></b>(2)               2.6
       <b><a href="../man2/ioctl.2.html">ioctl</a></b>(2)                   1.0
       <b><a href="../man2/ioperm.2.html">ioperm</a></b>(2)                  1.0
       <b><a href="../man2/iopl.2.html">iopl</a></b>(2)                    1.0
       <b><a href="../man2/ioprio_get.2.html">ioprio_get</a></b>(2)              2.6.13
       <b><a href="../man2/ioprio_set.2.html">ioprio_set</a></b>(2)              2.6.13
       <b><a href="../man2/ipc.2.html">ipc</a></b>(2)                     1.0
       <b><a href="../man2/kcmp.2.html">kcmp</a></b>(2)                    3.5
       <b><a href="../man2/kern_features.2.html">kern_features</a></b>(2)           3.7           Sparc64
       <b><a href="../man2/kexec_file_load.2.html">kexec_file_load</a></b>(2)         3.17
       <b><a href="../man2/kexec_load.2.html">kexec_load</a></b>(2)              2.6.13
       <b><a href="../man2/keyctl.2.html">keyctl</a></b>(2)                  2.6.11
       <b><a href="../man2/kill.2.html">kill</a></b>(2)                    1.0
       <b><a href="../man2/lchown.2.html">lchown</a></b>(2)                  1.0           See <b><a href="../man2/chown.2.html">chown</a></b>(2) for
                                                version details
       <b><a href="../man2/lchown32.2.html">lchown32</a></b>(2)                2.4
       <b><a href="../man2/lgetxattr.2.html">lgetxattr</a></b>(2)               2.6; 2.4.18
       <b><a href="../man2/link.2.html">link</a></b>(2)                    1.0
       <b><a href="../man2/linkat.2.html">linkat</a></b>(2)                  2.6.16
       <b><a href="../man2/listen.2.html">listen</a></b>(2)                  2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/listxattr.2.html">listxattr</a></b>(2)               2.6; 2.4.18
       <b><a href="../man2/llistxattr.2.html">llistxattr</a></b>(2)              2.6; 2.4.18
       <b><a href="../man2/lookup_dcookie.2.html">lookup_dcookie</a></b>(2)          2.6
       <b><a href="../man2/lremovexattr.2.html">lremovexattr</a></b>(2)            2.6; 2.4.18
       <b><a href="../man2/lseek.2.html">lseek</a></b>(2)                   1.0
       <b><a href="../man2/lsetxattr.2.html">lsetxattr</a></b>(2)               2.6; 2.4.18
       <b><a href="../man2/lstat.2.html">lstat</a></b>(2)                   1.0
       <b><a href="../man2/lstat64.2.html">lstat64</a></b>(2)                 2.4
       <b><a href="../man2/madvise.2.html">madvise</a></b>(2)                 2.4
       <b><a href="../man2/mbind.2.html">mbind</a></b>(2)                   2.6.6

       <b><a href="../man2/memfd_create.2.html">memfd_create</a></b>(2)            3.17
       <b><a href="../man2/migrate_pages.2.html">migrate_pages</a></b>(2)           2.6.16
       <b><a href="../man2/mincore.2.html">mincore</a></b>(2)                 2.4
       <b><a href="../man2/mkdir.2.html">mkdir</a></b>(2)                   1.0
       <b><a href="../man2/mkdirat.2.html">mkdirat</a></b>(2)                 2.6.16
       <b><a href="../man2/mknod.2.html">mknod</a></b>(2)                   1.0
       <b><a href="../man2/mknodat.2.html">mknodat</a></b>(2)                 2.6.16
       <b><a href="../man2/mlock.2.html">mlock</a></b>(2)                   2.0
       <b><a href="../man2/mlock2.2.html">mlock2</a></b>(2)                  4.4
       <b><a href="../man2/mlockall.2.html">mlockall</a></b>(2)                2.0
       <b><a href="../man2/mmap.2.html">mmap</a></b>(2)                    1.0
       <b><a href="../man2/mmap2.2.html">mmap2</a></b>(2)                   2.4
       <b><a href="../man2/modify_ldt.2.html">modify_ldt</a></b>(2)              1.0
       <b><a href="../man2/mount.2.html">mount</a></b>(2)                   1.0
       <b><a href="../man2/move_pages.2.html">move_pages</a></b>(2)              2.6.18
       <b><a href="../man2/mprotect.2.html">mprotect</a></b>(2)                1.0
       <b><a href="../man2/mq_getsetattr.2.html">mq_getsetattr</a></b>(2)           2.6.6
       <b><a href="../man2/mq_notify.2.html">mq_notify</a></b>(2)               2.6.6
       <b><a href="../man2/mq_open.2.html">mq_open</a></b>(2)                 2.6.6
       <b><a href="../man2/mq_timedreceive.2.html">mq_timedreceive</a></b>(2)         2.6.6
       <b><a href="../man2/mq_timedsend.2.html">mq_timedsend</a></b>(2)            2.6.6
       <b><a href="../man2/mq_unlink.2.html">mq_unlink</a></b>(2)               2.6.6
       <b><a href="../man2/mremap.2.html">mremap</a></b>(2)                  2.0
       <b><a href="../man2/msgctl.2.html">msgctl</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/msgget.2.html">msgget</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/msgrcv.2.html">msgrcv</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/msgsnd.2.html">msgsnd</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/msync.2.html">msync</a></b>(2)                   2.0
       <b><a href="../man2/munlock.2.html">munlock</a></b>(2)                 2.0
       <b><a href="../man2/munlockall.2.html">munlockall</a></b>(2)              2.0
       <b><a href="../man2/munmap.2.html">munmap</a></b>(2)                  1.0
       <b><a href="../man2/name_to_handle_at.2.html">name_to_handle_at</a></b>(2)       2.6.39
       <b><a href="../man2/nanosleep.2.html">nanosleep</a></b>(2)               2.0
       <b><a href="../man2/nfsservctl.2.html">nfsservctl</a></b>(2)              2.2           Removed in 3.1
       <b><a href="../man2/nice.2.html">nice</a></b>(2)                    1.0
       <b><a href="../man2/oldfstat.2.html">oldfstat</a></b>(2)                1.0
       <b><a href="../man2/oldlstat.2.html">oldlstat</a></b>(2)                1.0
       <b><a href="../man2/oldolduname.2.html">oldolduname</a></b>(2)             1.0
       <b><a href="../man2/oldstat.2.html">oldstat</a></b>(2)                 1.0
       <b><a href="../man2/olduname.2.html">olduname</a></b>(2)                1.0
       <b><a href="../man2/open.2.html">open</a></b>(2)                    1.0
       <b><a href="../man2/open_by_handle_at.2.html">open_by_handle_at</a></b>(2)       2.6.39
       <b><a href="../man2/openat.2.html">openat</a></b>(2)                  2.6.16
       <b><a href="../man2/pause.2.html">pause</a></b>(2)                   1.0
       <b><a href="../man2/pciconfig_iobase.2.html">pciconfig_iobase</a></b>(2)        2.2.15; 2.4   Not on x86
       <b><a href="../man2/pciconfig_read.2.html">pciconfig_read</a></b>(2)          2.0.26; 2.2   Not on x86
       <b><a href="../man2/pciconfig_write.2.html">pciconfig_write</a></b>(2)         2.0.26; 2.2   Not on x86
       <b><a href="../man2/perf_event_open.2.html">perf_event_open</a></b>(2)         2.6.31        Was perf_counter_open() in
                                                2.6.31; renamed in 2.6.32
       <b><a href="../man2/personality.2.html">personality</a></b>(2)             1.2
       <b><a href="../man2/perfctr.2.html">perfctr</a></b>(2)                 2.2           Sparc; removed in 2.6.34
       <b><a href="../man2/perfmonctl.2.html">perfmonctl</a></b>(2)              2.4           ia64
       <b><a href="../man2/pipe.2.html">pipe</a></b>(2)                    1.0
       <b><a href="../man2/pipe2.2.html">pipe2</a></b>(2)                   2.6.27
       <b><a href="../man2/pivot_root.2.html">pivot_root</a></b>(2)              2.4
       <b><a href="../man2/pkey_alloc.2.html">pkey_alloc</a></b>(2)              4.8
       <b><a href="../man2/pkey_free.2.html">pkey_free</a></b>(2)               4.8
       <b><a href="../man2/pkey_mprotect.2.html">pkey_mprotect</a></b>(2)           4.8
       <b><a href="../man2/poll.2.html">poll</a></b>(2)                    2.0.36; 2.2
       <b><a href="../man2/ppc_rtas.2.html">ppc_rtas</a></b>(2)                2.6.2         PowerPC only
       <b><a href="../man2/ppc_swapcontext.2.html">ppc_swapcontext</a></b>(2)         2.6.3         PowerPC only
       <b><a href="../man2/ppoll.2.html">ppoll</a></b>(2)                   2.6.16
       <b><a href="../man2/prctl.2.html">prctl</a></b>(2)                   2.2
       <b><a href="../man2/pread64.2.html">pread64</a></b>(2)                               Added as "pread" in 2.2;
                                                renamed "pread64" in 2.6

       <b><a href="../man2/preadv.2.html">preadv</a></b>(2)                  2.6.30
       <b><a href="../man2/preadv2.2.html">preadv2</a></b>(2)                 4.6
       <b><a href="../man2/prlimit64.2.html">prlimit64</a></b>(2)               2.6.36
       <b><a href="../man2/process_vm_readv.2.html">process_vm_readv</a></b>(2)        3.2
       <b><a href="../man2/process_vm_writev.2.html">process_vm_writev</a></b>(2)       3.2
       <b><a href="../man2/pselect6.2.html">pselect6</a></b>(2)                2.6.16
       <b><a href="../man2/ptrace.2.html">ptrace</a></b>(2)                  1.0
       <b><a href="../man2/pwrite64.2.html">pwrite64</a></b>(2)                              Added as "pwrite" in 2.2;
                                                renamed "pwrite64" in 2.6
       <b><a href="../man2/pwritev.2.html">pwritev</a></b>(2)                 2.6.30
       <b><a href="../man2/pwritev2.2.html">pwritev2</a></b>(2)                4.6
       <b><a href="../man2/query_module.2.html">query_module</a></b>(2)            2.2           Removed in 2.6
       <b><a href="../man2/quotactl.2.html">quotactl</a></b>(2)                1.0
       <b><a href="../man2/read.2.html">read</a></b>(2)                    1.0
       <b><a href="../man2/readahead.2.html">readahead</a></b>(2)               2.4.13
       <b><a href="../man2/readdir.2.html">readdir</a></b>(2)                 1.0
       <b><a href="../man2/readlink.2.html">readlink</a></b>(2)                1.0
       <b><a href="../man2/readlinkat.2.html">readlinkat</a></b>(2)              2.6.16
       <b><a href="../man2/readv.2.html">readv</a></b>(2)                   2.0
       <b><a href="../man2/reboot.2.html">reboot</a></b>(2)                  1.0
       <b><a href="../man2/recv.2.html">recv</a></b>(2)                    2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/recvfrom.2.html">recvfrom</a></b>(2)                2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/recvmsg.2.html">recvmsg</a></b>(2)                 2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/recvmmsg.2.html">recvmmsg</a></b>(2)                2.6.33
       <b><a href="../man2/remap_file_pages.2.html">remap_file_pages</a></b>(2)        2.6           Deprecated since 3.16
       <b><a href="../man2/removexattr.2.html">removexattr</a></b>(2)             2.6; 2.4.18
       <b><a href="../man2/rename.2.html">rename</a></b>(2)                  1.0
       <b><a href="../man2/renameat.2.html">renameat</a></b>(2)                2.6.16
       <b><a href="../man2/renameat2.2.html">renameat2</a></b>(2)               3.15
       <b><a href="../man2/request_key.2.html">request_key</a></b>(2)             2.6.11
       <b><a href="../man2/restart_syscall.2.html">restart_syscall</a></b>(2)         2.6
       <b><a href="../man2/rmdir.2.html">rmdir</a></b>(2)                   1.0
       <b><a href="../man2/rt_sigaction.2.html">rt_sigaction</a></b>(2)            2.2
       <b><a href="../man2/rt_sigpending.2.html">rt_sigpending</a></b>(2)           2.2
       <b><a href="../man2/rt_sigprocmask.2.html">rt_sigprocmask</a></b>(2)          2.2
       <b><a href="../man2/rt_sigqueueinfo.2.html">rt_sigqueueinfo</a></b>(2)         2.2
       <b><a href="../man2/rt_sigreturn.2.html">rt_sigreturn</a></b>(2)            2.2
       <b><a href="../man2/rt_sigsuspend.2.html">rt_sigsuspend</a></b>(2)           2.2
       <b><a href="../man2/rt_sigtimedwait.2.html">rt_sigtimedwait</a></b>(2)         2.2
       <b><a href="../man2/rt_tgsigqueueinfo.2.html">rt_tgsigqueueinfo</a></b>(2)       2.6.31
       <b><a href="../man2/s390_runtime_instr.2.html">s390_runtime_instr</a></b>(2)      3.7           s390 only
       <b><a href="../man2/s390_pci_mmio_read.2.html">s390_pci_mmio_read</a></b>(2)      3.19          s390 only
       <b><a href="../man2/s390_pci_mmio_write.2.html">s390_pci_mmio_write</a></b>(2)     3.19          s390 only
       <b><a href="../man2/sched_get_priority_max.2.html">sched_get_priority_max</a></b>(2)  2.0
       <b><a href="../man2/sched_get_priority_min.2.html">sched_get_priority_min</a></b>(2)  2.0
       <b><a href="../man2/sched_getaffinity.2.html">sched_getaffinity</a></b>(2)       2.6
       <b><a href="../man2/sched_getattr.2.html">sched_getattr</a></b>(2)           3.14
       <b><a href="../man2/sched_getparam.2.html">sched_getparam</a></b>(2)          2.0
       <b><a href="../man2/sched_getscheduler.2.html">sched_getscheduler</a></b>(2)      2.0
       <b><a href="../man2/sched_rr_get_interval.2.html">sched_rr_get_interval</a></b>(2)   2.0
       <b><a href="../man2/sched_setaffinity.2.html">sched_setaffinity</a></b>(2)       2.6
       <b><a href="../man2/sched_setattr.2.html">sched_setattr</a></b>(2)           3.14
       <b><a href="../man2/sched_setparam.2.html">sched_setparam</a></b>(2)          2.0
       <b><a href="../man2/sched_setscheduler.2.html">sched_setscheduler</a></b>(2)      2.0
       <b><a href="../man2/sched_yield.2.html">sched_yield</a></b>(2)             2.0
       <b><a href="../man2/seccomp.2.html">seccomp</a></b>(2)                 3.17
       <b><a href="../man2/select.2.html">select</a></b>(2)                  1.0
       <b><a href="../man2/semctl.2.html">semctl</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/semget.2.html">semget</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/semop.2.html">semop</a></b>(2)                   2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/semtimedop.2.html">semtimedop</a></b>(2)              2.6; 2.4.22
       <b><a href="../man2/send.2.html">send</a></b>(2)                    2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/sendfile.2.html">sendfile</a></b>(2)                2.2
       <b><a href="../man2/sendfile64.2.html">sendfile64</a></b>(2)              2.6; 2.4.19
       <b><a href="../man2/sendmmsg.2.html">sendmmsg</a></b>(2)                3.0

       <b><a href="../man2/sendmsg.2.html">sendmsg</a></b>(2)                 2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/sendto.2.html">sendto</a></b>(2)                  2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/set_mempolicy.2.html">set_mempolicy</a></b>(2)           2.6.6
       <b><a href="../man2/set_robust_list.2.html">set_robust_list</a></b>(2)         2.6.17
       <b><a href="../man2/set_thread_area.2.html">set_thread_area</a></b>(2)         2.6
       <b><a href="../man2/set_tid_address.2.html">set_tid_address</a></b>(2)         2.6
       <b><a href="../man2/setdomainname.2.html">setdomainname</a></b>(2)           1.0
       <b><a href="../man2/setfsgid.2.html">setfsgid</a></b>(2)                1.2
       <b><a href="../man2/setfsgid32.2.html">setfsgid32</a></b>(2)              2.4
       <b><a href="../man2/setfsuid.2.html">setfsuid</a></b>(2)                1.2
       <b><a href="../man2/setfsuid32.2.html">setfsuid32</a></b>(2)              2.4
       <b><a href="../man2/setgid.2.html">setgid</a></b>(2)                  1.0
       <b><a href="../man2/setgid32.2.html">setgid32</a></b>(2)                2.4
       <b><a href="../man2/setgroups.2.html">setgroups</a></b>(2)               1.0
       <b><a href="../man2/setgroups32.2.html">setgroups32</a></b>(2)             2.4
       <b><a href="../man2/sethostname.2.html">sethostname</a></b>(2)             1.0
       <b><a href="../man2/setitimer.2.html">setitimer</a></b>(2)               1.0
       <b><a href="../man2/setns.2.html">setns</a></b>(2)                   3.0
       <b><a href="../man2/setpgid.2.html">setpgid</a></b>(2)                 1.0
       <b><a href="../man2/setpriority.2.html">setpriority</a></b>(2)             1.0
       <b><a href="../man2/setregid.2.html">setregid</a></b>(2)                1.0
       <b><a href="../man2/setregid32.2.html">setregid32</a></b>(2)              2.4
       <b><a href="../man2/setresgid.2.html">setresgid</a></b>(2)               2.2
       <b><a href="../man2/setresgid32.2.html">setresgid32</a></b>(2)             2.4
       <b><a href="../man2/setresuid.2.html">setresuid</a></b>(2)               2.2
       <b><a href="../man2/setresuid32.2.html">setresuid32</a></b>(2)             2.4
       <b><a href="../man2/setreuid.2.html">setreuid</a></b>(2)                1.0
       <b><a href="../man2/setreuid32.2.html">setreuid32</a></b>(2)              2.4
       <b><a href="../man2/setrlimit.2.html">setrlimit</a></b>(2)               1.0
       <b><a href="../man2/setsid.2.html">setsid</a></b>(2)                  1.0
       <b><a href="../man2/setsockopt.2.html">setsockopt</a></b>(2)              2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/settimeofday.2.html">settimeofday</a></b>(2)            1.0
       <b><a href="../man2/setuid.2.html">setuid</a></b>(2)                  1.0
       <b><a href="../man2/setuid32.2.html">setuid32</a></b>(2)                2.4
       <b><a href="../man2/setup.2.html">setup</a></b>(2)                   1.0           Removed in 2.2
       <b><a href="../man2/setxattr.2.html">setxattr</a></b>(2)                2.6; 2.4.18
       <b><a href="../man2/sgetmask.2.html">sgetmask</a></b>(2)                1.0
       <b><a href="../man2/shmat.2.html">shmat</a></b>(2)                   2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/shmctl.2.html">shmctl</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/shmdt.2.html">shmdt</a></b>(2)                   2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/shmget.2.html">shmget</a></b>(2)                  2.0           See notes on <b><a href="../man2/ipc.2.html">ipc</a></b>(2)
       <b><a href="../man2/shutdown.2.html">shutdown</a></b>(2)                2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/sigaction.2.html">sigaction</a></b>(2)               1.0
       <b><a href="../man2/sigaltstack.2.html">sigaltstack</a></b>(2)             2.2
       <b><a href="../man2/signal.2.html">signal</a></b>(2)                  1.0
       <b><a href="../man2/signalfd.2.html">signalfd</a></b>(2)                2.6.22
       <b><a href="../man2/signalfd4.2.html">signalfd4</a></b>(2)               2.6.27
       <b><a href="../man2/sigpending.2.html">sigpending</a></b>(2)              1.0
       <b><a href="../man2/sigprocmask.2.html">sigprocmask</a></b>(2)             1.0
       <b><a href="../man2/sigreturn.2.html">sigreturn</a></b>(2)               1.0
       <b><a href="../man2/sigsuspend.2.html">sigsuspend</a></b>(2)              1.0
       <b><a href="../man2/socket.2.html">socket</a></b>(2)                  2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)              1.0
       <b><a href="../man2/socketpair.2.html">socketpair</a></b>(2)              2.0           See notes on <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)
       <b><a href="../man2/splice.2.html">splice</a></b>(2)                  2.6.17
       <b><a href="../man2/spu_create.2.html">spu_create</a></b>(2)              2.6.16        PowerPC only
       <b><a href="../man2/spu_run.2.html">spu_run</a></b>(2)                 2.6.16        PowerPC only
       <b><a href="../man2/ssetmask.2.html">ssetmask</a></b>(2)                1.0
       <b><a href="../man2/stat.2.html">stat</a></b>(2)                    1.0
       <b><a href="../man2/stat64.2.html">stat64</a></b>(2)                  2.4
       <b><a href="../man2/statfs.2.html">statfs</a></b>(2)                  1.0
       <b><a href="../man2/statfs64.2.html">statfs64</a></b>(2)                2.6
       <b><a href="../man2/stime.2.html">stime</a></b>(2)                   1.0
       <b><a href="../man2/subpage_prot.2.html">subpage_prot</a></b>(2)            2.6.25        PowerPC only
       <b><a href="../man2/swapoff.2.html">swapoff</a></b>(2)                 1.0

       <b><a href="../man2/swapon.2.html">swapon</a></b>(2)                  1.0
       <b><a href="../man2/symlink.2.html">symlink</a></b>(2)                 1.0
       <b><a href="../man2/symlinkat.2.html">symlinkat</a></b>(2)               2.6.16
       <b><a href="../man2/sync.2.html">sync</a></b>(2)                    1.0
       <b><a href="../man2/sync_file_range.2.html">sync_file_range</a></b>(2)         2.6.17
       <b><a href="../man2/sync_file_range2.2.html">sync_file_range2</a></b>(2)        2.6.22
       <b><a href="../man2/syncfs.2.html">syncfs</a></b>(2)                  2.6.39
       <b><a href="../man2/sysfs.2.html">sysfs</a></b>(2)                   1.2
       <b><a href="../man2/sysinfo.2.html">sysinfo</a></b>(2)                 1.0
       <b><a href="../man2/syslog.2.html">syslog</a></b>(2)                  1.0
       <b><a href="../man2/tee.2.html">tee</a></b>(2)                     2.6.17
       <b><a href="../man2/tgkill.2.html">tgkill</a></b>(2)                  2.6
       <b><a href="../man2/time.2.html">time</a></b>(2)                    1.0
       <b><a href="../man2/timer_create.2.html">timer_create</a></b>(2)            2.6
       <b><a href="../man2/timer_delete.2.html">timer_delete</a></b>(2)            2.6
       <b><a href="../man2/timer_getoverrun.2.html">timer_getoverrun</a></b>(2)        2.6
       <b><a href="../man2/timer_gettime.2.html">timer_gettime</a></b>(2)           2.6
       <b><a href="../man2/timer_settime.2.html">timer_settime</a></b>(2)           2.6
       <b><a href="../man2/timerfd_create.2.html">timerfd_create</a></b>(2)          2.6.25
       <b><a href="../man2/timerfd_gettime.2.html">timerfd_gettime</a></b>(2)         2.6.25
       <b><a href="../man2/timerfd_settime.2.html">timerfd_settime</a></b>(2)         2.6.25
       <b><a href="../man2/times.2.html">times</a></b>(2)                   1.0
       <b><a href="../man2/tkill.2.html">tkill</a></b>(2)                   2.6; 2.4.22
       <b><a href="../man2/truncate.2.html">truncate</a></b>(2)                1.0
       <b><a href="../man2/truncate64.2.html">truncate64</a></b>(2)              2.4
       <b><a href="../man2/ugetrlimit.2.html">ugetrlimit</a></b>(2)              2.4
       <b><a href="../man2/umask.2.html">umask</a></b>(2)                   1.0
       <b><a href="../man2/umount.2.html">umount</a></b>(2)                  1.0
       <b><a href="../man2/umount2.2.html">umount2</a></b>(2)                 2.2
       <b><a href="../man2/uname.2.html">uname</a></b>(2)                   1.0
       <b><a href="../man2/unlink.2.html">unlink</a></b>(2)                  1.0
       <b><a href="../man2/unlinkat.2.html">unlinkat</a></b>(2)                2.6.16
       <b><a href="../man2/unshare.2.html">unshare</a></b>(2)                 2.6.16
       <b><a href="../man2/uselib.2.html">uselib</a></b>(2)                  1.0
       <b><a href="../man2/ustat.2.html">ustat</a></b>(2)                   1.0
       <b><a href="../man2/userfaultfd.2.html">userfaultfd</a></b>(2)             4.2
       <b><a href="../man2/utime.2.html">utime</a></b>(2)                   1.0
       <b><a href="../man2/utimensat.2.html">utimensat</a></b>(2)               2.6.22
       <b><a href="../man2/utimes.2.html">utimes</a></b>(2)                  2.2
       <b><a href="../man2/utrap_install.2.html">utrap_install</a></b>(2)           2.2           Sparc only
       <b><a href="../man2/vfork.2.html">vfork</a></b>(2)                   2.2
       <b><a href="../man2/vhangup.2.html">vhangup</a></b>(2)                 1.0
       <b><a href="../man2/vm86old.2.html">vm86old</a></b>(2)                 1.0           Was "vm86"; renamed in
                                                2.0.28/2.2
       <b><a href="../man2/vm86.2.html">vm86</a></b>(2)                    2.0.28; 2.2
       <b><a href="../man2/vmsplice.2.html">vmsplice</a></b>(2)                2.6.17
       <b><a href="../man2/wait4.2.html">wait4</a></b>(2)                   1.0
       <b><a href="../man2/waitid.2.html">waitid</a></b>(2)                  2.6.10
       <b><a href="../man2/waitpid.2.html">waitpid</a></b>(2)                 1.0
       <b><a href="../man2/write.2.html">write</a></b>(2)                   1.0
       <b><a href="../man2/writev.2.html">writev</a></b>(2)                  2.0

       On  many  platforms, including x86-32, socket calls are all multiplexed
       (via glibc  wrapper  functions)  through  <b><a href="../man2/socketcall.2.html">socketcall</a></b>(2)  and  similarly
       System V IPC calls are multiplexed through <b><a href="../man2/ipc.2.html">ipc</a></b>(2).

       Although  slots  are  reserved  for  them in the system call table, the
       following system calls are not  implemented  in  the  standard  kernel:
       <b><a href="../man2/afs_syscall.2.html">afs_syscall</a></b>(2),   <b><a href="../man2/break.2.html">break</a></b>(2),  <b><a href="../man2/ftime.2.html">ftime</a></b>(2),  <b><a href="../man2/getpmsg.2.html">getpmsg</a></b>(2),  <b><a href="../man2/gtty.2.html">gtty</a></b>(2),  <b><a href="../man2/idle.2.html">idle</a></b>(2),
       <b><a href="../man2/lock.2.html">lock</a></b>(2), <b><a href="../man2/madvise1.2.html">madvise1</a></b>(2), <b><a href="../man2/mpx.2.html">mpx</a></b>(2), <b><a href="../man2/phys.2.html">phys</a></b>(2), <b><a href="../man2/prof.2.html">prof</a></b>(2), <b><a href="../man2/profil.2.html">profil</a></b>(2),  <b><a href="../man2/putpmsg.2.html">putpmsg</a></b>(2),
       <b><a href="../man2/security.2.html">security</a></b>(2),  <b><a href="../man2/stty.2.html">stty</a></b>(2),  <b><a href="../man2/tuxcall.2.html">tuxcall</a></b>(2), <b><a href="../man2/ulimit.2.html">ulimit</a></b>(2), and <b><a href="../man2/vserver.2.html">vserver</a></b>(2) (see also
       <b><a href="../man2/unimplemented.2.html">unimplemented</a></b>(2)).  However, <b><a href="../man3/ftime.3.html">ftime</a></b>(3), <b><a href="../man3/profil.3.html">profil</a></b>(3), and  <b><a href="../man3/ulimit.3.html">ulimit</a></b>(3)  exist
       as  library  routines.   The  slot  for  <b><a href="../man2/phys.2.html">phys</a></b>(2) is in use since kernel
       2.1.116  for  <b><a href="../man2/umount.2.html">umount</a></b>(2);  <b><a href="../man2/phys.2.html">phys</a></b>(2)  will  never  be  implemented.    The
       <b><a href="../man2/getpmsg.2.html">getpmsg</a></b>(2)  and  <b><a href="../man2/putpmsg.2.html">putpmsg</a></b>(2)  calls  are  for kernels patched to support
       STREAMS, and may never be in the standard kernel.

       There was briefly  <b><a href="../man2/set_zone_reclaim.2.html">set_zone_reclaim</a></b>(2),  added  in  Linux  2.6.13,  and
       removed in 2.6.16; this system call was never available to user space.

</pre><h4><b>NOTES</b></h4><pre>
       Roughly  speaking,  the  code  belonging to the system call with number
       __NR_xxx defined in <u>/usr/include/asm/unistd.h</u> can be found in the Linux
       kernel  source  in the routine <u>sys_xxx</u>().  (The dispatch table for i386
       can be found in  <u>/usr/src/linux/arch/i386/kernel/entry.S</u>.)   There  are
       many  exceptions,  however,  mostly  because  older  system  calls were
       superseded  by  newer  ones,  and  this  has  been   treated   somewhat
       unsystematically.    On  platforms  with  proprietary  operating-system
       emulation, such as parisc, sparc, sparc64, and alpha,  there  are  many
       additional  system  calls;  mips64  also  contains a full set of 32-bit
       system calls.

       Over time, changes to the interfaces of some  system  calls  have  been
       necessary.   One  reason  for such changes was the need to increase the
       size of structures or scalar values passed to the system call.  Because
       of  these  changes, certain architectures (notably, longstanding 32-bit
       architectures such as i386) now have various groups of  related  system
       calls  (e.g.,  <b><a href="../man2/truncate.2.html">truncate</a></b>(2)  and  <b><a href="../man2/truncate64.2.html">truncate64</a></b>(2))  which  perform similar
       tasks, but which vary in details such as the size of  their  arguments.
       (As  noted  earlier,  applications  are  generally unaware of this: the
       glibc wrapper functions do some work to ensure that  the  right  system
       call  is  invoked,  and  that  ABI  compatibility  is preserved for old
       binaries.)  Examples of systems calls that exist in  multiple  versions
       are the following:

       *  By  now  there  are  three different versions of <b><a href="../man2/stat.2.html">stat</a></b>(2): <u>sys_stat</u>()
          (slot   <b>__</b><u>NR_oldstat</u>),   <u>sys_newstat</u>()   (slot    <b>__</b><u>NR_stat</u>),    and
          <u>sys_stat64</u>()  (slot  <b>__</b><u>NR_stat64</u>),  with  the  last  being  the most
          current.  A similar story applies for <b><a href="../man2/lstat.2.html">lstat</a></b>(2) and <b><a href="../man2/fstat.2.html">fstat</a></b>(2).

       *  Similarly,  the   defines   <b>__</b><u>NR_oldolduname</u>,   <b>__</b><u>NR_olduname</u>,   and
          <b>__</b><u>NR_uname</u>  refer  to  the  routines <u>sys_olduname</u>(), <u>sys_uname</u>() and
          <u>sys_newuname</u>().

       *  In Linux 2.0, a new version of <b><a href="../man2/vm86.2.html">vm86</a></b>(2) appeared, with  the  old  and
          the new kernel routines being named <u>sys_vm86old</u>() and <u>sys_vm86</u>().

       *  In  Linux  2.4, a new version of <b><a href="../man2/getrlimit.2.html">getrlimit</a></b>(2) appeared, with the old
          and the new kernel routines being  named  <u>sys_old_getrlimit</u>()  (slot
          <b>__</b><u>NR_getrlimit</u>) and <u>sys_getrlimit</u>() (slot <b>__</b><u>NR_ugetrlimit</u>).

       *  Linux  2.4  increased  the  size of user and group IDs from 16 to 32
          bits.  To support this change, a range of system  calls  were  added
          (e.g.,  <b><a href="../man2/chown32.2.html">chown32</a></b>(2),  <b><a href="../man2/getuid32.2.html">getuid32</a></b>(2),  <b><a href="../man2/getgroups32.2.html">getgroups32</a></b>(2),  <b><a href="../man2/setresuid32.2.html">setresuid32</a></b>(2)),
          superseding earlier calls of the same name without the "32" suffix.

       *  Linux 2.4 added support for applications on 32-bit architectures  to
          access large files (i.e., files for which the sizes and file offsets
          can't  be  represented  in  32  bits.)   To  support  this   change,
          replacements  were  required  for  system  calls that deal with file
          offsets and sizes.  Thus the  following  system  calls  were  added:
          <b><a href="../man2/fcntl64.2.html">fcntl64</a></b>(2),  <b><a href="../man2/getdents64.2.html">getdents64</a></b>(2),  <b><a href="../man2/stat64.2.html">stat64</a></b>(2),  <b><a href="../man2/statfs64.2.html">statfs64</a></b>(2), <b><a href="../man2/truncate64.2.html">truncate64</a></b>(2),
          and their analogs that work with file descriptors or symbolic links.
          These system calls supersede the older system calls which, except in
          the case of the "stat" calls, have the same name  without  the  "64"
          suffix.

          On  newer  platforms  that  only  have 64-bit file access and 32-bit
          UIDs/GIDs (e.g., alpha, ia64, s390x, x86-64), there is just a single
          version  of  the UID/GID and file access system calls.  On platforms
          (typically, 32-bit platforms) where the *64 and *32 calls exist, the
          other versions are obsolete.

       *  The  <u>rt_sig*</u>  calls were added in kernel 2.2 to support the addition
          of real-time signals (see <b><a href="../man7/signal.7.html">signal</a></b>(7)).  These system calls  supersede
          the older system calls of the same name without the "rt_" prefix.

       *  The  <b><a href="../man2/select.2.html">select</a></b>(2)  and <b><a href="../man2/mmap.2.html">mmap</a></b>(2) system calls use five or more arguments,
          which caused problems in the way argument passing on the  i386  used
          to be set up.  Thus, while other architectures have <u>sys_select</u>() and
          <u>sys_mmap</u>() corresponding to <b>__</b><u>NR_select</u> and <b>__</b><u>NR_mmap</u>, on  i386  one
          finds <u>old_select</u>() and <u>old_mmap</u>() (routines that use a pointer to an
          argument block) instead.  These days passing five arguments is not a
          problem  any  more,  and there is a <b>__</b><u>NR</u><b>__</b><u>newselect</u> that corresponds
          directly to <u>sys_select</u>() and similarly <b>__</b><u>NR_mmap2</u>.

</pre><h4><b>SEE</b> <b>ALSO</b></h4><pre>
       <b><a href="../man2/intro.2.html">intro</a></b>(2), <b><a href="../man2/syscall.2.html">syscall</a></b>(2), <b><a href="../man2/unimplemented.2.html">unimplemented</a></b>(2), <b><a href="../man3/errno.3.html">errno</a></b>(3), <b><a href="../man7/libc.7.html">libc</a></b>(7), <b><a href="../man7/vdso.7.html">vdso</a></b>(7)

</pre><h4><b>COLOPHON</b></h4><pre>
       This page is part of release 4.09 of the Linux  <u>man-pages</u>  project.   A
       description  of  the project, information about reporting bugs, and the
       latest    version    of    this    page,    can     be     found     at
       https://www.kernel.org/doc/man-pages/.
</pre>
</pre>
                </div>
            </div>
            <div id="copyright">
                <p>
                Powered by the <a href="https://launchpad.net/ubuntu-manpage-repository">Ubuntu Manpage Repository</a> generator
                maintained by <a href="http://blog.dustinkirkland.com/">Dustin Kirkland</a><br />
                &copy; 2010 Canonical Ltd. Ubuntu and Canonical are registered trademarks of Canonical Ltd.
                </p>
            </div>
        </div>
    </body>
</html>

{:/}
