require belcard.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '1ef124ff1d51f24d07fbb07e4b89712c8a0ea3eb')
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
