# The binary is stripped by qmake's install step, so there is no debuginfo
# to collect; disable the (empty) debug subpackages.
%global debug_package %{nil}

Name:           softprojector
Version:        2.2
Release:        1%{?dist}
Summary:        Open source church presentation software

License:        GPLv3
URL:            http://softprojector.org
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtdeclarative-devel
BuildRequires:  qt6-qtmultimedia-devel
BuildRequires:  qt6-qttools-devel
BuildRequires:  desktop-file-utils

Requires:       qt6-qtmultimedia
Requires:       qt6-qtdeclarative

%description
SoftProjector displays song lyrics, Bible verses, announcements and media
during church services. It supports a separate projector/display screen and
uses Qt 6's FFmpeg-based multimedia backend for video and audio playback.

%prep
%autosetup -n %{name}-%{version}

%build
mkdir -p build
cd build
qmake6 ../src/softProjector.pro PREFIX=%{_prefix}
%make_build

%install
cd build
make install INSTALL_ROOT=%{buildroot}
desktop-file-validate %{buildroot}%{_datadir}/applications/softprojector.desktop

%files
%license LICENSE
%doc README.md BUILDING_LINUX.md
%{_bindir}/SoftProjector
%{_datadir}/applications/softprojector.desktop
%{_datadir}/icons/hicolor/128x128/apps/softprojector.png

%changelog
* Sat Jul 25 2026 SoftProjector Linux port - 2.2-1
- Initial RPM packaging for the Qt6 Linux build.
