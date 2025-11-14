require bzrtp.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '922bd8a4f14e37c8a7702ecbc4e4d78d360081dd')
}

# For visualisation
python () {
    print("")
    print("bzrtp")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "37adaa0536432149a51332d8eb04973a3ba6bac9"
SRC_URI = "git://gitlab.linphone.org/BC/public/bzrtp.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=ff3103b5db8ba4e2c66c511b7a73e407"
