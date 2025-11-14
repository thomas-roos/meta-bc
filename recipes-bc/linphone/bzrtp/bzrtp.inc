DESCRIPTION = "Opensource implementation of ZRTP keys exchange protocol"
LICENSE = "GPL-3.0-only"
HOMEPAGE = "https://github.com/BelledonneCommunications/bzrtp"
SECTION = "libs/network"

INC_PR = "r0"

DEPENDS = "bctoolbox libxml2 zlib openssl"
PROVIDES = "libbzrtp"

FILES:${PN}-dev += "${datadir}/BZRTP"

EXTRA_OECMAKE += " -DENABLE_STATIC=NO  -DENABLE_TESTS=OFF -DENABLE_STRICT=NO"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

inherit cmake

CFLAGS += "-Wno-error"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/find_package(BCToolbox/i find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
}
