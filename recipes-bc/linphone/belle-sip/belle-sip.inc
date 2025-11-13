DESCRIPTION = "SIP stack from Belledonne Communications"
LICENSE = "GPLv2"
HOMEPAGE = "http://www.linphone.org/technical-corner/belle-sip/overview"
SECTION = "libs/network"

INC_PR = "r0"

DEPENDS = "libantlr3c-bc bctoolbox zlib antlr3-native"
PROVIDES = "libbellesip"

inherit pkgconfig cmake gettext

EXTRA_OECMAKE += " -DENABLE_STRICT=NO -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_TESTS=NO"
EXTRA_OECMAKE += " -DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

CFLAGS += " -Wno-error -std=gnu11"

INSANE_SKIP:${PN} += "dev-deps"

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/pkgconfig \
    ${libdir}/*.a \
    ${prefix}/share/BelleSIP"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}
