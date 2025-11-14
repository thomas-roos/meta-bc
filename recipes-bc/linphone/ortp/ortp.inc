DESCRIPTION = "Real-time transport protocol (RFC 3550) library"
LICENSE = "AGPL-3.0-only"
HOMEPAGE = "http://www.linphone.org/technical-corner/ortp/overview"
SECTION = "libs/network"

INC_PR = "r0"
DEPENDS = "libbctoolbox openssl"

CFLAGS += "-Wno-error=stringop-truncation"

EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_STRICT=NO -DENABLE_UNIT_TESTS=NO "
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"
PROVIDES = "libortp"

INSANE_SKIP:${PN}-dev += "buildpaths"

inherit cmake

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/find_package(BCToolbox/i find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
}

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/*.la \
    ${libdir}/*.a \
    ${libdir}/*.o \
    ${libdir}/pkgconfig \
    ${datadir}/Ortp \
    ${prefix}/share/oRTP/" 

