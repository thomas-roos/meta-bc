DESCRIPTION = "SIP stack from Belledonne Communications"
LICENSE = "GPL-3.0-only"
SECTION = "libs/network"

INC_PR = "r0"


DEPENDS = "libpolarssl-bc bcunit openssl"
PROVIDES = "libbctoolbox"

CFLAGS += "-Wno-error=array-parameter -Wno-error=stringop-overflow"

#-DCMAKE_BUILD_TYPE=Release
EXTRA_OECMAKE += " -DENABLE_STATIC=NO -DENABLE_TESTS=NO -DENABLE_TESTS_COMPONENT=NO -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_STRICT=NO -DENABLE_MBEDTLS=OFF -DENABLE_OPENSSL=ON"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

INSANE_SKIP:${PN} += "dev-deps "

FILES:${PN}-dev += "${datadir}/BCToolbox"

inherit cmake pkgconfig

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}

#PACKAGES = "${PN}-dbg ${PN} ${PN}-doc ${PN}-dev "


