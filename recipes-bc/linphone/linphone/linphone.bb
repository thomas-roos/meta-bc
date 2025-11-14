require linphone.inc

inherit gitpkgv

PR = "${INC_PR}.0"

SRCREV = "bb854e30a2f4af8088fa68adae6c1e92c1d2f226"
SRC_URI = "git://gitlab.linphone.org/BC/public/linphone.git;protocol=https;nobranch=1 \
           file://fix-libxml2-deprecated-api.patch"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=eb1e647870add0502f8f010b19de32af"

INSANE_SKIP:${PN} += "buildpaths"
