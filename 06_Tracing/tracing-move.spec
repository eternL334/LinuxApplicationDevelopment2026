Name: tracing-move
Version: 1.0
Release: alt1
Summary: Simple file mover with LD_PRELOAD protection
License: MIT
Group: File tools
Source: %name-%version.tar.gz
BuildRequires: gcc make glibc-devel strace

%description
Copy a file and remove the original on success.
The safemove wrapper protects filenames containing PROTECT from removal.

%prep
%setup -q

%build
make CFLAGS="%optflags -std=gnu11 -Wall -Wextra" BINDIR=%_bindir LIBDIR=%_libdir

%check
make test

%install
make install DESTDIR=%buildroot BINDIR=%_bindir LIBDIR=%_libdir

%files
%doc README
%_bindir/move
%_bindir/safemove
%_libdir/libprotect.so
