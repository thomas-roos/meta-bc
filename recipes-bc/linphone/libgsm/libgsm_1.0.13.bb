require libgsm.inc

inherit gitpkgv
PR = "${INC_PR}.0"



SRCREV = "0f8822b5326c76bb9dc4c6b552631f51792c3982"
SRC_URI = "git://gitlab.linphone.org/BC/public/external/gsm.git;protocol=https;branch=linphone"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"
