SUMMARY = "ANother Tool for Language Recognition v3"
HOMEPAGE = "http://www.antlr3.org/"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/BSD-3-Clause;md5=550794465ba0ec5312d6919e203a55f9"

SRC_URI = "https://www.antlr3.org/download/antlr-${PV}-complete.jar;name=jar;unpack=false"
SRC_URI[jar.sha256sum] = "e781de9b3e2cc1297dfdaf656da946a1fd22f449bd9e0ce1e12d488976887f83"

inherit native

do_compile() {
    :
}

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${datadir}/java
    
    install -m 0644 ${UNPACKDIR}/antlr-${PV}-complete.jar ${D}${datadir}/java/antlr3.jar
    
    cat > ${D}${bindir}/antlr3 <<EOF
#!/bin/sh
exec java -jar ${datadir}/java/antlr3.jar "\$@"
EOF
    chmod 0755 ${D}${bindir}/antlr3
}
