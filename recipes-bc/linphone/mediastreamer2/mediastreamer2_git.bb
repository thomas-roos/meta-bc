require mediastreamer2.inc

inherit gitpkgv

PR = "${INC_PR}.0"

#python () {
#    if d.getVar('LATEST_REVISIONS', True) == "1":
#        d.setVar('SRCREV', '${AUTOREV}')
#    else:
#        d.setVar('SRCREV', '62d456a13813f0628326de7038f25c9dbd873b5e')
#}

# Use at least this commit to fix compilation error
SRCREV = "62d456a13813f0628326de7038f25c9dbd873b5e"

# For visualisation
python () {
    print("")
    print("mediastreamer2")
    print(d.getVar('SRCREV', True))
}

#SRCREV = "1afe5d921a30b8dc2dffb8838e600b4bf97e623d"
SRC_URI = "git://gitlab.linphone.org/BC/public/mediastreamer2.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"
PKGV = "${GITPKGVTAG}"

LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=eb1e647870add0502f8f010b19de32af"
