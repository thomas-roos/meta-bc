DESCRIPTION = "SSL/TLS library"
LICENSE = "GPLv2"
HOMEPAGE = "https://tls.mbed.org/"
SECTION = "libs/network"

INC_PR = "r0"

PN = "polarssl-bc"
PROVIDES = "libpolarssl-bc"

EXTRA_OECMAKE += "-DENABLE_SHARED=ON"

PACKAGES += " ${PN}-utils"

FILES:${PN}-utils += "${bindir}/polarssl_*"

inherit cmake

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}

