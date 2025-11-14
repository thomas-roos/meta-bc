require bctoolbox.inc

inherit gitpkgv

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '556a46fc77ee27d22554bb7d1b01ed93e1bf5c21')
}

# For visualisation
python () {
    print("")
    print("bctoolbox")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "01285f4b49e4c06534058aa80dc6e9567cca9b16"
#SRCREV = "74918554d02e66e2fb1df0bbedcf7f19a7266584"
SRC_URI = "git://gitlab.linphone.org/BC/public/bctoolbox.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=ff3103b5db8ba4e2c66c511b7a73e407"
