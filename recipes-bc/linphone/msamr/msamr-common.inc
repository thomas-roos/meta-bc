DESCRIPTION = "Mediastreamer2 plugin adding support for AMR codec"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "GPLv3"

LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504"

DEPENDS = "linphone opencore-amr"
DEPENDS:append:wideband = " vo-amrwbenc"

MSAMR_WIDEBAND = ""
MSAMR_WIDEBAND:wideband = " -DENABLE_WIDEBAND=YES"

PACKAGES = " ${PN}-dbg ${PN} ${PN}-doc ${PN}-dev ${PN}-staticdev"

EXTRA_OECMAKE = "${MSAMR_WIDEBAND}"
EXTRA_OECMAKE += " -DCMAKE_SKIP_INSTALL_RPATH=ON"

FILES:${PN} = "${libdir}/mediastreamer/plugins/*.so \
    ${libdir}/lib*.so "

FILES:${PN}-dbg += "\
        ${libdir}/.debug/ \
        ${libdir}/mediastreamer/plugins/.debug"

FILES:${PN}-dev = "${libdir}/mediastreamer/plugins/*.la "

FILES:${PN}-staticdev = "${libdir}/mediastreamer/plugins/*.a "


inherit cmake  pkgconfig
