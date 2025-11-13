DESCRIPTION = "Linphone version of antlr3"
LICENSE = "BSD-3-Clause"
HOMEPAGE = "http://www.antlr3.org/"
SECTION = "libs"

INC_PR = "r0"

PN = "antlr3c-bc"
PROVIDES = "libantlr3c-bc"
S = "${UNPACKDIR}/${BPN}-${PV}/runtime/C"
ALLOW_EMPTY:${PN} = "1"

inherit cmake pkgconfig

do_configure:prepend() {
    if ! grep -q "cmake_minimum_required" ${S}/CMakeLists.txt; then
        sed -i '1i cmake_minimum_required(VERSION 3.5)' ${S}/CMakeLists.txt
    fi
}

do_install:append() {
	mkdir -p ${STAGING_DIR_NATIVE}/usr/share/java
	cp ${S}/../../antlr-3.4-complete.jar ${STAGING_DIR_NATIVE}/usr/share/java/antlr.jar
}
