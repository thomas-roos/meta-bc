require belle-sip.inc

inherit gitpkgv

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', 'e7a1bee26e08c46d9f6f9f403fa7db922d11b90e')
}

# For visualisation
python () {
    print("")
    print("belle-sip")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "c840e2192b2d0151cc895b844e44bfe0d2103fcf"
SRC_URI = "git://gitlab.linphone.org/BC/public/belle-sip.git;protocol=https;nobranch=1"
SRC_URI:remove = "file://pc.patch"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=ff3103b5db8ba4e2c66c511b7a73e407"


