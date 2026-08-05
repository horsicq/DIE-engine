Name:           detect-it-easy
Version:        3.21
Release:        1%{?dist}
Summary:        Program for determining types of files

License:        MIT
URL:            https://github.com/horsicq/DIE-engine
Source0:        https://github.com/horsicq/DIE-engine/releases/download/%{version}/die_sourcecode_%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qtscript-devel
BuildRequires:  qt5-linguist
BuildRequires:  desktop-file-utils

%define debug_package %{nil}

%description
Detect It Easy (DIE) is a program for determining types of files.

%prep
%autosetup -n die_sourcecode_%{version}

%build
chmod a+x configure
export CXXFLAGS="%{optflags}"
export CFLAGS="%{optflags}"
./configure --prefix=%{_prefix} --bindir=%{_bindir}
%make_build

%install
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/applications
mkdir -p %{buildroot}%{_datadir}/icons/hicolor/256x256/apps
mkdir -p %{buildroot}%{_prefix}/lib/detect-it-easy/signatures

install -m 0755 build/release/die %{buildroot}%{_bindir}/
install -m 0755 build/release/diec %{buildroot}%{_bindir}/
install -m 0755 build/release/diel %{buildroot}%{_bindir}/

# icon
install -m 0644 LINUX/hicolor/256x256/apps/io.github.horsicq.detect-it-easy.png %{buildroot}%{_datadir}/icons/hicolor/256x256/apps/detect-it-easy.png

# .desktop file
cat <<EOF > %{buildroot}%{_datadir}/applications/detect-it-easy.desktop
[Desktop Entry]
Version=1.0
Type=Application
Name=Detect It Easy
Comment=Advanced file analyzer
Exec=die
Icon=detect-it-easy
Terminal=false
Categories=Development;Utility;
Keywords=pe;elf;binary;bin;die;
EOF

desktop-file-validate %{buildroot}%{_datadir}/applications/detect-it-easy.desktop

cp -Rf XStyles/qss/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -f signatures/crypto.db %{buildroot}%{_prefix}/lib/detect-it-easy/signatures/
cp -Rf XInfoDB/info/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf Detect-It-Easy/db/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf XYara/yara_rules/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf XPEID/peid/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf Detect-It-Easy/db_extra/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf Detect-It-Easy/db_custom/ %{buildroot}%{_prefix}/lib/detect-it-easy/
cp -Rf images/ %{buildroot}%{_prefix}/lib/detect-it-easy/

find %{buildroot}%{_prefix}/lib/detect-it-easy/ -type f -exec chmod 0644 {} +

ln -s detect-it-easy %{buildroot}%{_prefix}/lib/die

%files
%{_bindir}/die
%{_bindir}/diec
%{_bindir}/diel
%{_datadir}/applications/detect-it-easy.desktop
%{_datadir}/icons/hicolor/256x256/apps/detect-it-easy.png
%{_prefix}/lib/detect-it-easy/
%{_prefix}/lib/die

%changelog
* Wed Jul 29 2026 build <abdullohraupov.work@gmail.com> - 3.21-1
- See changelog at changelog.txt
