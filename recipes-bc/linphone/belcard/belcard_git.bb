require belcard.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '23451a56c5af367108d4fa64613abfdff99413c9')
}

# For visualisation
python () {
    print("")
    print("belcard")
    print(d.getVar('SRCREV', True))
}

SRC_URI = "git://gitlab.linphone.org/BC/public/belcard.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=ff3103b5db8ba4e2c66c511b7a73e407"
