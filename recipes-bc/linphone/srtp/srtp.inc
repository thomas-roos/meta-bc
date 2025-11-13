DESCRIPTION = "SRTP is a security profile for RTP that adds confidentiality, message authentication, and replay protection to that protocol."
LICENSE = "BSD"
HOMEPAGE = "http://srtp.sourceforge.net/srtp.html"
SECTION = "libs/network"

INC_PR = "r0"

PN = "srtp-bc"
PROVIDES = "libsrtp-bc"

inherit cmake

do_create_spdx[noexec] = "1"

do_configure:prepend() {
    sed -i 's/cmake_minimum_required(VERSION [0-9.]*)/cmake_minimum_required(VERSION 3.5)/' ${S}/CMakeLists.txt
}