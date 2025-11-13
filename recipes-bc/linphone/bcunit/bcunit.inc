DESCRIPTION = "BCunit library"
LICENSE = "GPLv2"
HOMEPAGE = "https://tls.mbed.org/"
SECTION = "libs/"

INC_PR = "r0"

PN = "bcunit"
PROVIDES = "bcunit"

EXTRA_OECMAKE += " -DENABLE_SHARED=ON "
FILES:${PN} += "${datadir}/BCUnit \
		${datadir}/BCunit \
		/usr/doc"

inherit cmake pkgconfig

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}

