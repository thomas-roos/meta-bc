DESCRIPTION = "SIP stack from Belledonne Communications"
LICENSE = "GPL-3.0-only"
HOMEPAGE = "http://www.linphone.org/technical-corner/belle-sip/overview"
SECTION = "libs/network"

INC_PR = "r0"

DEPENDS = "libantlr3c-bc bctoolbox belr zlib antlr3-native openssl"

PREFERRED_VERSION_antlr3-native = "3.5.2"
PROVIDES = "libbellesip"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

inherit pkgconfig cmake gettext

EXTRA_OECMAKE += " -DENABLE_STRICT=NO -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_UNIT_TESTS=NO"
EXTRA_OECMAKE += " -DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"
EXTRA_OECMAKE += " -DCMAKE_PREFIX_PATH=${STAGING_LIBDIR}/cmake"
EXTRA_OECMAKE += " -DCMAKE_C_FLAGS='-Wno-error=implicit-function-declaration -Wno-error=incompatible-pointer-types'"

CFLAGS += " -Wno-error -std=gnu11"

INSANE_SKIP:${PN} += "dev-deps"
INSANE_SKIP:${PN}-staticdev += "buildpaths"

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/pkgconfig \
    ${libdir}/*.a \
    ${libdir}/cmake \
    ${datadir}/belr \
    ${prefix}/share/BelleSIP"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/find_package(BCToolbox/i find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
}
