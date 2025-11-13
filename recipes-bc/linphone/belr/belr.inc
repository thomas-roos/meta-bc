DESCRIPTION = "Belr is Belledonne Communications' language recognition library."
LICENSE = "GPLv2"
HOMEPAGE = "http://www.linphone.org/"
SECTION = "libs/network"

INC_PR = "r0"
DEPENDS = "bctoolbox"
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_STRICT=NO "
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"
PROVIDES = "libbelr"

inherit cmake

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/*.a \
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
}
