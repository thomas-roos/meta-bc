DESCRIPTION = "GSM Audio Library"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "CLOSED"

INC_PR = "r2"

inherit cmake pkgconfig

do_create_spdx[noexec] = "1"

PROVIDES = "libgsm"
PACKAGES = " ${PN}-dbg ${PN} ${PN}-doc ${PN}-dev "

CFLAGS += "-Wno-error=implicit-function-declaration"
#EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON"
FILES:${PN} += " \
    ${libdir}/lib*.so "    

FILES:${PN}-dbg += "\
 	${libdir}/.debug/ \"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}
