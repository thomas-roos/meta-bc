PR = "r0"

SRC_URI = "http://www.soft-switch.org/downloads/spandsp/${PN}-0.0.6pre18.tgz"
DEPENDS = "tiff"
S = "${WORKDIR}/spandsp-0.0.6"

# *cough*
do_configure:append() {
      rm config.log
}

DESCRIPTION = "A library of many DSP functions for telephony."
HOMEPAGE = "http://www.soft-switch.org"
SECTION = "libs"
LICENSE = "LGPL"
DEPENDS = "tiff libxml2"

inherit autotools pkgconfig

#PARALLEL_MAKE = ""

SRC_URI[md5sum] = "98330bc00a581ed8d71ebe34afabbcf9"
LIC_FILES_CHKSUM = "file://COPYING;md5=8791c23ddf418deb5be264cffb5fa6bc"


