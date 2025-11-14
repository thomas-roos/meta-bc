require linphone.inc

inherit gitpkgv

PR = "${INC_PR}.0"

SRCREV = "3e08ecc97001da2f1cc40b3d045b9bba6ceab089"
SRC_URI = "git://gitlab.linphone.org/BC/public/linphone.git;protocol=https;branch=release/5.3 \
           file://fix-libxml2-deprecated-api.patch"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=eb1e647870add0502f8f010b19de32af"
