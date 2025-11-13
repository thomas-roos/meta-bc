PR = "r3"
SRC_URI = "git://gitlab.linphone.org/BC/public/mswebrtc.git;protocol=https;protocol=https;nobranch=1"

python () {
    if d.getVar('LATEST_REVISIONS', True) == "1":
        d.setVar('SRCREV', '${AUTOREV}')
    else:
        d.setVar('SRCREV', '19814348807cdab7f13637993a05a696f47685eb')
}

# For visualisation
python () {
    print("")
    print("mswebrtc")
    print(d.getVar('SRCREV', True))
}


require mswebrtc-common.inc
