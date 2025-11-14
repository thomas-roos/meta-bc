DESCRIPTION = "Powerful and lightweight streaming engine specialized for voice and video telephony applications"
LICENSE = "AGPL-3.0-only"
HOMEPAGE = "http://www.linphone.org/technical-corner/mediastreamer2/overview"
SECTION = "libs/network"

INC_PR = "r0"

DEPENDS = "libortp speex alsa-lib spandsp libv4l virtual/gettext openssl"
PROVIDES += "libmediastreamer2"

PACKAGECONFIG ??= "vpx opus bzrtp gsm ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11', '', d)}"
PACKAGECONFIG[ffmpeg] = ",  -DENABLE_FFMPEG=NO, ffmpeg"
PACKAGECONFIG[vpx] = ", -DENABLE_VPX=NO, libvpx"
PACKAGECONFIG[opus] = ", -DENABLE_OPUS=NO, libopus"
PACKAGECONFIG[srtp] = ", -DENABLE_SRTP=NO, libsrtp-bc"
PACKAGECONFIG[bzrtp] = " -DENABLE_BZRTP=YES, -DENABLE_BZRTP=NO, libbzrtp"
PACKAGECONFIG[gsm] = " -DENABLE_GSM=YES, -DENABLE_GSM=NO, libgsm"
PACKAGECONFIG[x11] = " -DENABLE_X11=YES -DENABLE_XV=YES,  -DENABLE_X11=NO -DENABLE_XV=NO, libxv"

inherit cmake lib_package pkgconfig python3native 
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_GLX=ON -DENABLE_GL=NO -DENABLE_GLX_DEFAULT_VALUE=NO -DENABLE_PCAP=NO -DENABLE_BV16=NO -DENABLE_DOC=NO -DENABLE_STRICT=NO -DENABLE_UNIT_TESTS=NO -DENABLE_FFMPEG=NO -DENABLE_SRTP=NO -DENABLE_TOOLS=NO"
EXTRA_OECMAKE += "-DCMAKE_INSTALL_DATAROOTDIR=share -DCMAKE_INSTALL_LIBDIR=lib"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INSANE_SKIP:${PN} += "installed-vs-shipped"
INSANE_SKIP:${PN}-dev += "installed-vs-shipped"
INSANE_SKIP:${PN}-staticdev += "buildpaths"

# Skip SPDX creation due to missing dependency metadata
do_create_spdx[noexec] = "1"
do_create_package_spdx[noexec] = "1"

CFLAGS += "-Wno-error=stringop-truncation -Wno-error=implicit-function-declaration -Wno-error=int-conversion"


#PACKAGES += "${PN}-bin"

#FILES_${PN}-bin = "${bindir}/mediastream ${bindir}/msaudiocmp"
FILES_${PN} += " ${libdir}/mediastreamer ${datadir}/images"
ALLOW_EMPTY:${PN} = "1"

FILES_${PN}-dev += " \
    ${includedir} \
    ${libdir}/lib*.so \
    ${libdir}/*.la \
    ${libdir}/*.a \
    ${libdir}/pkgconfig \
    ${datadir}/Mediastreamer2"

#inherit autotools pkgconfig gettext

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
    sed -i '/^project(/a find_package(OpenSSL REQUIRED)' ${S}/CMakeLists.txt
    sed -i 's/find_package(ORTP/find_package(Ortp/g' ${S}/CMakeLists.txt
    sed -i 's/const char \*key/const uint8_t *key/g' ${S}/src/crypto/ms_srtp.cpp
    sed -i 's/MSEKTParametersSet \*ekt)/const MSEKTParametersSet *ekt)/g' ${S}/src/crypto/ms_srtp.cpp
}

