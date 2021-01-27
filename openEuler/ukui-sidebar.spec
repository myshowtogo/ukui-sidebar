%define debug_package %{nil}
Name:           ukui-sidebar
Version:        3.1.0
Release:        1
Summary:        parallels toolbox for UKUI
License:        GPL-3+
URL:            http://www.ukui.org
Source0:        %{name}-%{version}.tar.gz

BuildRequires: glib2-devel
BuildRequires: qt5-qtbase-devel
BuildRequires: qt5-qtsvg-devel
BuildRequires: qt5-qtmultimedia-devel
BuildRequires: qt5-qttools-devel
BuildRequires: gsettings-qt-devel
BuildRequires: dconf-devel
BuildRequires: libX11-devel
BuildRequires: kf5-kwindowsystem-devel
BuildRequires: libXtst-devel

Requires: glib2-devel
Requires: qt5-qtbase-devel
Requires: qt5-qtsvg-devel
Requires: qt5-qtmultimedia-devel
Requires: qt5-qttools-devel
Requires: gsettings-qt-devel
Requires: dconf-devel

%description
 The ukui-sidebar is mainly used in the desktop operating system.
 It pops up from the right side of the desktop in the form of a tray,
 displaying some application notification messages and some cutting
 storage information.

%prep
%setup -q

%build
mkdir build && cd build
qmake-qt5 ..
make

%install
rm -rf $RPM_BUILD_ROOT
cd %{_builddir}/%{name}-%{version}/build
make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf $RPM_BUILD_ROOT

%postun
set -e

home="/home/"

db_file="/.config/ukui/Clock_database.db"
feedback="/.config/ukui/url.conf"

for file in $(ls /home)
do
  db_file_path=${home}${file}${db_file}  
  feedback_path=${home}${file}${feedback}  
  if [  -f "$db_file_path" ]; then
 	rm "$db_file_path"
  fi
  if [  -f "$feedback_path" ]; then
 	rm "$feedback_path"
  fi
done

%files 
%doc debian/copyright debian/changelog
%{_sysconfdir}/xdg/autostart/ukui-sidebar.desktop
%{_bindir}/*
%{_libdir}/ukui-sidebar
%{_datadir}/ukui-sidebar*
%{_datadir}/applications/*
%{_datadir}/ukui-clock/

%changelog
* Mon Oct 27 2021 lvhan <lvhan@kylinos.cn> - 3.1.0-1
- update to upstream version 3.1.0-1

* Mon Oct 26 2020 douyan <douyan@kylinos.cn> - 3.0.1-1
- update to upstream version 3.0.0-1+1103

* Thu Jul 9 2020 douyan <douyan@kylinos.cn> - 1.1.2-1
- Init package for openEuler
