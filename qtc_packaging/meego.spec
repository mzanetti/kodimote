Name: xbmcremote
Summary: A remote control for xbmc written in Qt with QML
Version: 0.1.0
Release: 1
License: <Enter your application's license here>
Group: <Set your application's group here>
%description
<Insert longer, multi-line description
here.>

%prep
%setup -q

%build
# You can leave this empty for use with Qt Creator.
%install
rm -rf %{buildroot}
make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf %{buildroot}

BuildRequires: 
# %define _unpackaged_files_terminate_build 0
%files
%defattr(-,root,root,-)/usr
/opt
# Add additional files to be included in the package here.
%pre
# Add pre-install scripts here.%post
/sbin/ldconfig # For shared libraries
%preun
# Add pre-uninstall scripts here.%postun
# Add post-uninstall scripts here.