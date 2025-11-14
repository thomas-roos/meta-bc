DESCRIPTION = "Belcard is a C++ library to manipulate VCard standard format"
LICENSE = "GPLv2"
HOMEPAGE = "http://www.linphone.org/"
SECTION = "libs/network"

INC_PR = "r0"
DEPENDS = "libbelr openssl"
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON "
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"
EXTRA_OECMAKE += "-DENABLE_UNIT_TESTS=NO -DENABLE_TOOLS=NO"
PROVIDES = "libbelcard"

inherit cmake

FILES:${PN}-dev = " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/*.a \
    ${prefix}/share/Belcard \
    ${prefix}/share/BelCard \
    ${prefix}/share/belr \
    ${prefix}/share/belcard_tester \
    ${prefix}/share/belcard_tester/vcards \
    ${prefix}/share/belcard_tester/vcards/foldtest.vcf \
    ${prefix}/share/belcard_tester/vcards/unfoldtest.vcf \
    ${prefix}/share/belcard_tester/vcards/vcard.vcf \
    ${prefix}/share/belcard_tester/vcards/vcards.vcf"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/^project(/a find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
}

