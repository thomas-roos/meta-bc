require bcunit.inc

PR = "${INC_PR}.0"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', 'd8d2f4b40209e06b400f893cce58e4c6ba73341d')
}

# For visualisation
python () {
    print("")
    print("bcunit")
    print(d.getVar('SRCREV', True))
}

SRC_URI = "git://gitlab.linphone.org/BC/public/bcunit.git;protocol=https;nobranch=1"
PV = "git_${SRCREV}"


LIC_FILES_CHKSUM = "file://COPYING;md5=fbb9c6cedd181f6aff9467f29667a660"
