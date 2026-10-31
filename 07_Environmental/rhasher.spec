Name: rhasher
Version: 1.0
Release: alt1
Summary: Simple interactive hash calculator
License: MIT
Group: File tools
Source: %name-%version.tar.gz
BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake gcc make glibc-devel librhash-devel libreadline-devel coreutils diffutils

%description
Compute MD5, SHA1 and TTH hashes of files and strings in hex or Base64.
Readline support is optional.

%prep
%setup -q

%build
%cmake -DWITH_READLINE=ON -DBUILD_TESTING=ON
%cmake_build

%install
%cmake_install

%check
ctest --test-dir "%_cmake__builddir" --output-on-failure

%files
%doc README
%_bindir/rhasher
