SUMMARY = "Amazon Corretto 11 - Native"

require ${COREBASE}/../meta-aws/recipes-devtools/amazon-corretto/corretto-11-bin_${PV}.bb

inherit native

# Disable ptest for native
SRC_URI:remove = "file://run-ptest"
RDEPENDS:${PN}-ptest = ""


