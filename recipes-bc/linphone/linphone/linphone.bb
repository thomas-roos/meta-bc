require linphone.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    d.setVar('SRCREV', '${AUTOREV}')
}

# For visualisation
python () {
    print("")
    print("linphone")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "7e2a6a56fa6a70a6ec3875cea26f1f1e17910917"
SRC_URI = "git://gitlab.linphone.org/BC/public/linphone.git;protocol=https;branch=master \
           file://fix-libxml2-deprecated-api.patch"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=eb1e647870add0502f8f010b19de32af"
