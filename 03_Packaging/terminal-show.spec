Name:           terminal-show
Version:        0.0.1
Release:        alt1
Summary:        Simple ncurses text viewer
License:        MIT
Group:          Other
Source:         %name-%version.tar.gz

BuildRequires:  gcc make libncurses-devel

%description
A simple terminal text viewer using ncurses.
Space scrolls the text, Escape exits.

%prep
%setup -q

%build
make CFLAGS="%optflags -std=c11 -Wall -Wextra"

%install
make install DESTDIR="%buildroot" BINDIR="%_bindir"

%files
%_bindir/Show
