
DESCRIPTION = "Mediastreamer2 plugin adding support for WebRTC features (iSAC codec, AEC...)"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "GPLv2"
DEPENDS = "libmediastreamer2 ortp bctoolbox pkgconfig-native"
PROVIDES = "libmswebrtc"

#INHIBIT_PACKAGE_STRIP = "1"  
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_ISAC=NO -DENABLE_AECM=YES -DENABLE_AEC=NO -DENABLE_ILBC=YES"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

FILES_${PN} = "${libdir}/mediastreamer/plugins/*.so"
#FILES_${PN}-dev = "${libdir}/mediastreamer/plugins/*.so"
FILES_${PN}-dbg = "${libdir}/mediastreamer/plugins/.debug ${libdir}/mediastreamer/plugins/.debug/*.so.* /usr/src/debug"

FILES_${PN}-staticdev += " ${libdir}/mediastreamer/plugins/*.a "

LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"


inherit  cmake

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i 's/find_package(ORTP REQUIRED)/find_package(PkgConfig REQUIRED)\npkg_check_modules(ORTP REQUIRED ortp)/' ${S}/CMakeLists.txt
    sed -i 's/find_package(BcToolbox REQUIRED)/pkg_check_modules(BCTOOLBOX REQUIRED bctoolbox)/' ${S}/CMakeLists.txt
} 
