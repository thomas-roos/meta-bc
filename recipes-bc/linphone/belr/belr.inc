DESCRIPTION = "Belr is Belledonne Communications' language recognition library."
LICENSE = "GPL-3.0-only"
HOMEPAGE = "http://www.linphone.org/"
SECTION = "libs/network"

INC_PR = "r0"
DEPENDS = "bctoolbox openssl"
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_STRICT=NO -DENABLE_UNIT_TESTS=NO "
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"
EXTRA_OECMAKE += " -DCMAKE_PREFIX_PATH=${STAGING_LIBDIR}/cmake"
PROVIDES = "libbelr"

inherit cmake

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/*.a \
    ${libdir}/cmake \
    ${prefix}/share/Belr \
    ${datadir}/belr-tester/res \
    ${datadir}/belr-tester/res/sipgrammar.txt \
    ${datadir}/belr-tester/res/response.txt \
    ${datadir}/belr-tester/res/vcardgrammar.txt \
    ${datadir}/belr-tester/res/basicgrammar.txt \
    ${datadir}/belr-tester/res/register.txt"

CFLAGS += "-Wno-error"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/find_package(BCToolbox/i find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
}
