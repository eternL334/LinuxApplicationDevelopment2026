Name: ioccc-kurdyukov
Version: 2025
Release: Lollipop

Summary: Arithmetic expression finder from IOCCC 2025
License: CC-BY-SA-4.0
Group: Sciences/Mathematics
Url: https://www.ioccc.org/2025/kurdyukov/index.html

Source0: 2025_kurdyukov.tar.bz2
Source1: LICENSE

BuildRequires: gcc glibc-devel

%description
Magic Equations by Ilya Kurdyukov, an IOCCC 2025 entry.
Finds short arithmetic expressions representing target numbers
using a supplied set of positive integers.

%prep
%setup -q -c -n %{name}-%{version}
cp -p %{SOURCE1} LICENSE

%build
gcc %optflags -std=gnu17 2025/kurdyukov/prog.c -o %{name}

%install
install -Dpm755 %{name} %{buildroot}%{_bindir}/%{name}

%files
%doc 2025/kurdyukov/README.md LICENSE
%{_bindir}/%{name}

%changelog
* Sat Oct 10 2026 Danila Aristarkhov <dan_arist04@mail.ru> 2025-Lollipop
- Initial build.
