require ortp.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '005a9db5ae3e8bb725323ee8864f3e3d1617d6f9')
}

# For visualisation
python () {
    print("")
    print("ortp")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "5f8fcddce392f1510768949a4691f9e8c170badb"
SRC_URI = "git://gitlab.linphone.org/BC/public/ortp.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=eb1e647870add0502f8f010b19de32af"
