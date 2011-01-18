# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.21
# 
# >> macros
# << macros

Name:       omweather-qml
Summary:    OMWeather in qml
Version:    0.1
Release:    1
Group:      Applications/Internet
License:    GPLv2.1
URL:        https://garage.maemo.org/projects/omweather/
Source0:    %{name}-%{version}.tar.bz2
Source100:  omweather-qml.yaml
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(libcurl)

%description
Weather Forecast on Nokia N900. Ultra-customisable weather widget for showing forecast the way you want.



%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export PATH=/usr/lib/qt4/bin:$PATH
qmake PREFIX=%{_prefix}
make
# << build pre

#%configure --disable-static
#make %{?jobs:-j%jobs}

# >> build post
# << build post
%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%make_install

# >> install post
make INSTALL_ROOT=%{buildroot}/usr install
# << install post






%files
%defattr(-,root,root,-)
# >> files
# << files


